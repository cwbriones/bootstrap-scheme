#include "Scheme.h"

#include <sstream>

int main(int argc, char* args[]){

	Scheme interpreter(std::cin);

    interpreter.load_file("standardlibrary.scm");
	interpreter.main_loop();

	return 0;
}
