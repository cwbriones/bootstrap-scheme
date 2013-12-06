#include "Environment.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"
#include "Procedures/TypeConversions.h"
#include "Procedures/EnvironmentProcedures.h"
#include "Procedures/StringProcedures.h"
#include "Procedures/VectorProcedures.h"
#include "InputOutput.h"

#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

#include "SchemeEnvironment.h"
#include "SchemeGarbageCollector.h"

#include "SchemeVector.h"

SchemeObjectCreator::SchemeObjectCreator() :
    the_interaction_env_(Environment::get_global_environment()) 
{
    the_interaction_env_.protect_from_gc();
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
        NewPrimProcedure::procedure_t func,
        int argc)
{
    SchemeObject* obj = new NewPrimProcedure(this, func, argc);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

void SchemeObjectCreator::make_procedure_in_env(
        Environment* env,
        const std::string& var,
        NewPrimProcedure::procedure_t func,
        int argc)
{
    env->define_variable_value(
            make_symbol(var)->to_symbol(), 
            make_prim_procedure(func, argc)
        );
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

SchemeObject* SchemeObjectCreator::make_empty_list() {
    return &SchemeObject::the_empty_list_;
}

SchemeObject* SchemeObjectCreator::make_tagged_list(std::string tag, SchemeObject* obj) {
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
        make_pair(
            obj1,
            make_pair(obj2, make_empty_list())
        )
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

    env->define_variable_value(
            make_symbol("random")->to_symbol(),
            new RandomProcedure()
        );
    // Apply/Eval
    env->define_variable_value(
            make_symbol("apply")->to_symbol(),
            new SchemeApplyProcedure()
        );
    env->define_variable_value(
            make_symbol("eval")->to_symbol(),
            new SchemeEvalProcedure()
        );
    // Environment operations
    env->define_variable_value(
            make_symbol("null-environment")->to_symbol(),
            new NullEnvironmentProcedure(this)
        );
    env->define_variable_value(
            make_symbol("interaction-environment")->to_symbol(),
            new InteractionEnvironmentProcedure(this)
        );
    env->define_variable_value(
            make_symbol("environment")->to_symbol(),
            new StandardEnvironmentProcedure(this)
        );
    // String operations
    env->define_variable_value(
            make_symbol("string-ref")->to_symbol(),
            new StringRefProcedure(this)
        );
    env->define_variable_value(
            make_symbol("string-length")->to_symbol(),
            new StringLengthProcedure(this)
        );
    // Vector Operations
    env->define_variable_value(
            make_symbol("make-vector")->to_symbol(),
            new MakeVectorProcedure(this)
        );
    env->define_variable_value(
            make_symbol("vector-ref")->to_symbol(),
            new VectorRefProcedure(this)
        );
    env->define_variable_value(
            make_symbol("vector-set!")->to_symbol(),
            new VectorSetProcedure(this)
        );

    init_type_predicates(env);
    init_type_conversions(env);
    init_input_output(env);
}

void SchemeObjectCreator::init_input_output(Environment* env) {
    env->define_variable_value(
            make_symbol("load")->to_symbol(),
            new LoadProcedure(this)
        );
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
    
    // Compound checks
    auto procedure_check = PredicateProcedures::create_type_check(
                SchemeObject::NEWPROCEDURE | SchemeObject::COMPPROCEDURE
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
    make_procedure_in_env(env, "list?", PredicateProcedures::list_check, 1);
}

void SchemeObjectCreator::init_type_conversions(Environment* env) {
    env->define_variable_value(
            make_symbol("char->integer")->to_symbol(),
            new SchemeCharToIntProcedure(this)
        );
    env->define_variable_value(
            make_symbol("integer->char")->to_symbol(),
            new SchemeIntToCharProcedure(this)
        );
    env->define_variable_value(
            make_symbol("string->int")->to_symbol(),
            new SchemeStringToIntProcedure(this)
        );
    env->define_variable_value(
            make_symbol("int->string")->to_symbol(),
            new SchemeIntToStringProcedure(this)
        );
    env->define_variable_value(
            make_symbol("string->symbol")->to_symbol(),
            new SchemeStringToSymbolProcedure(this)
        );
    env->define_variable_value(
            make_symbol("symbol->string")->to_symbol(),
            new SchemeSymbolToStringProcedure(this)
        );
    env->define_variable_value(
            make_symbol("list->string")->to_symbol(),
            new ListStringProcedure(this)
        );
    env->define_variable_value(
            make_symbol("vector->list")->to_symbol(),
            new VectorToListProcedure(this)
        );
    env->define_variable_value(
            make_symbol("vector-length")->to_symbol(),
            new VectorLengthProcedure(this)
        );
}
