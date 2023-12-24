#include "npc.h"
#include "orc.h"
#include "outlaw.h"
#include "werewolf.h"
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <optional>
#include <array>

using namespace std::chrono_literals;
std::mutex mutex;

class TextObserver: public FightObserver{
private:
    TextObserver(){};

public:
        static std::shared_ptr<FightObserver> get(){
            static TextObserver instance;
            return std::shared_ptr<FightObserver>(&instance, [](FightObserver *) {});
        }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override{
        if (win){
            std::lock_guard<std::mutex> lck(mutex);
            std::cout << "---------Kill---------" << "\n";
            defender->print();
            std::cout << "was killed by" << "\n";
            attacker->print();
            std::cout << "\n";
        }
    }
};

std::shared_ptr<NPC> factory(std::istream &is){
    std::shared_ptr<NPC> result;
    int type = 0;
    if (is >> type){
        switch (type){
        case OrcType:
            result = std::make_shared<Orc>(is);
            break;
        case OutlawType:
            result = std::make_shared<Outlaw>(is);
            break;
        case WerewolfType:
            result = std::make_shared<Werewolf>(is);
            break;
        }
    }
    else{
        std::cerr << "Unexpected NPC type:" << type << "\n";
    }

    if (result)
        result->subscribe(TextObserver::get());

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y){
    std::shared_ptr<NPC> result;

    switch (type){
    case OrcType:
        result = std::make_shared<Orc>(x, y);
        break;
    case OutlawType:
        result = std::make_shared<Outlaw>(x, y);
        break;
    case WerewolfType:
        result = std::make_shared<Werewolf>(x, y);
        break;
    default:
        break;
    }

    if (result){
        result->subscribe(TextObserver::get());
    }

    return result;
}

std::ostream &operator<<(std::ostream &os, const std::set<std::shared_ptr<NPC>> &array){
    for (auto &n : array){
        n->print();
    }
    return os;
}

std::set<std::shared_ptr<NPC>> fight(const std::set<std::shared_ptr<NPC>> &array, size_t distance){
    std::set<std::shared_ptr<NPC>> dead_list;
    for (const auto &attacker : array){
        for (const auto &defender : array){
            if ((attacker != defender) && attacker->is_close(defender, distance) && defender->accept(attacker)){
                dead_list.insert(defender);
            }
        }
    }

    return dead_list;
}

struct print: std::stringstream{
    ~print()
    {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lck(mutex);
        std::cout << this->str();
        std::cout.flush();
    }
};

struct FightEvent{
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class FightManager{
private:
    std::queue<FightEvent> events;
    std::shared_mutex mtx;

    FightManager(){}

public:
    static FightManager &get(){
        static FightManager instance;
        return instance;
    }

    void add_event(FightEvent &&event){
        std::lock_guard<std::shared_mutex> lock(mtx);
        events.push(event);
    }

    void operator()(){
        while (true){
                std::optional<FightEvent> event;{
                    std::lock_guard<std::shared_mutex> lock(mtx);
                    if (!events.empty()){
                        event = events.back();
                        events.pop();
                    }
                }

                if (event){
                    try{
                        if (event->attacker->is_alive()){    
                            if (event->defender->is_alive()){
                                if (event->defender->accept(event->attacker)){
                                    event->defender->must_die();
                                }
                            }
                        }
                    }
                    catch (...){
                        std::lock_guard<std::shared_mutex> lock(mtx);
                        events.push(*event);
                    }
                }
                else{
                    std::this_thread::sleep_for(100ms);
                }
        }
    }
};


int main(){

    std::set<std::shared_ptr<NPC>> array; 

    const int MAX_X = 100;
    const int MAX_Y = 100;
    const int DISTANCE = 20;

    for (size_t i = 0; i < 50; ++i){
        array.insert(factory(NpcType(std::rand() % 3 + 1), std::rand() % MAX_X, std::rand() % MAX_Y));
    }

    std::chrono::seconds duration(30);
    auto startTime = std::chrono::steady_clock::now();

    std::cout << "************Monsters************" << "\n" << array << "\n";
    std::thread fight_thread(std::ref(FightManager::get()));

    std::thread move_thread([&array, MAX_X, MAX_Y, DISTANCE](){
            while (true){
                for (std::shared_ptr<NPC> npc: array){
                        if (npc->is_alive()){
                            int shift_x = std::rand() % 20 - 10;
                            int shift_y = std::rand() % 20 - 10;
                            npc->move(shift_x, shift_y, MAX_X, MAX_Y);
                        }
                }
                for (std::shared_ptr<NPC> npc: array){
                    for (std::shared_ptr<NPC> other: array){
                        if (other != npc){
                            if (npc->is_alive()){
                                if (other->is_alive()){
                                    if (npc->is_close(other, DISTANCE)){
                                        FightManager::get().add_event({npc, other});
                                    }
                                }
                            }
                        }
                    }
                }                            
                std::this_thread::sleep_for(1s);
            } });

    while (true){
        const int grid{20}, step_x{MAX_X / grid}, step_y{MAX_Y / grid};{
            std::array<char, grid * grid> fields{0};
            for (std::shared_ptr<NPC> npc : array){
                auto [x, y] = npc->position();
                int i = x / step_x;
                int j = y / step_y;

                if (npc->is_alive()){
                    switch (npc->get_type()){
                    case OrcType:
                        fields[i + grid * j] = 'O';
                        break;
                    case OutlawType:
                        fields[i + grid * j] = 'L';
                        break;
                    case WerewolfType:
                        fields[i + grid * j] = 'W';
                        break;

                    default:
                        break;
                    }
                }
                else{
                    fields[i + grid * j] = '.';
                }
            }

            std::lock_guard<std::mutex> lck(mutex);
            for (int j = 0; j < grid; ++j){
                for (int i = 0; i < grid; ++i){
                    char c = fields[i + j * grid];
                    if (c != 0){
                        std::cout << "[" << c << "]";
                    }
                    else{
                        std::cout << "[ ]";
                    }
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
        std::this_thread::sleep_for(1s);

        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - startTime > duration){
            break;
        }
    };
    
    std::set<std::shared_ptr<NPC>> alive;
        for (const auto& npc: array) {
            if (npc->is_alive()) {
                alive.insert(npc);
            }
        }
    std::cout << "************Alive************" << "\n" << alive << "\n";
    std::exit(EXIT_SUCCESS);
    return 0;
}