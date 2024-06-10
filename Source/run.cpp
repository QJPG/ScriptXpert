#include <iostream>

#include "Program.h"

int main(int argc, const char * argv[]) {
	/*for (int i = 0; i < argc; i++) {
		std::cout << argv[i] << std::endl;
	}*/

	Program program;
	program.setup();

	return 0;
}