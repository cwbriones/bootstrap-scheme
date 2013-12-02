#ifndef SCHEME_H
#define SCHEME_H 

#include <string>
#include <iostream>
#include <unordered_map>

#include "Environment.h"
#include "SchemeEvaluator.h"
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
    SchemeObjectCreator obj_creator_;
    
    // Read
    SchemeReader reader_;
    
    // Evaluate
    SchemeEvaluator evaluator_;

    // Print
	void write(SchemeObject* obj);
    void write_pair(SchemeObject* obj);
    void write_vector(SchemeObject* obj);
    void write_string(std::string);

    void print_welcome_message();
    void print_goodbye_message();
};


#endif // SCHEME_H
