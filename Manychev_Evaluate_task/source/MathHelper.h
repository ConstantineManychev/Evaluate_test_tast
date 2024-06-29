#pragma once

#include <string>

class MathHelper
{
public:

	enum eErrorCodes
	{
		NONE = 0,
		EMPTY_EXPRESSION,
		INVALID_SYMBOL,
		MISSING_CLOSING_BRACKET,
		MISSING_OPENING_BRACKER,
		DIVISION_BY_ZERO,
		MISSING_VALUE,
		INVALID_EXPRESSION, 
		INVALID_BEGINING,
		MISSING_OPERATION
	};

private:

	static int findNumberFromCharsByPos(const char* aString, size_t& aPos);
	static int applyOp(int aFirstValue, int aSecondValue, char aOperation);

	static bool isValidChar(char aChar);

public:

	static bool evaluate(const char *expression, int &result);

	static std::string converteErrorCodeToString(eErrorCodes aErrorCode);
};
