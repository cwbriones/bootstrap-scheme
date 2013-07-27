#include "Object.h"

Object Object::empty_list_obj;
Object Object::false_obj;
Object Object::true_obj;

bool Object::is_false(){
    return this == &false_obj;
}

bool Object::is_true(){
    return this == &true_obj;
}

bool Object::is_empty_list(){
    return this == &empty_list_obj;
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

bool Object::is_pair(){
    return type == PAIR;
}

bool Object::is_symbol(){
    return type == SYMBOL;
}

bool Object::is_tagged_list(Object* tag){
    return is_pair() && 
        data.pair.car->is_symbol() &&
        data.pair.car == tag;
}

void Object::init_statics(){
    // Singletons
    false_obj.type = BOOLEAN;
    false_obj.data.boolean.value = 0;

    true_obj.type = BOOLEAN;
    true_obj.data.boolean.value = 1;

    empty_list_obj.type = EMPTY_LIST;
}
