#include "MathHelper.h"

#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>

int main()
{
	//declare and initialize all necessary variables at the beginning of our method. 
	//This will make further searching easier when extending the method.
	std::string expr;
	int result = 0;

	bool isValid = false;

	while( true )
	{
		std::cout << "Input expression: ";
		getline(std::cin, expr);

		if (expr == "end" || expr == "exit")
		{
			std::cout << "Exit from application" << std::endl;
			break;
		}

		//Decided to move calculations into a separate class
		isValid = MathHelper::evaluate(expr.c_str(), result);

		std::cout << expr + " | " + ( isValid ? std::to_string(result) : "N/A" ) + " | " 
			+ ( isValid ? "true" : "false" + MathHelper::converteErrorCodeToString( static_cast<MathHelper::eErrorCodes>( result) ) );
		std::cout << std::endl;
	}

	return 0;
}