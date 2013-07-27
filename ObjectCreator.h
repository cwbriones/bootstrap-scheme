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

    Object* empty_list_obj = nullptr;
    Object* false_obj = nullptr;
    Object* true_obj = nullptr;

    Object* quote_symbol = nullptr;
    Object* define_symbol = nullptr;
    Object* set_symbol = nullptr;
    Object* ok_symbol = nullptr;

    Object* alloc_obj();
    std::unordered_map<std::string, Object*> symbols_;
};

#endif
