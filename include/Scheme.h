#ifndef SCHEME_H
#define SCHEME_H 

#include <string>
#include <iostream>
#include <unordered_map>

#include "Environment.h"
#include "Reader.h"
#include "SchemeObjectCreator.h"

class SchemeObject;

class Scheme {
public:
	Scheme(std::istream& instream);
	void main_loop();
    bool load_file(std::string fname);
private:
	const std::string cursor_;
    
    // Model
    Environment::Ptr the_global_environment_;
    SchemeObjectCreator obj_creator_;
    
    // Read
    SchemeReader reader_;

    // Evaluate
    SchemeObject* cons(SchemeObject* car, SchemeObject* cdr);
    SchemeObject* eval(SchemeObject* exp, Environment::Ptr env);
    SchemeObject* get_value_of_args(SchemeObject* args, Environment::Ptr env);
    SchemeObject* eval_let_form(SchemeObject* args, Environment::Ptr env);

    SchemeObject* prepare_apply_args(SchemeObject* args_to_apply);
    SchemeObject* convert_eval_form(SchemeObject* eval_args);

    // Print
	void write(SchemeObject* obj);
    void write_pair(SchemeObject* obj);
    void write_vector(SchemeObject* obj);
    void write_string(std::string);

    void print_welcome_message();
    void print_goodbye_message();
};


#endif // SCHEME_H
