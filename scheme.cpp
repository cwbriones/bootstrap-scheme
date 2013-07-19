/*
 * Boostrap Scheme - a quick and dirty Scheme interpreter
 * Copyright (c) 2013 Christian Briones
 * 
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License version 3 along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <cstdlib>

struct Object {
	Object();

    static Object* true_obj;
    static Object* false_obj;
    static Object* empty_list_obj;

	enum {
		FIXNUM,
        BOOLEAN,
        CHARACTER,
        STRING,
        EMPTY_LIST
	} type;

	union {
        struct {
            bool value;
        } boolean;
		struct {
			long value;
		} fixnum;
        struct {
            char value;
        } character;
        struct {
            char* value;
        } string;
	} data;

    bool is_false();
    bool is_true();
    bool is_empty_list();
    bool is_fixnum();
    bool is_boolean();
    bool is_character();
    bool is_string();

    static void init_statics();
};

class Scheme {
private:
	const std::string cursor_;
	std::istream& instream_;

	void eat_whitespace();
    void eat_expected_word(std::string);
    void peek_expecting_delimiter();

	Object* read();
    Object* read_character();
    Object* read_string();

	void write(Object*);
    void write_string(std::string);
	Object* alloc_object();
public:
	Scheme(std::istream&);

	Object* make_fixnum(long);
    Object* make_character(char);
    Object* make_string(std::string);
	bool is_delimiter(char);

	void print_welcome_message();
	void main_loop();
};

Scheme::Scheme(std::istream& instream) : cursor_(">>"), instream_(instream){}

void Scheme::print_welcome_message(){
	std::cout << "Welcome to Bootstrap Scheme. " << std::endl;
	std::cout << "Use ctrl-c to exit." << std::endl;
}

/************************ MODEL ******************************/

Object::Object(){}

Object* Object::empty_list_obj = NULL;
Object* Object::false_obj = NULL;
Object* Object::true_obj = NULL;

bool Object::is_false(){
    return this == false_obj;
}

bool Object::is_true(){
    return this == true_obj;
}

bool Object::is_empty_list(){
    return this == empty_list_obj;
}

bool Object::is_fixnum(){
    return type == FIXNUM;
}

bool Object::is_boolean(){
    return type == BOOLEAN;
}

bool Object::is_character(){
    return type == CHARACTER;
}

bool Object::is_string(){
    return type == STRING;
}

void Object::init_statics(){
    if (false_obj == NULL){
        false_obj = new Object();
        false_obj->type = BOOLEAN;
        false_obj->data.boolean.value = 0;
    }
    if (true_obj == NULL){
        true_obj = new Object();
        true_obj->type = BOOLEAN;
        true_obj->data.boolean.value = 1;
    }
    if (empty_list_obj == NULL){
        empty_list_obj = new Object();
        empty_list_obj->type = EMPTY_LIST;
    }
}

Object* Scheme::alloc_object(){
	Object* obj;

	obj = new Object();
	if (obj == 0){
		std::cerr << "Out of memory" << std::endl;
		exit(1);
 	}
 	return obj;
}

Object* Scheme::make_fixnum(long value){
	Object* obj;

	obj = alloc_object();
	obj->type = Object::FIXNUM;
	obj->data.fixnum.value = value;
	return obj;
}

Object* Scheme::make_character(char value){
    Object* obj;

    obj = alloc_object();
    obj->type = Object::CHARACTER;
    obj->data.character.value = value;
    return obj;
}

Object* Scheme::make_string(std::string value){
    Object* obj;

    obj = alloc_object();
    obj->type = Object::STRING;
    obj->data.string.value = const_cast<char*>(value.c_str());

    return obj;
}

/************************ READ *******************************/

bool Scheme::is_delimiter(char c){
	return isspace(c) || c == EOF ||
			c == '('  || c == ')' ||
			c == '"'  || c == ';';
}

void Scheme::eat_whitespace(){
	int c;

	while ( (c = instream_.get()) != EOF ){
		if (isspace(c)){
			continue;
		}
		else if (c == ';'){ // Comments are also whitespace

			//Error here?
			while (((c = instream_.get()) != EOF) && (c != '\n'));
			continue;
		}
		instream_.unget();
		break;
	}
}

