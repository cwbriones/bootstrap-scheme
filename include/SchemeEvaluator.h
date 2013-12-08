#ifndef SCHEME_EVALUATOR_H_
#define SCHEME_EVALUATOR_H_

#include "Environment.h"

class SchemeObject;
class SchemeCreator;

class SchemeEvaluator {
public:
    SchemeEvaluator(SchemeObjectCreator* creator) :
        obj_creator_(creator) {}

    SchemeObject* eval(SchemeObject* exp, Environment::Ptr env);
    SchemeObject* eval_in_global_env(SchemeObject* exp);
private:
    std::string make_string(
            const std::string& prefix, size_t suffix, size_t maxlen);

    SchemeObject* cons(SchemeObject* car, SchemeObject* cdr);
    SchemeObject* get_value_of_args(SchemeObject* args, Environment::Ptr env);
    
    SchemeObject* eval_let_form(SchemeObject* args, Environment::Ptr env);
    SchemeObject* convert_let_star(SchemeObject* args, Environment::Ptr env);
    SchemeObject* convert_named_let(SchemeObject* args, Environment::Ptr env);
    SchemeObject* convert_letrec(SchemeObject* args);

    SchemeObject* prepare_apply_args(SchemeObject* args_to_apply);

    SchemeObjectCreator* obj_creator_;
};

#endif /* SCHEME_EVALUATOR_H_ */
