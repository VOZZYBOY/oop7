#include "orc.h"
#include "outlaw.h"
#include "werewolf.h"

bool Orc::accept(std::shared_ptr<NPC> visitor){
    return visitor->fight(std::shared_ptr<Orc>(this,[](Orc*){}));
}

void Orc::print(){
    std::cout << *this;
}

void Orc::save(std::ostream &os){
    os << OrcType << "\n";
    NPC::save(os);
}

bool Orc::fight(std::shared_ptr<Orc> other){
    notify(other, false);
    return false;
}

bool Orc::fight(std::shared_ptr<Outlaw> other){
    notify(other, true);
    return true;
}

bool Orc::fight(std::shared_ptr<Werewolf> other){
    notify(other, false);
    return false;
}

std::ostream &operator<<(std::ostream &os, Orc &orc){
    os << "Orc: " << *static_cast<NPC *>(&orc) << "\n";
    return os;
}