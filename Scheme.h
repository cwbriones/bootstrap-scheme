#ifndef SCHEME_H
#define SCHEME_H 

#include <string>
#include <iostream>
#include <unordered_map>

#include "Environment.h"
#include "Reader.h"
#include "ObjectCreator.h"

class Object;

class Scheme {
public:
	Scheme(std::istream& instream);
	void main_loop();
private:
	const std::string cursor_;
    Environment global_env_;
    Environment* env_;    
    // Read
    SchemeReader reader_;
    
    // Model
    ObjectCreator objcreator_;

    // Evaluate
    Object* eval(Object* exp);
    Object* cons(Object* exp1, Object* exp2);

    // Print
	void write(Object* obj);
    void write_pair(Object* obj);
    void write_string(std::string);
    void print_welcome_message();
};


#endif // SCHEME_H
