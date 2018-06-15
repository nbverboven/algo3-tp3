#include "../../src/Ball.h"
#include "gtest/gtest.h"

TEST(TestBall, TestEmptyConstructor)
{
	Ball b;
	EXPECT_EQ(b.getPosition(), std::make_pair(0, 0));
	EXPECT_EQ(b.getMovementDirection(), -1);
}

TEST(TestBall, TestSetPosition)
{
	Ball b;

	b.setPosition(4, 4);
	EXPECT_EQ(b.getPosition(), std::make_pair(4, 4));

	b.setPosition(1, 140);
	EXPECT_EQ(b.getPosition(), std::make_pair(1, 140));

	b.setPosition(-4, 504);
	EXPECT_EQ(b.getPosition(), std::make_pair(-4, 504));

	b.setPosition(0, -55);
	EXPECT_EQ(b.getPosition(), std::make_pair(0, -55));
}

TEST(TestBall, TestSetMovement)
{
	Ball b;
	b.setPosition(55, 55);
	b.setMovement(std::make_pair(6, 3));

	EXPECT_EQ(b.getMovementDirection(), 6);
}

TEST(TestBall, TestIfInvalidMovementDoesntMove)
{
	Ball b;
	EXPECT_EQ(b.getPosition(), std::make_pair(0, 0));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(0, 0));
}

TEST(TestBall, TestIfValidMovementMovesInTheRightDirection)
{
	Ball b;
	b.setPosition(55, 55);
	b.setMovement(std::make_pair(0, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(1, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(53, 53));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(2, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(53, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(3, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(53, 57));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(4, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 57));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(5, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(57, 57));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(6, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(57, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(7, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(57, 53));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(8, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 53));
}

TEST(TestBall, TestUndoMove)
{
	Ball b;
	b.setPosition(55, 55);
	b.setMovement(std::make_pair(0, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(1, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(53, 53));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(2, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(53, 55));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(3, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(53, 57));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(4, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 57));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(5, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(57, 57));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(6, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(57, 55));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(7, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(57, 53));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));

	b.setPosition(55, 55);
	b.setMovement(std::make_pair(8, 1));
	b.move();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 53));
	b.undoMove();
	EXPECT_EQ(b.getPosition(), std::make_pair(55, 55));
}

TEST(TestBall, TestCopyConstructor)
{
	Ball b;
	b.setPosition(-5, 10);
	b.setMovement(std::make_pair(2, 1));

	Ball c(b);
	c.setPosition(5, -10);
	c.setMovement(std::make_pair(3, 1));

	EXPECT_EQ(b.getPosition(), std::make_pair(-5, 10));
	EXPECT_EQ(c.getPosition(), std::make_pair(5, -10));
	EXPECT_EQ(b.getMovementDirection(), 2);
	EXPECT_EQ(c.getMovementDirection(), 3);
	EXPECT_EQ(b.finalPosition(), std::make_pair(-7, 10));
	EXPECT_EQ(c.finalPosition(), std::make_pair(3, -8));
}
