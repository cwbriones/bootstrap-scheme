#include "Scheme.h"

int main(int argc, char* args[]){

	Scheme interpreter(std::cin);
	interpreter.main_loop();

	return 0;
}
