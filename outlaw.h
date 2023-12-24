#pragma once
#include "npc.h"

struct Outlaw: public NPC{
public:
    Outlaw(int x, int y): NPC(OutlawType, x, y){}
    Outlaw(std::istream &is): NPC(OutlawType, is){}

    void print() override;
    bool accept(std::shared_ptr<NPC> visitor) override;
    void save(std::ostream &os) override;
    bool fight(std::shared_ptr<Orc> other) override;
    bool fight(std::shared_ptr<Outlaw> other) override;
    bool fight(std::shared_ptr<Werewolf> other) override;


    friend std::ostream &operator<<(std::ostream &os, Outlaw &outlaw);
};