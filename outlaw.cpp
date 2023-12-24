#include "outlaw.h"
#include "werewolf.h"
#include "orc.h"

void Outlaw::print(){
    std::cout << *this;
}

bool Outlaw::accept(std::shared_ptr<NPC> visitor){
    return visitor->fight(std::shared_ptr<Outlaw>(this,[](Outlaw*){}));
}

void Outlaw::save(std::ostream &os){
    os << OutlawType << "\n";
    NPC::save(os);
}

bool Outlaw::fight(std::shared_ptr<Orc> other){
    notify(other, false);
    return true;
}

bool Outlaw::fight(std::shared_ptr<Outlaw> other){
    notify(other, false);
    return false;
}

bool Outlaw::fight(std::shared_ptr<Werewolf> other){
    notify(other, true);
    return true;
}

std::ostream &operator<<(std::ostream &os, Outlaw &outlaw){
    os << "Outlaw: " << *static_cast<NPC *>(&outlaw) << "\n";
    return os;
}