#ifndef OBJECT_H
#define OBJECT_H

#include <string>

struct Object {
	enum {
		FIXNUM,
        BOOLEAN,
        CHARACTER,
        STRING,
        EMPTY_LIST,
        PAIR,
        SYMBOL
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
        struct {
            char* value;
        } symbol;
        struct {
            Object* car;
            Object* cdr;
        } pair;
	} data;

    bool is_fixnum();
    bool is_boolean();
    bool is_character();
    bool is_string();
    bool is_pair();
    bool is_symbol();

    bool is_false_obj();
    bool is_true_obj();
    bool is_empty_list();

    bool is_tagged_list(std::string tag);
    bool is_tagged_list(Object* tag);
    bool is_self_evaluating();

    Object* car();
    Object* cdr();
    Object* cadr();
};

#endif // OBJECT_H