Object* Scheme::read(){
	char c;
	short sign = 1;
	long num = 0;

	eat_whitespace();

	c = instream_.get();
    if (c == '\"'){
        // Read in a string.
        return read_string();
    }
    else if (c == '('){
        // Read in the empty list.
        eat_whitespace();
        c = instream_.get();
        if (c == ')'){
            return Object::empty_list_obj;
        } else {
            std::cerr << "unexpected character \'" << c << "\', expected ')'" << std::endl;
            exit(1);
        }
    }
    else if (c == '#'){
        c = instream_.get();

        if (c == '\\'){
            return read_character();
            // Read in a character literal.
        } else {
            // Read in a boolean.
            switch(c){
                case 't':
                    return Object::true_obj;
                case 'f':
                    return Object::false_obj;
                default:
                    std::cerr << "unknown boolean literal." << std::endl;
                    exit(1);
            }
        } 
    }
    else if (isdigit(c) || (c == '-' && isdigit(instream_.peek())) ){
		// Read in a fixnum.
		if (c == '-'){
			sign = -1;
		}
		else {
			instream_.unget();
		}

		while (isdigit(c = instream_.get())){
			num = (num * 10) + (c - '0');
		}
		num *= sign;
		if (is_delimiter(c)){
			instream_.unget();
			return make_fixnum(num);
		}
		else {
			std::cerr << "number not followed by delimiter" << std::endl;
			exit(1);
		}
	}
	else {
		std::cerr << "bad input. unexpected character '" << c << "'." << std::endl;
		exit(1);
	}
	std::cerr << "read illegal state." << std::endl;
	exit(1);
}

Object* Scheme::read_character(){
    int c;
    c = instream_.get();

    switch(c){
        case EOF:
            std::cerr << "incomplete character literal." << std::endl;
            exit(1);
        case 's':
            if (instream_.peek() == 'p') {
                eat_expected_word("pace");
                peek_expecting_delimiter();
                return make_character(' ');
            }
            break;
        case 'n':
            if (instream_.peek() == 'e'){
                eat_expected_word("ewline");
                peek_expecting_delimiter();
                return make_character('\n');
            }
            break;
        case 't':
            if (instream_.peek() == 'a'){
                eat_expected_word("ab");
                peek_expecting_delimiter();
                return make_character('\t');
            }
    }
    peek_expecting_delimiter();
    return make_character(c);
}

Object* Scheme::read_string(){
    std::string buffer;
    char c = '\0';

    while( (c = instream_.get()) != '\"') {

        if (c == '\\'){
            // Escaped characters
            c = instream_.get();
            switch(c){
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
            }
        }
        if (c == EOF) {
            std::cerr << "non-terminated string literal." << std::endl;
            exit(1);
        }

        buffer += c;
    }

    return make_string(buffer);
}

void Scheme::eat_expected_word(std::string word){

    int c = 0;
    std::string::iterator iter = word.begin();

    while (iter != word.end()){
        c = instream_.get();
        if (c != *iter){
            std::cerr << "unexpected character '" << (char)c << '\'' << std::endl;
            exit(1);
        }
        iter++;
    }
}

void Scheme::peek_expecting_delimiter(){
    if (!is_delimiter(instream_.peek())){
        std::cerr << "character not followed by delimiter." << std::endl;
        exit(1);
    }
}

/********************** EVALUATE *****************************/

// For now we just echo until we have lists and symbols
Object* eval(Object* exp){
    // Empty list sent here is an error
	return exp;
}

/*********************** PRINT *******************************/

void Scheme::write(Object* obj){
	switch (obj->type){
		case Object::FIXNUM:
			std::cout << obj->data.fixnum.value;
			break;
        case Object::BOOLEAN:
            std::cout << '#' << ((obj->is_true()) ? 't' : 'f');
            break;
        case Object::CHARACTER:
            std::cout << "#\\";
            switch(obj->data.character.value){
                case '\n':
                    std::cout << "newline";
                    break;
                case ' ':
                    std::cout << "space";
                    break;
                case '\t':
                    std::cout << "tab";
                    break;
                default:
                    std::cout << obj->data.character.value;
            }
            break;
        case Object::STRING:
            write_string(obj->data.string.value);
            break;
        case Object::EMPTY_LIST:
            std::cout << "()";
            break;
		default:
			std::cerr << "unknown type, cannot write." << std::endl;
			exit(1);
	}
}

void Scheme::write_string(std::string str){
    std::cout << '\"';
    std::string::iterator iter = str.begin();

    while (iter != str.end()){
        switch(*iter){
            case '\n':
                std::cout << "\\n";
                break;
            case '\t':
                std::cout << "\\t";
                break;
            case '\"':
                std::cout << "\\\"";
                break;
            default:
                std::cout << *iter;
        }
        iter++;
    }
    std::cout << '\"';
}

/************************ REPL *******************************/

void Scheme::main_loop(){
	print_welcome_message();
	while (true) {
		std::cout << cursor_ << ' ';
		write(eval(read()));
		std::cout << std::endl;
	}
}

int main(int argc, char* args[]){
    Object::init_statics();
	Scheme interpreter(std::cin);
	interpreter.main_loop();
	return 0;
}
