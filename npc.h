#pragma once

#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <random>
#include <fstream>
#include <set>
#include <math.h>
#include <shared_mutex>

struct NPC;
struct Orc;
struct Outlaw;
struct Werewolf;
enum NpcType{
    Unknown = 0,
    OutlawType = 1,
    OrcType = 2,
    WerewolfType = 3
};

struct FightObserver{
    virtual void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) = 0;
};

class NPC{
private:
    std::mutex mtx;
    NpcType type;
    int x = 0;
    int y = 0;
    bool alive = true;
    std::vector<std::shared_ptr<FightObserver>> observers;

public:
    NPC(NpcType t, int x, int y);
    NPC(NpcType t, std::istream &is);

    void subscribe(std::shared_ptr<FightObserver> observer);
    void notify(const std::shared_ptr<NPC> defender, bool win);
    virtual bool is_close(const std::shared_ptr<NPC> &other, size_t distance);
    virtual void save(std::ostream &os);
    virtual void print() = 0;
    virtual bool accept(std::shared_ptr<NPC> visitor) = 0;
    virtual bool fight(std::shared_ptr<Orc> other) = 0;
    virtual bool fight(std::shared_ptr<Outlaw> other) = 0;
    virtual bool fight(std::shared_ptr<Werewolf> other) = 0;

    std::pair<int, int> position();
    NpcType get_type();
    void move(int shift_x, int shift_y, int max_x, int max_y);

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);

    bool is_alive();
    void must_die();
};