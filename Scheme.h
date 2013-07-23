#ifndef SCHEME_H
#define SCHEME_H 

#include <string>
#include <iostream>
#include <unordered_map>

class Object;

class Scheme {
public:
	Scheme(std::istream&);
	void main_loop();
private:
	const std::string cursor_;
	std::istream& instream_;
    std::unordered_map<std::string, Object*> symbols_;

    // Model
	Object* alloc_object();
	Object* make_fixnum(long value);
    Object* make_character(char value);
    Object* make_string(std::string value);
    Object* make_boolean(bool value);
    Object* make_pair(Object* car, Object* cdr);
    Object* make_empty_list();
    Object* make_symbol(std::string value);

    // Read
	Object* read();
    Object* read_character();
    Object* read_string();
    Object* read_pair();
    Object* read_symbol();

	void eat_whitespace();
    void eat_expected_word(std::string word);
    void peek_expecting_delimiter();
	void print_welcome_message();
	bool is_delimiter(char);

    // Evaluate
    Object* eval(Object* exp);
    Object* cons(Object* exp1, Object* exp2);

    // Print
    void write_continue_marker();
	void write(Object* obj);
    void write_pair(Object* obj);
    void write_string(std::string);

};


#endif // SCHEME_H
