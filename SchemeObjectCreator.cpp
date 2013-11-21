#include "SchemeObjectCreator.h"
#include "SchemeObject.h"

SchemeObjectCreator::SchemeObjectCreator() {
    init_symbols();
}

//============================================================================
// Creational Methods
//============================================================================

SchemeObject* SchemeObjectCreator::make_fixnum(int value) {
    return new SchemeFixnum(SchemeObject::FIXNUM, value);
}

SchemeObject* SchemeObjectCreator::make_boolean(bool value) {
    if (value) {
        return &SchemeObject::the_true_object_;
    }
    return &SchemeObject::the_false_object_;
}

SchemeObject* SchemeObjectCreator::make_character(char value) {
    return new SchemeCharacter(SchemeObject::CHARACTER, value);
}

SchemeObject* SchemeObjectCreator::make_string(std::string& value) {
    return new SchemeString(SchemeObject::STRING, value);
}

SchemeObject* SchemeObjectCreator::make_pair(SchemeObject* car, SchemeObject* cdr) {
    return new SchemePair(car, cdr);
}

SchemeObject* SchemeObjectCreator::make_symbol(std::string value) {
    return SchemeSymbol::make_symbol(value);
}

SchemeObject* SchemeObjectCreator::make_empty_list() {
    return &SchemeObject::the_empty_list_;
}

SchemeObject* SchemeObjectCreator::make_special_form(std::string tag, SchemeObject* cdr) {
    return new SchemePair(make_symbol(tag), cdr);
}

SchemeObject* SchemeObjectCreator::make_tagged_list(std::string tag, SchemeObject* obj) {
    return new SchemePair(make_symbol(tag), obj->cons(make_empty_list()));
}

SchemeObject* SchemeObjectCreator::make_procedure() {
    return nullptr;
}

//============================================================================
// Initialization
//============================================================================

void SchemeObjectCreator::init_symbols() {
    make_symbol("quote");
    make_symbol("define");
    make_symbol("set!");

    make_symbol("ok");
    make_symbol("if");
}

void SchemeObjectCreator::init_arithmetic_operators() {

}

void SchemeObjectCreator::init_type_predicates() {

}

void SchemeObjectCreator::init_type_conversions() {

}
