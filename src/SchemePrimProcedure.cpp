#include "Procedures/SchemePrimProcedure.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

#include <vector>

bool SchemePrimProcedure::check_arg_length(SchemeObject* args) {
    return false;
}

SchemeObject* SchemeAddProcedure::func(SchemeObject* args) {

    int accum = 0;

    while (!args->is_empty_list()) {
        accum += args->car()->to_fixnum()->value();
        args = args->cdr();
    }
    return obj_creator_->make_fixnum(accum);
}

SchemeObject* SchemeSubProcedure::func(SchemeObject* args) {

    int accum = args->car()->to_fixnum()->value();
    args = args->cdr();

    while (!args->is_empty_list()) {
        accum -= args->car()->to_fixnum()->value();
        args = args->cdr();
    }
    return obj_creator_->make_fixnum(accum);
}

SchemeObject* SchemeMultProcedure::func(SchemeObject* args) {

    int accum = 1;

    while (!args->is_empty_list()) {
        accum *= args->car()->to_fixnum()->value();
        args = args->cdr();
    }
    return obj_creator_->make_fixnum(accum);
}

SchemeObject* SchemeDivProcedure::func(SchemeObject* args) {

    int accum = args->car()->to_fixnum()->value();
    args = args->cdr();

    while (!args->is_empty_list()) {
        accum /= args->car()->to_fixnum()->value();
        args = args->cdr();
    }
    return obj_creator_->make_fixnum(accum);
}

SchemeObject* SchemeLessThanProcedure::func(SchemeObject* args) {

    bool retval = true;
    int value = args->car()->to_fixnum()->value();
    int nextval;

    args = args->cdr();

    while (!args->is_empty_list()) {
        nextval = args->car()->to_fixnum()->value();
        retval = value < nextval;

        if (!retval) {
            break;
        }
        value = nextval;
        args = args->cdr();
    }
    return obj_creator_->make_boolean(retval);
}

SchemeObject* SchemeGreaterThanProcedure::func(SchemeObject* args) {

    bool retval = true;
    int value = args->car()->to_fixnum()->value();
    int nextval;

    args = args->cdr();

    while (!args->is_empty_list()) {
        nextval = args->car()->to_fixnum()->value();
        retval = value > nextval;

        if (!retval) {
            break;
        }
        value = nextval;
        args = args->cdr();
    }
    return obj_creator_->make_boolean(retval);
}

SchemeObject* SchemeEqualsProcedure::func(SchemeObject* args) {

    bool retval = true;
    int value = args->car()->to_fixnum()->value();
    args = args->cdr();

    while (!args->is_empty_list()) {
        retval = retval && (value == args->car()->to_fixnum()->value());
        if (!retval) {
            break;
        }
        args = args->cdr();
    }
    return obj_creator_->make_boolean(retval);
}

SchemeObject* SchemeQuotientProcedure::func(SchemeObject* args) {

    int m = args->car()->to_fixnum()->value();
    int n = args->cadr()->to_fixnum()->value();

    return obj_creator_->make_fixnum(m / n);
}

SchemeObject* SchemeModuloProcedure::func(SchemeObject* args) {

    int m = args->car()->to_fixnum()->value();
    int n = args->cadr()->to_fixnum()->value();

    return obj_creator_->make_fixnum(m % n);
}

SchemeObject* SchemePredicateProcedure::func(SchemeObject* args) {
    return obj_creator_->make_boolean(args->car()->type() & target_type_);
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
