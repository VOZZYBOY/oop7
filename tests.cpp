#include <gtest/gtest.h>
#include <memory>
#include "npc.h"
#include "orc.h"
#include "outlaw.h"
#include "werewolf.h"

TEST(test_01, Werewolf_constructor){
    int x = 60;
    int y = 100;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Werewolf>(x, y);

    EXPECT_EQ((a->position()).first,60);
    EXPECT_EQ((a->position()).second,100);
}

TEST(test_03, Werewolf_constructor){
    int x = 50;
    int y = 50;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Werewolf>(x, y);
    EXPECT_EQ(WerewolfType,a->get_type());
}

TEST(test_04, Werewolf_constructor){
    int x = 100;
    int y = 100;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Werewolf>(x, y);
    EXPECT_TRUE(a->is_alive());
}

TEST(test_07, Orc_constructor){
    int x = 100;
    int y = 100;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Orc>(x, y);

    EXPECT_EQ((a->position()).first,100);
    EXPECT_EQ((a->position()).second,100);
}

TEST(test_09, Orc_constructor){
    int x = 100;
    int y = 100;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Orc>(x, y);
    EXPECT_EQ(OrcType,a->get_type());
}

TEST(test_10, Orc_constructor){
    int x = 60;
    int y = 90;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Orc>(x, y);
    EXPECT_TRUE(a->is_alive());
}

TEST(test_13, Outlaw_constructor){
    int x = 100;
    int y = 90;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Outlaw>(x, y);

    EXPECT_EQ((a->position()).first,100);
    EXPECT_EQ((a->position()).second,90);
}

TEST(test_15, Outlaw_constructor){
    int x = 100;
    int y = 100;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Outlaw>(x, y);
    EXPECT_EQ(OutlawType,a->get_type());
}

TEST(test_16, Outlaw_constructor){
    int x = 100;
    int y = 100;

    std::shared_ptr<NPC> a;
    a = std::make_shared<Outlaw>(x, y);
    EXPECT_TRUE(a->is_alive());
}
