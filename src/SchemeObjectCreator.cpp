#include "Environment.h"

#include "Procedures/SchemePrimProcedure.h"
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
    return new SchemePair(
            make_symbol(tag), 
            make_pair(obj, make_empty_list())
        );
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

void SchemeObjectCreator::setup_environment(Environment* env) {
    
    // Arithmetic Operators
    env->define_variable_value(
            make_symbol("+")->to_symbol(), 
            new SchemeAddProcedure(this)
        );
    env->define_variable_value(
            make_symbol("-")->to_symbol(), 
            new SchemeSubProcedure(this)
        );
    env->define_variable_value(
            make_symbol("*")->to_symbol(), 
            new SchemeMultProcedure(this)
        );
    env->define_variable_value(
            make_symbol("/")->to_symbol(), 
            new SchemeDivProcedure(this)
        );
    env->define_variable_value(
            make_symbol("<")->to_symbol(), 
            new SchemeLessThanProcedure(this)
        );
    env->define_variable_value(
            make_symbol(">")->to_symbol(), 
            new SchemeGreaterThanProcedure(this)
        );
    env->define_variable_value(
            make_symbol("=")->to_symbol(), 
            new SchemeEqualsProcedure(this)
        );
    env->define_variable_value(
            make_symbol("quotient")->to_symbol(),
            new SchemeQuotientProcedure(this)
        );
    env->define_variable_value(
            make_symbol("modulo")->to_symbol(), 
            new SchemeModuloProcedure(this)
        );

    // Type Predicates
    env->define_variable_value(
            make_symbol("null?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::EMPTY_LIST)
        );
    env->define_variable_value(
            make_symbol("integer?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::FIXNUM)
        );
    env->define_variable_value(
            make_symbol("boolean?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::BOOLEAN)
        );
    env->define_variable_value(
            make_symbol("char?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::CHARACTER)
        );
    env->define_variable_value(
            make_symbol("symbol?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::SYMBOL)
        );
    env->define_variable_value(
            make_symbol("string?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::STRING)
        );
    env->define_variable_value(
            make_symbol("pair?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::PAIR)
        );
    env->define_variable_value(
            make_symbol("procedure?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::PRIMPROCEDURE)
        );

    // List operations
    env->define_variable_value(
            make_symbol("cons")->to_symbol(),
            new SchemeConsProcedure(this)
        );
}

void SchemeObjectCreator::init_type_predicates() {

}

void SchemeObjectCreator::init_type_conversions() {

}
