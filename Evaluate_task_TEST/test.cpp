#include "pch.h"

#include "MathHelper.h"
#include "MathHelper.cpp"

#include <gtest/gtest.h>

// Test for valid expressions
TEST(EvaluateTest, ValidExpressions)
{
	int result = 0;

	// Elementary check
	EXPECT_TRUE(MathHelper::evaluate("1 + 3", result));
	EXPECT_EQ(result, 4);

	// Brackets check
	EXPECT_TRUE(MathHelper::evaluate("(1 + 3) * 2", result));
	EXPECT_EQ(result, 8);

	EXPECT_TRUE(MathHelper::evaluate("(4 / 2) + 6", result));
	EXPECT_EQ(result, 8);

	EXPECT_TRUE(MathHelper::evaluate("4 + (12 / (1 * 2))", result));
	EXPECT_EQ(result, 10);

	// Begin from minus
	EXPECT_TRUE(MathHelper::evaluate("-1 + 2", result));
	EXPECT_EQ(result, 1);

	// Begin from plus
	EXPECT_TRUE(MathHelper::evaluate("+1 + 2", result));
	EXPECT_EQ(result, 3);

	// Negative answer check
	EXPECT_TRUE(MathHelper::evaluate("2 - 5", result));
	EXPECT_EQ(result, -3);

	// Zero answer check
	EXPECT_TRUE(MathHelper::evaluate("5 - 5", result));
	EXPECT_EQ(result, 0);

	// Left to right check
	EXPECT_TRUE(MathHelper::evaluate("1 + 3 * 2", result));
	EXPECT_EQ(result, 8);

	// First bracket check
	EXPECT_TRUE(MathHelper::evaluate("(1 + 3) * 2", result));
	EXPECT_EQ(result, 8);

	// Last brackets check
	EXPECT_TRUE(MathHelper::evaluate("1 + (3 * 2)", result));
	EXPECT_EQ(result, 7);

	// Complex brackets check1
	EXPECT_TRUE(MathHelper::evaluate("2 - (3 * 2) - (5 * 2)", result));
	EXPECT_EQ(result, -14);

	// Complex brackets check2
	EXPECT_TRUE(MathHelper::evaluate("2 - ((3 * 2) - (5 * 2))", result));
	EXPECT_EQ(result, 6);

	// Multiple operators
	EXPECT_TRUE(MathHelper::evaluate("3 + 2 - 3 * 4 / 2", result));
	EXPECT_EQ(result, 4);

	// Nested brackets
	EXPECT_TRUE(MathHelper::evaluate("2 * (3 + (4 - 2))", result));
	EXPECT_EQ(result, 10);

	// Spaces handling
	EXPECT_TRUE(MathHelper::evaluate("  1  +  3 ", result));
	EXPECT_EQ(result, 4);
}

// Test for invalid expressions
TEST(EvaluateTest, InvalidExpressions)
{
	int result = 0;

	// Missing closing bracket
	EXPECT_FALSE(MathHelper::evaluate("(1 + (12 * 2)", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::MISSING_CLOSING_BRACKET));

	// Missing opening bracket
	EXPECT_FALSE(MathHelper::evaluate("1 + 3 * 2)", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::MISSING_OPENING_BRACKER));

	// Begin from divider
	EXPECT_FALSE(MathHelper::evaluate("/2 + 3", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::INVALID_BEGINING));

	// Begin from multiplier
	EXPECT_FALSE(MathHelper::evaluate("*2 + 3", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::INVALID_BEGINING));

	// Division by zero
	EXPECT_FALSE(MathHelper::evaluate("4 / 0", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::DIVISION_BY_ZERO));

	// Invalid symbol
	EXPECT_FALSE(MathHelper::evaluate("1 + a", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::INVALID_SYMBOL));

	// Missing operation
	EXPECT_FALSE(MathHelper::evaluate("1 2", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::MISSING_OPERATION));

	// Missing value
	EXPECT_FALSE(MathHelper::evaluate("1 +", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::INVALID_EXPRESSION));

	// Invalid expression
	EXPECT_FALSE(MathHelper::evaluate("1 + + 2", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::MISSING_VALUE));

	// Nested invalid symbol
	EXPECT_FALSE(MathHelper::evaluate("1 + (3 * a)", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::INVALID_SYMBOL));

	// Multiple operators in a row
	EXPECT_FALSE(MathHelper::evaluate("1 + * 2", result));
	EXPECT_EQ(result, static_cast<int>(MathHelper::eErrorCodes::MISSING_VALUE));
}

int runTests(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}