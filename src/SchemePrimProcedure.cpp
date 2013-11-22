#include "Procedures/SchemePrimProcedure.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

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
    return obj_creator_->make_boolean(args->car()->type() == target_type_);
}

//============================================================================
// List Operations
//============================================================================

SchemeObject* SchemeConsProcedure::func(SchemeObject* args) {
    return obj_creator_->make_pair(args->car(), args->cadr());
}
