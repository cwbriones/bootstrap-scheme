#include "Object.h"
#include <string>

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

bool Object::is_pair(){
    return type == PAIR;
}

bool Object::is_symbol(){
    return type == SYMBOL;
}

bool Object::is_true_obj(){
    return is_boolean() && data.boolean.value;
}

bool Object::is_false_obj(){
    return is_boolean() && !data.boolean.value;
}

bool Object::is_empty_list(){
    return type == EMPTY_LIST;
}

bool Object::is_tagged_list(std::string tag){
    return is_pair() &&
        data.pair.car->is_symbol() &&
        data.pair.car->data.symbol.value == tag;
}

bool Object::is_tagged_list(Object* tag){
    return is_pair() && 
        data.pair.car->is_symbol() &&
        data.pair.car == tag;
}

bool Object::is_self_evaluating(){
    return is_string() ||
           is_fixnum() ||
           is_character() ||
           is_boolean() ||
           is_empty_list();
}

Object* Object::car(){
    return this->data.pair.car;
}

Object* Object::cdr(){
    return this->data.pair.cdr;
}

Object* Object::cadr(){
    return (data.pair.cdr)->data.pair.car;
}
