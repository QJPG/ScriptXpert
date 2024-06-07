#include <iostream>
#include "AppDevice.h"

int main(int argc, const char * argv[]) {
	AppDevice app(argc, argv);
	if (app.run() < 1) app.step();

	return 0;
}