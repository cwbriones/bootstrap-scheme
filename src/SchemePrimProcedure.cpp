#include "Procedures/SchemePrimProcedure.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

#include <vector>
#include <ctime>

namespace ArithmeticProcedures {

SchemeObject* add(SchemeObject* args, SchemeObjectCreator* creator) {
    long accum = 0;

    while (!args->is_empty_list()) {
        accum += args->car()->fixnum_value();
        args = args->cdr();
    }
    return creator->make_fixnum(accum);
}

SchemeObject* sub(SchemeObject* args, SchemeObjectCreator* creator) {

    long accum = args->car()->fixnum_value();
    args = args->cdr();

    while (!args->is_empty_list()) {
        accum -= args->car()->fixnum_value();
        args = args->cdr();
    }
    return creator->make_fixnum(accum);
}

SchemeObject* mul(SchemeObject* args, SchemeObjectCreator* creator) {

    long accum = 1;

    while (!args->is_empty_list()) {
        accum *= args->car()->fixnum_value();
        args = args->cdr();
    }
    return creator->make_fixnum(accum);
}

SchemeObject* div(SchemeObject* args, SchemeObjectCreator* creator) {

    long accum = args->car()->fixnum_value();
    args = args->cdr();

    while (!args->is_empty_list()) {
        accum /= args->car()->fixnum_value();
        args = args->cdr();
    }
    return creator->make_fixnum(accum);
}

SchemeObject* less_than(SchemeObject* args, SchemeObjectCreator* creator) {

    bool retval = true;
    long value = args->car()->fixnum_value();
    long nextval;

    args = args->cdr();

    while (!args->is_empty_list()) {
        nextval = args->car()->fixnum_value();
        retval = value < nextval;

        if (!retval) {
            break;
        }
        value = nextval;
        args = args->cdr();
    }
    return creator->make_boolean(retval);
}

SchemeObject* greater_than(SchemeObject* args, SchemeObjectCreator* creator) {

    bool retval = true;
    long value = args->car()->fixnum_value();
    long nextval;

    args = args->cdr();

    while (!args->is_empty_list()) {
        nextval = args->car()->fixnum_value();
        retval = value > nextval;

        if (!retval) {
            break;
        }
        value = nextval;
        args = args->cdr();
    }
    return creator->make_boolean(retval);
}

SchemeObject* equals(SchemeObject* args, SchemeObjectCreator* creator) {

    bool retval = true;
    long value = args->car()->fixnum_value();
    args = args->cdr();

    while (!args->is_empty_list()) {
        retval = retval && (value == args->car()->fixnum_value());
        if (!retval) {
            break;
        }
        args = args->cdr();
    }
    return creator->make_boolean(retval);
}

SchemeObject* quotient(SchemeObject* args, SchemeObjectCreator* creator) {

    long m = args->car()->fixnum_value();
    long n = args->cadr()->fixnum_value();

    return creator->make_fixnum(m / n);
}

SchemeObject* modulo(SchemeObject* args, SchemeObjectCreator* creator) {

    long m = args->car()->fixnum_value();
    long n = args->cadr()->fixnum_value();

    return creator->make_fixnum(m % n);
}

} /* namespace ArithmeticProcedures */

bool SchemePrimProcedure::check_arg_length(SchemeObject* args) {
    return false;
}

RandomProcedure::RandomProcedure() : SchemePrimProcedure(nullptr, 1) {
    generator_.seed(static_cast<unsigned int>(time(0)));
}

SchemeObject* RandomProcedure::func(SchemeObject* args) {
    SchemeObject* top = args->car();

    if (top->is_flonum()) {
        double value = top->flonum_value();
        std::uniform_real_distribution<double> float_dist(0.0, value);
        double result = float_dist(generator_);

        return obj_creator_->make_flonum(result);

    } else if (top->is_fixnum()) {
        long value = top->fixnum_value();
        std::uniform_int_distribution<long> int_dist(0, value);
        long result = int_dist(generator_);

        return obj_creator_->make_fixnum(result);
    }
}

//============================================================================
// List Operations
//============================================================================

namespace ListProcedures {
 
SchemeObject* length(SchemeObject* args, SchemeObjectCreator* creator) {
    if (args->is_proper_list()) {
        return creator->make_fixnum(args->car()->length_as_list());
    }
    return nullptr;
}

SchemeObject* cons(SchemeObject* args, SchemeObjectCreator* creator) {
    args->set_cdr(args->cadr());

    return args;
}

SchemeObject* car(SchemeObject* args, SchemeObjectCreator* creator) {
    return args->caar();
}

SchemeObject* cdr(SchemeObject* args, SchemeObjectCreator* creator) {
    return args->cdar();
}

SchemeObject* set_car(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeObject* the_pair = args->car();;
    the_pair->set_car(args->cadr());

    return creator->make_symbol("ok");
}

SchemeObject* set_cdr(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeObject* the_pair = args->car();
    the_pair->set_cdr(args->cadr());

    return creator->make_symbol("ok");
}   

} /* namespace ListProcedures */

namespace PredicateProcedures {

SchemeObject* list_check(SchemeObject* args, SchemeObjectCreator* creator) {
    return creator->make_boolean(args->car()->is_proper_list());
}

SchemeObject* type_check(
        SchemeObject* args, 
        SchemeObjectCreator* creator,
        uint32_t type)
{
    return creator->make_boolean(args->car()->type() & type);
}
    
NewPrimProcedure::procedure_t create_type_check(uint32_t type) {
    return std::bind(
            type_check,
            std::placeholders::_1,
            std::placeholders::_2,
            type);
}

SchemeObject* obj_equiv(SchemeObject* args, SchemeObjectCreator* creator) {
    return creator->make_boolean(args->car() == args->cadr());
}

SchemeObject* boolean_not(SchemeObject* args, SchemeObjectCreator* creator) {
    return creator->make_boolean(args->car()->is_false_obj());
}

} /* namespace PredicateProcedures */

//============================================================================
// Apply and Eval
//============================================================================

SchemeObject* SchemeApplyProcedure::func(SchemeObject* args) {
    //TODO: This should be an error
    return nullptr;
}

SchemeObject* SchemeEvalProcedure::func(SchemeObject* args) {
    //TODO: This should be an error
    return nullptr;
}
