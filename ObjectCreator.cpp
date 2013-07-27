#include "ObjectCreator.h"
#include "Object.h"

#include <cstring>

ObjectCreator::ObjectCreator(){
    init_singletons();
    init_symbols();
}

void ObjectCreator::init_singletons(){
    if (!empty_list_obj){
        empty_list_obj = alloc_obj();
        empty_list_obj->type = Object::EMPTY_LIST;
    }
    if (!false_obj){
        false_obj = alloc_obj();
        false_obj->type = Object::BOOLEAN;
        false_obj->data.boolean.value = false;
    }
    if (!true_obj){
        true_obj = alloc_obj();
        true_obj->type = Object::BOOLEAN;
        true_obj->data.boolean.value = true;
    }
}

void ObjectCreator::init_symbols(){
    if (!quote_symbol){
        quote_symbol = make_symbol("quote");
    }
    if (!define_symbol){
        define_symbol = make_symbol("define");
    }
    if (!ok_symbol){
        ok_symbol = make_symbol("ok");
    }
    if (!set_symbol){
        set_symbol = make_symbol("set!");
    }
}

Object* ObjectCreator::alloc_obj(){
    return new Object();
}

Object* ObjectCreator::make_fixnum(long value){
    Object* obj = alloc_obj();

    obj->type = Object::FIXNUM;
    obj->data.fixnum.value = value;
    return obj;
}

Object* ObjectCreator::make_boolean(bool value){
    if (value){
        return true_obj;
    }
    return false_obj;
}

Object* ObjectCreator::make_string(std::string value){
    Object* obj = alloc_obj();

    obj->type = Object::STRING;
    obj->data.string.value = new char[value.size() + 1];
    std::strcpy( obj->data.string.value, value.c_str() );

    return obj;
}

Object* ObjectCreator::make_pair(Object* car, Object* cdr){
    Object* obj = alloc_obj();

    obj->type = Object::PAIR;
    obj->data.pair.car = car;
    obj->data.pair.cdr = cdr;
    return obj;
}

Object* ObjectCreator::make_character(char value){
    Object* obj = alloc_obj();

    obj->type = Object::CHARACTER;
    obj->data.character.value = value;
    return obj;
}

Object* ObjectCreator::make_symbol(std::string value){
    // Check that the symbol isn't already in the table
    auto iter = symbols_.find(value);
    if (iter != symbols_.end()){
        return symbols_[value];
    } else {
        // Create the symbol and add it to the table
        Object* obj = alloc_obj();
        obj->type = Object::SYMBOL;

        obj->data.symbol.value = new char[value.size() + 1]; 
        std::strcpy(obj->data.symbol.value, value.c_str());

        symbols_[value] = obj;

        return obj;
    }
}

Object* ObjectCreator::make_empty_list(){
    return empty_list_obj;
}
