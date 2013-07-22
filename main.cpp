#include "Object.h"
#include "Scheme.h"

int main(int argc, char* args[]){
    Object::init_statics();
	Scheme interpreter(std::cin);
	interpreter.main_loop();
	return 0;
}
