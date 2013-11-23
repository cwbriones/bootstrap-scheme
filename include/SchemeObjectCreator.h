#ifndef SCHEMEOBJECTCREATOR_H_
#define SCHEMEOBJECTCREATOR_H_

#include <string>

#include "Environment.h"

class SchemeObject;

class SchemeObjectCreator {
public:
    SchemeObjectCreator();

    SchemeObject* make_fixnum(int value);
    SchemeObject* make_boolean(bool value);
    SchemeObject* make_character(char value);
    SchemeObject* make_string(std::string& value);
    SchemeObject* make_pair(SchemeObject* car, SchemeObject* cdr);
    SchemeObject* make_symbol(std::string value);
    SchemeObject* make_empty_list();

    SchemeObject* make_special_form(std::string tag, SchemeObject* cdr);
    SchemeObject* make_tagged_list(std::string tag, SchemeObject* obj);
    SchemeObject* make_tagged_list(
            std::string tag, SchemeObject* obj1, SchemeObject* obj2);

    SchemeObject* make_comp_procedure(
            Environment::Ptr& env, SchemeObject* params, SchemeObject* body);

    void setup_environment(Environment* env);
private:
    void init_keywords();
    void init_type_predicates();
    void init_type_conversions();

    // -----------------------------
    // Symbols
    // -----------------------------

    SchemeObject* quote_symbol = nullptr;
    SchemeObject* define_symbol = nullptr;
    SchemeObject* set_symbol = nullptr;
    SchemeObject* ok_symbol = nullptr;
    SchemeObject* if_symbol = nullptr;
    SchemeObject* cons_symbol = nullptr;

    // -----------------------------
    // Primitive Procedures
    // -----------------------------

    // Arithmetic Operators
    SchemeObject* addition_symbol = nullptr;
    SchemeObject* subtract_symbol = nullptr;
    SchemeObject* multiply_symbol = nullptr;
    SchemeObject* divide_symbol = nullptr;
    
    // Type predicates
    SchemeObject* null_predicate = nullptr;
    SchemeObject* int_predicate = nullptr;
    SchemeObject* char_predicate = nullptr;
    SchemeObject* string_predicate = nullptr;
    SchemeObject* pair_predicate = nullptr;
    SchemeObject* proc_predicate = nullptr;
    
    // Type conversions
    SchemeObject* char_to_int = nullptr;
    SchemeObject* int_to_char = nullptr;
    SchemeObject* num_to_string = nullptr;
    SchemeObject* str_to_number = nullptr;
    SchemeObject* symbol_to_str = nullptr;
    SchemeObject* str_to_symbol = nullptr;
};

#endif /* SCHEMEOBJECTCREATOR_H_ */
