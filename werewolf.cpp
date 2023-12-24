#include "werewolf.h"
#include "orc.h"
#include "outlaw.h"

bool Werewolf::accept(std::shared_ptr<NPC> visitor){
    return visitor->fight(std::shared_ptr<Werewolf>(this,[](Werewolf*){}));
}

void Werewolf::print(){
    std::cout << *this;
}

void Werewolf::save(std::ostream &os){
    os << WerewolfType << "\n";
    NPC::save(os);
}

bool Werewolf::fight(std::shared_ptr<Orc> other){
    notify(other, false);
    return false;
}

bool Werewolf::fight(std::shared_ptr<Outlaw> other){
    notify(other, true);
    return true;
}

bool Werewolf::fight(std::shared_ptr<Werewolf> other){
    notify(other, false);
    return false;
}

std::ostream &operator<<(std::ostream &os, Werewolf &werewolf){
    os << "Werewolf: " << *static_cast<NPC *>(&werewolf) << "\n";
    return os;
}