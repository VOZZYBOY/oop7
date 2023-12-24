#pragma once
#include "npc.h"

struct Orc: public NPC{
public:
    Orc(int x, int y): NPC(OrcType, x, y){}
    Orc(std::istream &is): NPC(OrcType, is){}

    void print() override;
    bool accept(std::shared_ptr<NPC> visitor) override;
    void save(std::ostream &os) override;
    bool fight(std::shared_ptr<Orc> other) override;
    bool fight(std::shared_ptr<Outlaw> other) override;
    bool fight(std::shared_ptr<Werewolf> other) override;
    
    friend std::ostream &operator<<(std::ostream &os, Orc &orc);
};