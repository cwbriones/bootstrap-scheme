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
    SchemeObject* obj = new SchemeObject(SchemeObject::FIXNUM);
    obj->init_fixnum(value);

    SchemeGarbageCollector::the_gc().add(obj);
    return obj;
}

SchemeObject* SchemeObjectCreator::make_flonum(double value) {
    SchemeObject* obj = new SchemeObject(SchemeObject::FLONUM);
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
    SchemeObject* obj = new SchemeObject(SchemeObject::CHARACTER);
    obj->init_char(value);

    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_string(std::string value) {
    SchemeObject* obj = new SchemeString(SchemeObject::STRING, value);
    SchemeGarbageCollector::the_gc().add(obj);

    return obj;
}

SchemeObject* SchemeObjectCreator::make_pair(SchemeObject* car, SchemeObject* cdr) {
    SchemeObject* obj = new SchemeObject(SchemeObject::PAIR);
    SchemeGarbageCollector::the_gc().add(obj);

    obj->set_car(car);
    obj->set_cdr(cdr);

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
    // Arithmetic Operators
    // Note that by defining these in the environment
    // they are accessible to the garbage collector since it adds them
    // with the other bindings
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
    // List operations
    env->define_variable_value(
            make_symbol("length")->to_symbol(),
            new SchemeLengthProcedure(this)
        );
    env->define_variable_value(
            make_symbol("cons")->to_symbol(),
            new SchemeConsProcedure()
        );
    env->define_variable_value(
            make_symbol("car")->to_symbol(),
            new SchemeCarProcedure()
        );
    env->define_variable_value(
            make_symbol("set-car!")->to_symbol(),
            new SchemeSetCarProcedure()
        );
    env->define_variable_value(
            make_symbol("cdr")->to_symbol(),
            new SchemeCdrProcedure()
        );
    env->define_variable_value(
            make_symbol("set-cdr!")->to_symbol(),
            new SchemeSetCdrProcedure()
        );
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
            make_symbol("flonum?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::FLONUM)
        );
    env->define_variable_value(
            make_symbol("procedure?")->to_symbol(),
            new SchemePredicateProcedure(this, 
                SchemeObject::PRIMPROCEDURE | SchemeObject::COMPPROCEDURE)
        );
    env->define_variable_value(
            make_symbol("vector?")->to_symbol(),
            new SchemePredicateProcedure(this, SchemeObject::VECTOR)
        );
    env->define_variable_value(
            make_symbol("eq?")->to_symbol(),
            new SchemePolyEqProcedure(this)
        );
    env->define_variable_value(
            make_symbol("not")->to_symbol(),
            new SchemeNotProcedure(this)
        );
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
