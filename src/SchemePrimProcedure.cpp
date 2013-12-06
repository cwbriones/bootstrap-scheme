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

SchemeObject* SchemePredicateProcedure::func(SchemeObject* args) {
    return obj_creator_->make_boolean(args->car()->type() & target_type_);
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

#include <iostream>

SchemeObject* SchemeLengthProcedure::func(SchemeObject* args) {
    if (args->is_proper_list()) {
        return obj_creator_->make_fixnum(args->car()->length_as_list());
    }
    return nullptr;
}

SchemeObject* SchemeConsProcedure::func(SchemeObject* args) {
    args->set_cdr(args->cadr());

    return args;
}

SchemeObject* SchemeCarProcedure::func(SchemeObject* args) {
    return args->caar();
}

SchemeObject* SchemeCdrProcedure::func(SchemeObject* args) {
    return args->cdar();
}

SchemeObject* SchemeSetCarProcedure::func(SchemeObject* args) {
    SchemeObject* the_pair = args->car();;
    the_pair->set_car(args->cadr());

    return obj_creator_->make_symbol("ok");
}

SchemeObject* SchemeSetCdrProcedure::func(SchemeObject* args) {
    SchemeObject* the_pair = args->car();
    the_pair->set_cdr(args->cadr());

    return obj_creator_->make_symbol("ok");
}

//============================================================================
// Polymorphic Equality Testing
//============================================================================

SchemeObject* SchemePolyEqProcedure::func(SchemeObject* args) {
    if (args->car() != args->cadr()) {
        return &the_false_object_;
    }
    return &the_true_object_;
}

//============================================================================
// Boolean Operations
//============================================================================

SchemeObject* SchemeNotProcedure::func(SchemeObject* args) {
    if (args->car()->is_true()) {
        return &the_false_object_;
    }
    return &the_true_object_;
}

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
