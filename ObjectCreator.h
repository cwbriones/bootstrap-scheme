#ifndef OBJECT_CREATOR_H
#define OBJECT_CREATOR_H value

#include <string>
#include <unordered_map>

class Object;

class ObjectCreator {
public:
    ObjectCreator();

    Object* make_fixnum(long value);
    Object* make_boolean(bool value);
    Object* make_string(std::string value);
    Object* make_pair(Object* car, Object* cdr);
    Object* make_character(char value);
    Object* make_symbol(std::string value);
    Object* make_empty_list();

    Object* make_special_form(std::string tag, Object* cdr);
private:
    void init_singletons();

    void init_symbols();
    void init_arithmetic_operators();
    void init_type_predicates();
    void init_type_conversions();

    // -----------------------------
    // Symbols
    // -----------------------------

    Object* empty_list_obj = nullptr;
    Object* false_obj = nullptr;
    Object* true_obj = nullptr;

    // -----------------------------
    // Symbols
    // -----------------------------

    Object* quote_symbol = nullptr;

    Object* define_symbol = nullptr;
    Object* set_symbol = nullptr;
    Object* ok_symbol = nullptr;
    Object* if_symbol = nullptr;
    Object* cons_symbol = nullptr;

    // -----------------------------
    // Primitive Procedures
    // -----------------------------

    // Arithmetic Operators
    Object* addition_symbol = nullptr;
    Object* subtract_symbol = nullptr;
    Object* multiply_symbol = nullptr;
    Object* divide_symbol = nullptr;
    
    // Type predicates
    Object* null_predicate = nullptr;
    Object* int_predicate = nullptr;
    Object* char_predicate = nullptr;
    Object* string_predicate = nullptr;
    Object* pair_predicate = nullptr;
    Object* proc_predicate = nullptr;
    
    // Type conversions
    Object* char_to_int = nullptr;
    Object* int_to_char = nullptr;
    Object* num_to_string = nullptr;
    Object* str_to_number = nullptr;
    Object* symbol_to_str = nullptr;
    Object* str_to_symbol = nullptr;

    Object* alloc_obj();
    std::unordered_map<std::string, Object*> symbols_;
};

#endif
