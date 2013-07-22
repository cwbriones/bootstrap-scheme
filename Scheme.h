#ifndef SCHEME_H
#define SCHEME_H 

#include <string>
#include <iostream>

class Object;

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

    Object* eval(Object* exp);

	void write(Object*);
    void write_string(std::string);
	Object* alloc_object();
public:
	Scheme(std::istream&);

	Object* make_fixnum(long);
    Object* make_character(char);
    Object* make_string(std::string);
    Object* make_boolean(bool value);
    Object* make_empty_list();

	bool is_delimiter(char);

	void print_welcome_message();
	void main_loop();
};


#endif // SCHEME_H
