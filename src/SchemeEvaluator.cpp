#include <iostream>
#include <sstream>
#include <iomanip>

#include "SchemeEvaluator.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"
#include "SchemeVector.h"
#include "SchemeEnvironment.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"

std::string SchemeEvaluator::make_string(
        const std::string& prefix, 
        size_t suffix, 
        size_t maxlen)
{
    std::ostringstream result;

    result << prefix 
           << std::setfill('0') 
           << std::setw(maxlen - prefix.length())
           << suffix;

    return result.str();
}

SchemeObject* SchemeEvaluator::cons(SchemeObject* car, SchemeObject* cdr) {
    return obj_creator_->make_pair(car, cdr);
}

SchemeObject* SchemeEvaluator::eval_in_global_env(SchemeObject* exp) {
    return eval(exp, Environment::get_global_environment());
}

SchemeObject* SchemeEvaluator::eval(SchemeObject* exp, Environment::Ptr env) {
    tailcall:

    if (exp->is_self_evaluating()){

        return exp;

    } else if (exp->is_symbol()){

        SchemeObject* value = env->lookup_variable_value(exp->to_symbol());

        if (!value){
            std::cerr << "Error: unbound variable \'" << exp->to_symbol()->value()
                      << "\'." << std::endl;
            exit(1);
        }

        return value;

    } else if (exp->is_tagged_list("and")) {

        SchemeObject* result = obj_creator_->make_boolean(true);
        exp = exp->cdr();
        if (exp->is_empty_list()) {
            return result;
        }

        while (exp->length_as_list() > 1) {
            result = eval(exp->car(), env);
            if (result->is_false_obj()) {
                return result;
            }
            exp = exp->cdr();
        }
        exp = exp->car();
        goto tailcall;

    } else if (exp->is_tagged_list("or")) {

        SchemeObject* result = obj_creator_->make_boolean(false);
        exp = exp->cdr();
        if (exp->is_empty_list()) {
            return result;
        }

        while (exp->length_as_list() > 1) {
            result = eval(exp->car(), env);
            if (result->is_true()) {
                return result;
            }
            exp = exp->cdr();
        }
        exp = exp->car();
        goto tailcall;

    } else if (exp->is_tagged_list("begin")) {

        // Point expression to the body of the begin form
        exp = exp->cdr();
        if (exp->is_empty_list()) {
            return obj_creator_->make_unspecified();
        }

        while (exp->length_as_list() > 1) {
            eval(exp->car(), env);
            exp = exp->cdr();
        }
        exp = exp->car();
        goto tailcall;

    } else if (exp->is_tagged_list("lambda")) {
   
        return obj_creator_->make_comp_procedure(
                env,
                exp->cadr(),
                exp->cddr());

    } else if (exp->is_tagged_list("let")) {

        exp = eval_let_form(exp->cdr(), env);
        return eval(exp, env);

    } else if (exp->is_tagged_list("letrec")) {

        exp = convert_letrec_form(exp->cdr());
        return eval(exp, env);

    } else if (exp->is_tagged_list("if")) {

        if (exp->length_as_list() == 4 || exp->length_as_list() == 3) {
            exp = exp->cdr();

            if (eval(exp->car(), env)->is_false_obj()) {
                exp = exp->caddr();
                if (!exp) {
                    return obj_creator_->make_unspecified();
                }
            } else {
                exp = exp->cadr();
            }
            goto tailcall;

        } else {
            std::cerr << "Error: cannot evaluate if form" << std::endl;
            exit(1);
        }

    } else if (exp->is_tagged_list("cond")) {

        SchemeObject* the_begin_form;
        exp = exp->cdr();

        while (!exp->is_empty_list()) {
            if (exp->car()->is_tagged_list("else")) {
                // Make a begin form and assert that this is valid
                // placement for an if clause
                if (exp->length_as_list() > 1) {
                    std::cerr << "Error: misplaced ELSE clause";
                    exit(1);
                }
                the_begin_form = cons(
                        obj_creator_->make_symbol("begin"), 
                        exp->cdar());
                return eval(the_begin_form, env);
            }
            else if (eval(exp->caar(), env)->is_true()) {
                // Make a begin form out of the body
                the_begin_form = cons(
                        obj_creator_->make_symbol("begin"), 
                        exp->cdar());
                return eval(the_begin_form, env);
            }
            exp = exp->cdr();
        }

        return obj_creator_->make_unspecified();

    } else if (exp->is_tagged_list("quote")){

        return exp->cadr();

    } else if (exp->is_tagged_list("define")){
        if (exp->cadr()->is_pair()) {
            SchemeObject* the_lambda = cons(
                    obj_creator_->make_symbol("lambda"),
                    cons(exp->cdadr(), exp->cddr()));
            SchemeObject* var = exp->caadr();

            return eval(obj_creator_->make_tagged_list("define", var, the_lambda), env);

        } else {
            env->define_variable_value(
                    exp->cadr()->to_symbol(), 
                    eval(exp->caddr(), env)
                );
            return obj_creator_->make_symbol("ok");
        }
    } else if (exp->is_tagged_list("set!")){
        if (exp->length_as_list() == 3){

            if (env->set_variable_value(
                        exp->cadr()->to_symbol(), 
                        eval(exp->caddr(), env))){

                return obj_creator_->make_symbol("ok");

            } else {
                std::cerr << "Error: unbound variable " 
                          << exp->cadr()->to_symbol()->value()
                          << "." << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "Error: cannot evaluate set! form" << std::endl;
            exit(1);
        }
    } else if (exp->is_application()) {

        SchemeObject* proc = eval(exp->car(), env);
        SchemeObject* args = get_value_of_args(exp->cdr(), env);

        if (proc->is_prim_procedure() &&
            proc->to_prim_procedure()->is_apply()) {

            proc = args->car();
            args = prepare_apply_args(args->cdr());
        }

        if (proc->is_prim_procedure()) {

            SchemePrimProcedure* prim = proc->to_prim_procedure();
            
            if (prim->is_eval()) {
                SchemeObject* the_eval_env = args->cadr();
                return eval(args->car(), the_eval_env->to_environment()->get());
            }
            return prim->apply(args);
        
        } else if (proc->is_comp_procedure()) {

            SchemeCompoundProcedure* comp = proc->to_comp_procedure();
            env = std::make_shared<Environment>(comp->env(), comp->params(), args);

            return eval(cons(obj_creator_->make_symbol("begin"), comp->body()), env);

        } else {
            std::cerr << "Error: Cannot apply object ";
            exit(1);
        }
    } else { 
        std::cerr << "Error: cannot evaluate unknown expression type." << std::endl;
        exit(1);
    }
    
    std::cerr << "Error: eval illegal state." << std::endl;
    exit(1);
}

SchemeObject* SchemeEvaluator::get_value_of_args(SchemeObject* args, Environment::Ptr env) {
    if (!args->is_proper_list()) {
        std::cout << "Error: ill-formed argument list" << std::endl;
        exit(1);
    }

    std::vector<SchemeObject*> stack;
    stack.reserve(args->length_as_list() + 1);

    while (!args->is_empty_list()) {
        // Evaluate each item and add it to the list
        stack.push_back(eval(args->car(), env));
        args = args->cdr();
    }
    SchemeObject* list = obj_creator_->make_empty_list();
    while (!stack.empty()) {
        list = cons(stack.back(), list);
        stack.pop_back();
    }
    return list;
}

SchemeObject* SchemeEvaluator::eval_let_form(
        SchemeObject* args, 
        Environment::Ptr env) 
{
    if (args->car()->is_symbol()) {
        return convert_named_let_form(args, env);
    }
    SchemeObject* body = args->cdr();
    // point args to the list of (variable, value) pairs
    args = args->car();

    SchemeObject* vars = obj_creator_->make_empty_list();
    SchemeObject* vals = obj_creator_->make_empty_list();

    while (!args->is_empty_list()) {
        vars = cons(args->caar(), vars);
        vals = cons(args->cdar()->car(), vals);
        args = args->cdr();
    }

    SchemeObject* lambda = cons(vars, body);
    lambda = cons(obj_creator_->make_symbol("lambda"), lambda);
    lambda = cons(lambda, vals);

    return lambda;
}

SchemeObject* SchemeEvaluator::convert_named_let_form(
        SchemeObject* args, 
        Environment::Ptr env) 
{
    //Initialize each list to empty
    SchemeObject* lambda_params = obj_creator_->make_empty_list();
    SchemeObject* lambda_args = lambda_params;
    SchemeObject* letrec_vars = lambda_params;

    // Grab each of the relevant pieces
    SchemeObject* lambda_name = args->car();
    SchemeObject* bindings = args->cadr();
    SchemeObject* body = args->cddr();

    // Parse the param/arg pairs
    while (!bindings->is_empty_list()) {
        lambda_params = cons(bindings->caar(), lambda_params);
        lambda_args = cons(bindings->cdar()->car(), lambda_args);
        bindings = bindings->cdr();
    }
    // (<name> <init-arg1> ... <init-arg N>)
    lambda_args = cons(lambda_name, lambda_args);

    // (lambda (params) body1 ... bodyN)
    SchemeObject* the_lambda = 
        cons(obj_creator_->make_symbol("lambda"), cons(lambda_params, body));
    // (<the-lambda>)
    letrec_vars = cons(the_lambda, letrec_vars);
    // (<name> <the-lambda>)
    letrec_vars = cons(lambda_name, letrec_vars);
    // ((<name> <the-lambda>)
    letrec_vars = cons(letrec_vars, obj_creator_->make_empty_list());

    return obj_creator_->make_tagged_list("letrec", letrec_vars, lambda_args);
}

SchemeObject* SchemeEvaluator::convert_letrec_form(SchemeObject* args) {
    /*
     * (letrec ([x1 e1] ... [xn en]) body)
     *      ---->
     * (let ([x1 undef] .. [xn undef])
     *     (let ([t1 e1] ...[t2 e2])
     *          (set! x1 t1) ... (set! xn tn))
     *     body)
     */
    SchemeObject* vars = args->car();
    SchemeObject *var, *val, *inner_let, *tmp_var, *tmp_binding, *set_form;

    SchemeObject* new_args = obj_creator_->make_empty_list();
    inner_let = new_args;
    
    int var_id = 0;

    while (!vars->is_empty_list()) {
        var = vars->caar();
        val = vars->cdar()->car();

        tmp_var = obj_creator_->make_symbol(make_string("t", var_id, 3));
        // Replace the value with unspecified
        vars->cdar()->set_car(obj_creator_->make_unspecified());
        // Make a set form with the temp
        set_form = obj_creator_->make_tagged_list("set!", var, tmp_var);
        // make a new binding for inner let
        tmp_binding = cons(tmp_var, cons(val, obj_creator_->make_empty_list()));

        // Add each to respective lists
        inner_let = cons(set_form, inner_let);
        new_args = cons(tmp_binding, new_args);

        ++var_id;
        vars = vars->cdr();
    }
    // Complete inner let
    inner_let = cons(obj_creator_->make_symbol("let"), cons(new_args, inner_let));
    // Complete outer let
    args->set_cdr(cons(inner_let, args->cdr()));
    args = cons(obj_creator_->make_symbol("let"), args);

    return args;
}

SchemeObject* SchemeEvaluator::prepare_apply_args(SchemeObject* args_to_apply) {
    // TODO: Error checking to ensure the values end with a list
    
    SchemeObject* the_args = args_to_apply;

    if (the_args->length_as_list() == 1 &&
        the_args->car()->is_proper_list())
    {
        // only arg is a list, just return it
        return the_args->car();
    }

    // Get to the last argument
    while (!args_to_apply->cadr()->is_proper_list()) {
        args_to_apply = args_to_apply->cdr();
    }
    // Make one big list
    args_to_apply->set_cdr(args_to_apply->cadr());

    return the_args;
}
