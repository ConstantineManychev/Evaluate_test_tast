#include "MathHelper.h"

#include <iostream>
#include <stack>
#include <vector>


bool MathHelper::evaluate(const char *expression, int &result)
{
	//declare and initialize all necessary variables at the beginning of our method. 
	std::stack<int> values;
	std::vector<char> ops;

	size_t exprSize = strlen(expression);

	bool isValid = exprSize > 0;//If expression is empty it is invalid
	bool isNumerExpected = false;

	size_t exprStartIndex = 0;

	if ( !isValid )
	{
		isValid = false;
		result = static_cast<int>(eErrorCodes::EMPTY_EXPRESSION);
	}

	//this logic will be needed in three places so I just take it beyond
	//We calculate two values, remove it from the 'values' and add result to the 'values'
	auto calcTwoValues = [&values, &ops]()
	{
		int val2 = values.top();
		values.pop();
		int val1 = values.top();
		values.pop();
		char op = ops.back();
		ops.pop_back();

		values.push( applyOp(val1, val2, op) );
	};


	for (size_t i = 0; i < exprSize && isValid; ++i)
	{
		//Skip char if it space
		if (expression[i] == ' ')
		{
			if (values.empty() && ops.empty())
			{
				++exprStartIndex;
			}
			continue;
		}

		bool isDigit = isdigit(expression[i]);

		if (!isValidChar(expression[i]) && !isDigit)
		{
			isValid = false;
			result = static_cast<int>(eErrorCodes::INVALID_SYMBOL);
		}
		else if (isDigit)//if current char is digit, we need to check if it is greater than 9 and/or negative and convert chars array to int
		{
			if (isNumerExpected || i == exprStartIndex)//We set isNumerExpected = false by default so first value can pass without exeption. But in another case it means that we missing the operation between values
			{
				int val = findNumberFromCharsByPos(expression, i);

				if (i == 1 && !ops.empty())
				{
					//If expression starting with '+' or '-'. We remove this operation
					if (ops.back() == '-' || ops.back() == '+')
					{
						ops.pop_back();
					}
					else if (ops.back() == '/' || ops.back() == '*')//Begin Expression from '/' or '*' is invalid case
					{
						isValid = false;
						result = static_cast<int>(eErrorCodes::INVALID_BEGINING);
					}
				}

				if (val == 0 && ops.back() == '/')
				{
					isValid = false;
					result = static_cast<int>(eErrorCodes::DIVISION_BY_ZERO);
				}

				values.push(val);

				isNumerExpected = false;
			}
			else
			{
				isValid = false;
				result = static_cast<int>(eErrorCodes::MISSING_OPERATION);
			}
		}
		else if (expression[i] == '(')
		{
			ops.push_back(expression[i]);

			isNumerExpected = true;
		}
		else if (expression[i] == ')')
		{
			if (std::find(ops.begin(), ops.end(), '(') != ops.end())//When bracket is close we need to check is exist bracket open
			{
				//Calculate all operations in brackets
				while (values.size() > 1 && ops.back() != '(')
				{
					calcTwoValues();
				}

				//'(' Still in 'ops' stack. Need to remove it when operations in brackets calculated
				if (!ops.empty() && ops.back() == '(')
				{
					ops.pop_back();
				}

				isNumerExpected = false;
			}
			else
			{
				isValid = false;
				result = static_cast<int>(eErrorCodes::MISSING_OPENING_BRACKER);
			}
		}
		else if (!isNumerExpected)
		{
			while ( values.size() > 1 && ops.back() != '(' )//We can't do any calculation if last char is '('. We don't write ')' to the 'ops', so we don't need to check it
			{
				calcTwoValues();
			}

			ops.push_back( expression[i] );

			isNumerExpected = true;
		}
		else
		{
			isValid = false;
			result = static_cast<int>(eErrorCodes::MISSING_VALUE);
		}
	}

	while ( !ops.empty() && isValid )
	{
		if (ops.size() == 1 && ops.back() == '(')
		{
			isValid = false;
			result = static_cast<int>(eErrorCodes::MISSING_CLOSING_BRACKET);
			ops.pop_back();
		}
		else
		{
			//If we have operator without two values it's means that somewhere missing value
			if (values.size() != 2)
			{
				isValid = false;
				result = static_cast<int>(eErrorCodes::INVALID_EXPRESSION);
			}
			else
			{
				calcTwoValues();
			}
		}
	}

	if ( !values.empty() && isValid )
	{
		result = values.top();
	}

	return isValid;
}

std::string MathHelper::converteErrorCodeToString( eErrorCodes aErrorCode )
{
	std::string result;

	switch (aErrorCode)
	{
		case eErrorCodes::EMPTY_EXPRESSION:
		{
			result = "(empty expression)";
			break;
		}
		case eErrorCodes::INVALID_SYMBOL:
		{
			result = "(invalid symbol)";
			break;
		}
		case eErrorCodes::MISSING_CLOSING_BRACKET:
		{
			result = "(missing closing bracket)";
			break;
		}
		case eErrorCodes::MISSING_OPENING_BRACKER:
		{
			result = "(missing opening bracket)";
			break;
		}
		case eErrorCodes::DIVISION_BY_ZERO:
		{
			result = "(division by zero)";
			break;
		}
		case eErrorCodes::MISSING_VALUE:
		{
			result = "(missing value between operators)";
			break;
		}
		case eErrorCodes::INVALID_EXPRESSION:
		{
			result = "(invalid expression)";
			break;
		}
		case eErrorCodes::INVALID_BEGINING:
		{
			result = "(invalid expression begin)";
			break;
		}
		case eErrorCodes::MISSING_OPERATION:
		{
			result = "(missing operation)";
			break;
		}
	}

	return result;
}

int MathHelper::findNumberFromCharsByPos(const char* aString, size_t& aPos)
{
	int result = 0;
	
	bool isNegative = false;

	//Value can be negative just if it first number in expression or first number under the parentheses
	if (aPos > 0 && aString[aPos - 1] == '-')
	{
		if (aPos == 1 || ( aPos > 1 
			&& ( aString[aPos - 2] == '(') || aString[aPos - 2] == ' ' ) )
		{
			isNegative = true;
		}
	}

	//While we get digit from the char it means that it stil one number
	while ( aPos < strlen( aString ) && isdigit( aString[aPos] ) )
	{
		result = (result * 10) + (aString[aPos] - '0');
		++aPos;
	}

	if (isNegative)
	{
		result = -result;
	}

	--aPos;

	return result;
}

int MathHelper::applyOp(int aFirstValue, int aSecondValue, char aOperation)
{
	int result = 0;

	switch (aOperation)
	{
		case '+':
		{
			result = aFirstValue + aSecondValue;
			break;
		}
		case '-': 
		{
			result = aFirstValue - aSecondValue;
			break;
		}
		case '*': 
		{
			result = aFirstValue * aSecondValue;
			break;
		}
		case '/': 
		{
			result = aFirstValue / aSecondValue;
			break;
		}
	}

	return result;
}

bool MathHelper::isValidChar(char aChar)
{
	return (aChar == '+' || aChar == '-' 
		|| aChar == '*' || aChar == '/' 
		|| aChar == '(' || aChar == ')');
}