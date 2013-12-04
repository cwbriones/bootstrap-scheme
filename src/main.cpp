#include "Scheme.h"

#include <sstream>

int main(int argc, char* args[]){

	Scheme interpreter(std::cin);

    interpreter.load_file("../src/scm/standardlibrary.scm");
	interpreter.main_loop();

	return 0;
}
