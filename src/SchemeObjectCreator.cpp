/*
 * Copyright (C) Christian Briones, 2013
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "Environment.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"
#include "Procedures/TypeConversions.h"
#include "Procedures/StringProcedures.h"
#include "Procedures/VectorProcedures.h"

#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

#include "SchemeEnvironment.h"
#include "SchemeGarbageCollector.h"

#include "SchemeVector.h"

SchemeObjectCreator::SchemeObjectCreator() :
    the_interaction_env_(Environment::get_global_environment()),
    standard_input_(this),
    standard_output_()
{
    // Guard single instances
    the_interaction_env_.protect_from_gc();
    standard_input_.protect_from_gc();
    standard_output_.protect_from_gc();

    init_keywords();
}

//============================================================================
// Creational Methods
//============================================================================

SchemeObject* SchemeObjectCreator::make_unspecified() {
    return &SchemeObject::the_unspecified_object_;
}

SchemeObject* SchemeObjectCreator::make_fixnum(long value) {
    SchemeObject* obj = new SchemeObject();
    obj->init_fixnum(value);

    SchemeGarbageCollector::the_gc().add(obj);
    return obj;
}

SchemeObject* SchemeObjectCreator::make_flonum(double value) {
    SchemeObject* obj = new SchemeObject();
    obj->init_flonum(value);

    SchemeGarbageCollector::the_gc().add(obj);
    return obj;
}

SchemeObject* SchemeObjectCreator::make_boolean(bool value) {
    if (value) {
        return &SchemeObject::the_true_object_;
    }
    return &SchemeObject::the_false_object_;
}

SchemeObject* SchemeObjectCreator::make_character(char value) {
    SchemeObject* obj = new SchemeObject();
    obj->init_char(value);

    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_pair(SchemeObject* car, SchemeObject* cdr) {
    SchemeObject* obj = new SchemeObject();
    obj->init_pair(car, cdr);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_string(std::string value) {
    SchemeObject* obj = new SchemeString(SchemeObject::STRING, value);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_symbol(std::string value) {
    SchemeObject* obj = SchemeSymbol::make_symbol(value);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_vector(size_t size, SchemeObject* init) {
    SchemeObject* obj = new SchemeVector(size, init);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_vector(std::vector<SchemeObject*>& objects) {
    SchemeObject* obj = new SchemeVector(objects);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_prim_procedure(
        SchemePrimProcedure::procedure_t func,
        int argc,
        bool apply,
        bool eval)
{
    SchemeObject* obj = new SchemePrimProcedure(this, func, argc, apply, eval);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

void SchemeObjectCreator::make_procedure_in_env(
        Environment* env,
        const std::string& var,
        SchemePrimProcedure::procedure_t func,
        int argc)
{
    SchemeObject* proc;

    if (var == "apply") {
        proc = make_prim_procedure(func, argc, true, false);
    } else if (var == "eval") {
        proc = make_prim_procedure(func, argc, false, true);
    } else {
        proc = make_prim_procedure(func, argc, false, false);
    }
    env->define_variable_value(make_symbol(var)->to_symbol(), proc);
}

SchemeObject* SchemeObjectCreator::make_environment() {
    Environment::Ptr env = std::make_shared<Environment>();
    setup_environment(env.get());

    SchemeObject* obj = new SchemeEnvironment(env);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_interaction_environment() {
    return &the_interaction_env_;
}

SchemeObject* SchemeObjectCreator::make_null_environment() {
    SchemeObject* obj = new SchemeEnvironment(std::make_shared<Environment>());
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_input_port(const std::string& fname) {
    SchemeObject* obj = new SchemeInputPort(this, fname);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_output_port(const std::string& fname) {
    SchemeObject* obj = new SchemeOutputPort(fname);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_standard_input() {
    return &standard_input_;
}

SchemeObject* SchemeObjectCreator::make_standard_output() {
    return &standard_output_;
}

SchemeObject* SchemeObjectCreator::make_eof() {
    return &SchemeObject::the_eof_object_;
}

SchemeObject* SchemeObjectCreator::make_empty_list() {
    return &SchemeObject::the_empty_list_;
}
    
SchemeObject* SchemeObjectCreator::make_list(
        const std::vector<SchemeObject*>& objects)
{
    SchemeObject* list = make_empty_list();
    for (auto iter = objects.crbegin(); iter != objects.crend(); ++iter) {
        list = make_pair(*iter, list);
    }
    return list;
}

SchemeObject* SchemeObjectCreator::make_tagged_list(
        std::string tag, 
        SchemeObject* obj) 
{
    return make_pair(
        make_symbol(tag), 
        make_pair(obj, make_empty_list())
    );
}

SchemeObject* SchemeObjectCreator::make_tagged_list(
        std::string tag, 
        SchemeObject* obj1,
        SchemeObject* obj2) {

    return make_pair(
        make_symbol(tag), 
        make_pair(obj1, make_pair(obj2, make_empty_list()))
    );
}

SchemeObject* SchemeObjectCreator::make_comp_procedure(
    Environment::Ptr& env,
    SchemeObject* params, 
    SchemeObject* body)
{
    SchemeObject* obj = new SchemeCompoundProcedure(env, params, body);
    SchemeGarbageCollector::the_gc().add(obj);
    
    return obj;
}

//============================================================================
// Initialization
//============================================================================

void SchemeObjectCreator::init_keywords() {
    make_symbol("quote");
    make_symbol("define");
    make_symbol("set!");

    make_symbol("ok");
    make_symbol("if");
    make_symbol("lambda");
    make_symbol("let");
    make_symbol("begin");
    make_symbol("cond");
    make_symbol("and");
    make_symbol("or");
    make_symbol("else");
}

void SchemeObjectCreator::setup_environment(Environment* env) {
    // Note that by defining these in the environment
    // they are accessible to the garbage collector since it adds them
    // with the other bindings
    
    // Arithmetic Operators
    make_procedure_in_env(env, "+", ArithmeticProcedures::add);
    make_procedure_in_env(env, "-", ArithmeticProcedures::sub);
    make_procedure_in_env(env, "*", ArithmeticProcedures::mul);
    make_procedure_in_env(env, "/", ArithmeticProcedures::div);

    make_procedure_in_env(env, "=", ArithmeticProcedures::equals);
    make_procedure_in_env(env, "<", ArithmeticProcedures::less_than);
    make_procedure_in_env(env, ">", ArithmeticProcedures::greater_than);

    make_procedure_in_env(env, "quotient", ArithmeticProcedures::quotient, 2);
    make_procedure_in_env(env, "modulo", ArithmeticProcedures::modulo, 2);
    
    // List operations
    make_procedure_in_env(env, "length", ListProcedures::length, 1);
    make_procedure_in_env(env, "cons", ListProcedures::cons, 1);
    make_procedure_in_env(env, "car", ListProcedures::car, 1);
    make_procedure_in_env(env, "cdr", ListProcedures::cdr, 1);
    make_procedure_in_env(env, "set-car!", ListProcedures::set_car, 2);
    make_procedure_in_env(env, "set-cdr!", ListProcedures::set_cdr, 2);

    make_procedure_in_env(env, "random", MiscProcedure::random, 1);
    
    // Apply/Eval
    make_procedure_in_env(env, "apply", MiscProcedure::null_proc);
    make_procedure_in_env(env, "eval", MiscProcedure::null_proc);
    
    // Environment operations
    make_procedure_in_env(env, "null-environment",
            [](SchemeObject* args, SchemeObjectCreator* creator){
                return creator->make_null_environment();
            }, 0);
    make_procedure_in_env(env, "environment",
            [](SchemeObject* args, SchemeObjectCreator* creator){
                return creator->make_environment();
            }, 0);
    make_procedure_in_env(env, "interaction-environment",
            [](SchemeObject* args, SchemeObjectCreator* creator){
                return creator->make_interaction_environment();
            }, 0);

    // String operations
    make_procedure_in_env(env, "string-length", StringProcedures::StringLength, 1);
    make_procedure_in_env(env, "string-ref", StringProcedures::StringRef, 2);
    // Vector Operations
    make_procedure_in_env(env, "make-vector", VectorProcedures::make_vec, 2);
    make_procedure_in_env(env, "vector-ref", VectorProcedures::vec_ref, 2);
    make_procedure_in_env(env, "vector-set!", VectorProcedures::vec_set, 3);
    make_procedure_in_env(env, "vector-length", VectorProcedures::vec_length, 1);

    init_type_predicates(env);
    init_type_conversions(env);
    init_input_output(env);
}

void SchemeObjectCreator::init_input_output(Environment* env) {
    make_procedure_in_env(env, "load", InputProcedures::load, 1);
    make_procedure_in_env(env, "read", InputProcedures::read, 1);
    make_procedure_in_env(env, "read-char", InputProcedures::read_char, 1);
    make_procedure_in_env(env, "peek-char", InputProcedures::peek_char, 1);

    make_procedure_in_env(env, "write", OutputProcedures::write, 2);
    make_procedure_in_env(env, "write-char", OutputProcedures::write_char, 2);

    make_procedure_in_env(env, "open-input-file",
            [](SchemeObject* args, SchemeObjectCreator* creator) {
                return creator->make_input_port(args->car()->to_string()->value());
            }, 0);
    make_procedure_in_env(env, "open-output-file",
            [](SchemeObject* args, SchemeObjectCreator* creator) {
                return creator->make_output_port(args->car()->to_string()->value());
            }, 0);
    make_procedure_in_env(env, "close-input-port",
            [](SchemeObject* args, SchemeObjectCreator* creator) {
                args->car()->to_input_port()->close_file();
                return creator->make_symbol("ok");
            }, 1);
    make_procedure_in_env(env, "close-output-port",
            [](SchemeObject* args, SchemeObjectCreator* creator) {
                args->car()->to_output_port()->close_file();
                return creator->make_symbol("ok");
            }, 1);
}

void SchemeObjectCreator::init_type_predicates(Environment* env) {

    make_procedure_in_env(env, "eq?", PredicateProcedures::obj_equiv);
    make_procedure_in_env(env, "not", PredicateProcedures::boolean_not, 1);

    // Single type checks
    auto null_check = 
        PredicateProcedures::create_type_check(SchemeObject::EMPTY_LIST);
    auto fixnum_check = 
        PredicateProcedures::create_type_check(SchemeObject::FIXNUM);
    auto flonum_check = 
        PredicateProcedures::create_type_check(SchemeObject::FLONUM);
    auto boolean_check = 
        PredicateProcedures::create_type_check(SchemeObject::BOOLEAN);
    auto char_check = 
        PredicateProcedures::create_type_check(SchemeObject::CHARACTER);
    auto string_check = 
        PredicateProcedures::create_type_check(SchemeObject::STRING);
    auto pair_check = 
        PredicateProcedures::create_type_check(SchemeObject::PAIR);
    auto symbol_check = 
        PredicateProcedures::create_type_check(SchemeObject::SYMBOL);
    auto vector_check = 
        PredicateProcedures::create_type_check(SchemeObject::VECTOR);
    auto input_check = 
        PredicateProcedures::create_type_check(SchemeObject::INPUT_PORT);
    auto output_check = 
        PredicateProcedures::create_type_check(SchemeObject::OUTPUT_PORT);
    auto eof_check =
        PredicateProcedures::create_type_check(SchemeObject::EOF_OBJECT);
    
    // Compound checks
    auto procedure_check = PredicateProcedures::create_type_check(
                SchemeObject::PRIMPROCEDURE | SchemeObject::COMPPROCEDURE
                );
    auto io_check = PredicateProcedures::create_type_check(
            SchemeObject::OUTPUT_PORT | SchemeObject::INPUT_PORT
            );

    // Binding
    make_procedure_in_env(env, "null?", null_check, 1);
    make_procedure_in_env(env, "integer?", fixnum_check, 1);
    make_procedure_in_env(env, "real?", flonum_check, 1);
    make_procedure_in_env(env, "boolean?", boolean_check, 1);
    make_procedure_in_env(env, "char?", char_check, 1);
    make_procedure_in_env(env, "string?", string_check, 1);
    make_procedure_in_env(env, "pair?", pair_check, 1);
    make_procedure_in_env(env, "symbol?", symbol_check, 1);
    make_procedure_in_env(env, "vector?", vector_check, 1);
    make_procedure_in_env(env, "procedure?", procedure_check, 1);
    make_procedure_in_env(env, "output-port?", output_check, 1);
    make_procedure_in_env(env, "input-port?", input_check, 1);
    make_procedure_in_env(env, "io-port?", io_check, 1);
    make_procedure_in_env(env, "eof-object?", eof_check, 1);
    // Special function since list checks are built-ins for SchemeObject
    make_procedure_in_env(env, "list?", PredicateProcedures::list_check, 1);
}

void SchemeObjectCreator::init_type_conversions(Environment* env) {
    make_procedure_in_env(env, "char->integer", TypeConversions::char_to_int, 1);
    make_procedure_in_env(env, "integer->char", TypeConversions::int_to_char, 1);
    make_procedure_in_env(env, "string->integer", TypeConversions::string_to_int, 1);
    make_procedure_in_env(env, "integer->string", TypeConversions::int_to_string, 1);
    make_procedure_in_env(env, "symbol->string", 
            TypeConversions::symbol_to_string, 1);
    make_procedure_in_env(env, "string->symbol", 
            TypeConversions::string_to_symbol, 1);
    make_procedure_in_env(env, "list->string", StringProcedures::ListToString, 1);
    make_procedure_in_env(env, "vector->list", VectorProcedures::vec_to_list, 1);
}
