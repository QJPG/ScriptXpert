#include <iostream>
#include "AppDevice.h"

int main(int argc, const char * argv[]) {
	/*for (int i = 0; i < argc; i++) {
		std::cout << argv[i] << std::endl;
	}*/

	AppDevice app(argc, argv);
	if (app.run() < 1) app.step();
	else std::cerr << "Aborted!";

	return 0;
}