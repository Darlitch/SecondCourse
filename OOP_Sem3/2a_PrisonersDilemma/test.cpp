#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "PrisonersDilemma.hpp"

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(StrategyLogic, strCfg) {
    AbstractStrategy* strat = StratFactory<std::string, AbstractStrategy>::GetInstance()
                                  ->CreateStrat("stratCfg");
    strat->UpdateStrat("config/strat6.txt");
    ASSERT_EQ(strat->Move("cdd", 0), 'd');
    ASSERT_EQ(strat->Move("cdd", 1), 'd');
    ASSERT_EQ(strat->Move("cdd", 2), 'c');
    ASSERT_EQ(strat->Move("", 2), 'c');
    delete strat;
}

TEST(StrategyLogic, str1) {
    AbstractStrategy* strat = StratFactory<std::string, AbstractStrategy>::GetInstance()
                                  ->CreateStrat("strat1");
    ASSERT_EQ(strat->Move("cdd", 0), 'd');
    ASSERT_EQ(strat->Move("dcd", 1), 'd');
    ASSERT_EQ(strat->Move("cdd", 2), 'c');
    ASSERT_EQ(strat->Move("ddc", 2), 'd');
    ASSERT_EQ(strat->Move("ccc", 5), 'c');
    delete strat;
}

TEST(StrategyLogic, str2) {
    AbstractStrategy* strat = StratFactory<std::string, AbstractStrategy>::GetInstance()
                                  ->CreateStrat("strat2");
    ASSERT_EQ(strat->Move("cdd", 0), 'd');
    ASSERT_EQ(strat->Move("ddc", 1), 'c');
    ASSERT_EQ(strat->Move("ddd", 2), 'c');
    delete strat;
}

TEST(StrategyLogic, str3) {
    AbstractStrategy* strat = StratFactory<std::string, AbstractStrategy>::GetInstance()
                                  ->CreateStrat("strat3");
    ASSERT_EQ(strat->Move("cdd", 0), 'd');
    ASSERT_EQ(strat->Move("cdd", 1), 'c');
    ASSERT_EQ(strat->Move("cdc", 2), 'd');
    delete strat;
}

TEST(FactoryLogic, Create) {
    bool b = StratFactory<std::string, AbstractStrategy>::GetInstance()
                                  ->CreateStrat("strat5");
    ASSERT_EQ(b, 0);
}
