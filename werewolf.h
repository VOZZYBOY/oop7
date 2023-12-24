#pragma once
#include "npc.h"

struct Werewolf: public NPC{
public:
    Werewolf(int x, int y): NPC(WerewolfType, x, y){}
    Werewolf(std::istream &is): NPC(WerewolfType, is){}

    void print() override;
    bool accept(std::shared_ptr<NPC> visitor) override;
    void save(std::ostream &os) override;
    bool fight(std::shared_ptr<Orc> other) override;
    bool fight(std::shared_ptr<Outlaw> other) override;
    bool fight(std::shared_ptr<Werewolf> other) override;

    friend std::ostream &operator<<(std::ostream &os, Werewolf &werewolf);
};