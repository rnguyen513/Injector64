#include "injection.h"
#include <iostream>

using namespace injection;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Need PID" << std::endl;
		return EXIT_FAILURE; //expands to 1
	}

	std::cout <<
		"\nDLLINJECT\n"
		"/*!\n"
		" *	Author: Ryan Nguyen\n"
		" *	Date: 5/19/24\n"
		" *	https://github.com/rnguyen513\n"
		" */\n"
		<< std::endl;

	std::cout << "Target PID: " << atoi(argv[1]) << std::endl;

	//injector parameters
	DWORD PID = atoi(argv[1]);


	//call inject
	bool result;
	result = injectDLL(PID);

	std::cout << "Result: ";
	if (result) std::cout << "Success\n";
	else {
		std::cout << "Failure\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}