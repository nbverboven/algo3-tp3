#include "../../src/Player.h"
#include "gtest/gtest.h"

TEST(TestPlayer, TestEmptyConstructor)
{
	Player p(0, 0.5);
	EXPECT_EQ(p.getId(), 0);
	EXPECT_EQ(p.getPosition(), std::make_pair(-1, -1));
	EXPECT_EQ(p.probQuite(), 0.5);
	EXPECT_EQ(p.getBall(), nullptr);
}