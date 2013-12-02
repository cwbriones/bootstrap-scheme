/*
 * Boostrap Scheme - a quick and dirty Scheme interpreter
 * Copyright (c) 2013 Christian Briones
 * 
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License version 3 along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>

#include "Scheme.h"
#include "Reader.h"
#include "SchemeObject.h"
#include "SchemeVector.h"
#include "SchemeEnvironment.h"
#include "SchemeObjectCreator.h"
#include "SchemeGarbageCollector.h"

#include "Environment.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"

std::string make_string(const std::string& prefix, size_t suffix, size_t maxlen) {
    std::ostringstream result;

    result << prefix 
           << std::setfill('0') 
           << std::setw(maxlen - prefix.length())
           << suffix;

    return result.str();
}

Scheme::Scheme(std::istream& instream) : 
    cursor_(">>>"),
    reader_(&obj_creator_, instream)
{
    reader_.set_evaluator(this);
    obj_creator_.setup_environment(Environment::get_global_environment().get());
}

void Scheme::print_welcome_message() {
	std::cout << "Welcome to Bootstrap Scheme. " << std::endl;
	std::cout << "Use ctrl-c to exit." << std::endl;
}

void Scheme::print_goodbye_message() {
    std::cout << "Happy Happy Joy Joy." << std::endl;
}

//============================================================================
// Evaluate
//============================================================================

SchemeObject* Scheme::cons(SchemeObject* car, SchemeObject* cdr) {
    return obj_creator_.make_pair(car, cdr);
}

SchemeObject* Scheme::eval_in_global_env(SchemeObject* exp) {
    return eval(exp, Environment::get_global_environment());
}

SchemeObject* Scheme::eval(SchemeObject* exp, Environment::Ptr env) {
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

        exp = exp->cdr();
        SchemeObject* result = obj_creator_.make_boolean(true);

        while (!exp->is_empty_list()) {
            result = eval(exp->car(), env);
            if (result->is_false_obj()) {
                return result;
            }
            exp = exp->cdr();
        }
        return result;

    } else if (exp->is_tagged_list("or")) {

        exp = exp->cdr();
        SchemeObject* result = obj_creator_.make_boolean(false);

        while (!exp->is_empty_list()) {
            result = eval(exp->car(), env);
            if (result->is_true()) {
                return result;
            }
            exp = exp->cdr();
        }
        return result;

    } else if (exp->is_tagged_list("begin")) {

        // Point expression to the body of the begin form
        exp = exp->cdr();

        while (exp->length_as_list() > 1) {
            eval(exp->car(), env);
            exp = exp->cdr();
        }
        exp = exp->car();
        goto tailcall;

    } else if (exp->is_tagged_list("lambda")) {
   
        return obj_creator_.make_comp_procedure(
                env,
                exp->cadr(),
                exp->cddr());

    } else if (exp->is_tagged_list("let")) {

        return eval_let_form(exp->cdr(), env);

    } else if (exp->is_tagged_list("letrec")) {

        exp = convert_letrec_form(exp->cdr());
        return eval(exp, env);

    } else if (exp->is_tagged_list("if")) {

        if (exp->length_as_list() == 4 || exp->length_as_list() == 3) {
            exp = exp->cdr();

            if (eval(exp->car(), env)->is_false_obj()) {
                exp = exp->caddr();
                if (!exp) {
                    return obj_creator_.make_unspecified();
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
                    std::cerr << "Error: misplaced ELSE clause:";
                    write(exp->car());
                    std::cerr << std::endl;
                    exit(1);
                }
                the_begin_form = cons(
                        obj_creator_.make_symbol("begin"), 
                        exp->cdar());
                return eval(the_begin_form, env);
            }
            else if (eval(exp->caar(), env)->is_true()) {
                // Make a begin form out of the body
                the_begin_form = cons(
                        obj_creator_.make_symbol("begin"), 
                        exp->cdar());
                return eval(the_begin_form, env);
            }
            exp = exp->cdr();
        }

        return obj_creator_.make_unspecified();

    } else if (exp->is_tagged_list("quote")){

        return exp->cadr();

    } else if (exp->is_tagged_list("define")){
        if (exp->cadr()->is_pair()) {
            SchemeObject* the_lambda = cons(
                    obj_creator_.make_symbol("lambda"),
                    cons(exp->cdadr(), exp->cddr()));
            SchemeObject* var = exp->caadr();

            return eval(obj_creator_.make_tagged_list("define", var, the_lambda), env);

        } else {
            env->define_variable_value(
                    exp->cadr()->to_symbol(), 
                    eval(exp->caddr(), env)
                );
            return obj_creator_.make_symbol("ok");
        }
    } else if (exp->is_tagged_list("set!")){
        if (exp->length_as_list() == 3){

            if (env->set_variable_value(
                        exp->cadr()->to_symbol(), 
                        eval(exp->caddr(), env))){

                return obj_creator_.make_symbol("ok");

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
            return prim->func(args);

        } else if (proc->is_comp_procedure()) {

            SchemeCompoundProcedure* comp = proc->to_comp_procedure();
            env = std::make_shared<Environment>(comp->env(), comp->params(), args);

            return eval(cons(obj_creator_.make_symbol("begin"), comp->body()), env);

        } else {
            std::cerr << "Error: Cannot apply object ";
            write(proc);
            std::cerr << std::endl;
            exit(1);
        }
    } else { 
        std::cerr << "Error: cannot evaluate unknown expression type." << std::endl;
        exit(1);
    }
    
    std::cerr << "Error: eval illegal state." << std::endl;
    exit(1);
}

SchemeObject* Scheme::get_value_of_args(SchemeObject* args, Environment::Ptr env) {
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
    SchemeObject* list = obj_creator_.make_empty_list();
    while (!stack.empty()) {
        list = cons(stack.back(), list);
        stack.pop_back();
    }
    return list;
}

SchemeObject* Scheme::eval_let_form(
        SchemeObject* args, 
        Environment::Ptr env) 
{
    SchemeObject* body = args->cdr();
    args = args->car();
    // Args now points to the list of (variable, value) pairs

    SchemeObject* vars = obj_creator_.make_empty_list();
    SchemeObject* vals = obj_creator_.make_empty_list();

    SchemeObject* pair;

    while (!args->is_empty_list()) {
        pair = args->car();
        vars = cons(pair->car(), vars);
        vals = cons(pair->cadr(), vals);
        args = args->cdr();
    }

    SchemeObject* lambda = cons(vars, body);
    lambda = cons(obj_creator_.make_symbol("lambda"), lambda);
    lambda = cons(lambda, vals);

    return eval(lambda, env);
}

SchemeObject* Scheme::convert_letrec_form(SchemeObject* args) {
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

    SchemeObject* new_args = obj_creator_.make_empty_list();
    inner_let = new_args;
    
    int var_id = 0;

    while (!vars->is_empty_list()) {
        var = vars->caar();
        val = vars->cdar()->car();

        tmp_var = obj_creator_.make_symbol(
                make_string("t", var_id, 3));
        // Replace the value with unspecified
        vars->cdar()->set_car(obj_creator_.make_unspecified());
        // Make a set form with the temp
        set_form = obj_creator_.make_tagged_list("set!", var, tmp_var);
        // make a new binding for inner let
        tmp_binding = cons(tmp_var, cons(val, obj_creator_.make_empty_list()));

        // Add the set form to the list
        inner_let = cons(set_form, inner_let);
        // Add the tmp binding to the list
        new_args = cons(tmp_binding, new_args);

        ++var_id;
        vars = vars->cdr();
    }
    // Complete the inner let
    inner_let = cons(obj_creator_.make_symbol("let"), cons(new_args, inner_let));
    args->set_cdr(cons(inner_let, args->cdr()));
    args = cons(obj_creator_.make_symbol("let"), args);

    return args;
}

SchemeObject* Scheme::prepare_apply_args(SchemeObject* args_to_apply) {
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

//============================================================================
// Print
//============================================================================

void Scheme::write(SchemeObject* obj){
	switch (obj->type()){
        case SchemeObject::UNSPECIFIED:
            std::cout << "#<unspecified>";
            break;
		case SchemeObject::FIXNUM:
			std::cout << obj->to_fixnum()->value();
			break;
        case SchemeObject::FLONUM:
            std::cout <<  obj->to_flonum()->value();
            break;
        case SchemeObject::BOOLEAN:
            std::cout << '#' << ((obj->is_true_obj()) ? 't' : 'f');
            break;
        case SchemeObject::CHARACTER:
            std::cout << "#\\";

            switch(obj->to_character()->value()){
                case '\n':
                    std::cout << "newline";
                    break;
                case ' ':
                    std::cout << "space";
                    break;
                case '\t':
                    std::cout << "tab";
                    break;
                default:
                    std::cout << obj->to_character()->value();
            }
            break;
        case SchemeObject::STRING:
            write_string(obj->to_string()->value());
            break;
        case SchemeObject::PAIR:
            std::cout << "(";
            write_pair(obj);
            std::cout << ")";
            break;
        case SchemeObject::EMPTY_LIST:
            std::cout << "()";
            break;
        case SchemeObject::SYMBOL:
            std::cout << obj->to_symbol()->value();
            break;
        case SchemeObject::PRIMPROCEDURE:
            std::cout << "#<compiled-procedure>";
            break;
        case SchemeObject::COMPPROCEDURE:
            std::cout << "#<compound-procedure>";
            break;
        case SchemeObject::ENVIRONMENT:
            std::cout << "#<environment>";
            break;
        case SchemeObject::VECTOR:
            std::cout << "#(";
            write_vector(obj);
            std::cout << ")";
            break;
		default:
			std::cerr << "unknown type, cannot write." << std::endl;
			exit(1);
	}
}

void Scheme::write_pair(SchemeObject* pair){
    SchemeObject* car_obj = pair->car();
    SchemeObject* cdr_obj = pair->cdr();

    write(car_obj);

    if (cdr_obj->is_pair()){
        std::cout << " ";
        write_pair(cdr_obj);
        return;
    } 
    else if (cdr_obj->is_empty_list()){
        return;
    } 
    else {
        std::cout << " . ";
        write(cdr_obj);
    }
}

void Scheme::write_vector(SchemeObject* obj) {
    std::vector<SchemeObject*>& the_vector = obj->to_vector()->data();

    for (int i = 0; i < the_vector.size() - 1; i++) {
        write(the_vector[i]);
        std::cout << " ";
    }
    write(the_vector.back());
}

void Scheme::write_string(std::string str){
    std::cout << '\"';
    std::string::iterator iter = str.begin();

    while (iter != str.end()){
        switch(*iter){
            case '\n':
                std::cout << "\\n";
                break;
            case '\t':
                std::cout << "\\t";
                break;
            case '\"':
                std::cout << "\\\"";
                break;
            default:
                std::cout << *iter;
        }
        iter++;
    }
    std::cout << '\"';
}

//============================================================================
// REPL
//============================================================================

bool Scheme::load_file(std::string fname) {

    std::ifstream file_stream(fname, std::ifstream::in);
    SchemeReader file_reader(&obj_creator_, file_stream);

    std::cout << "Loading file \'" << fname << "\'."  << std::endl;
    SchemeObject* what_was_read = file_reader.read();

    while (what_was_read) {
        eval(what_was_read, Environment::get_global_environment());
        what_was_read = file_reader.read();
    }
    std::cout << "done." << std::endl;
    file_stream.close();

    Environment* env = Environment::get_global_environment().get();
    SchemeGarbageCollector::the_gc().add_from_environment(env);
    SchemeGarbageCollector::the_gc().collect();

    return true;
}

void Scheme::main_loop(){
    Environment* env = Environment::get_global_environment().get();

	print_welcome_message();
	while (true) {
		std::cout << cursor_ << ' ';
		write(eval_in_global_env(reader_.read()));
		std::cout << std::endl;

        SchemeGarbageCollector::the_gc().add_from_environment(env);
        SchemeGarbageCollector::the_gc().collect();
	}
    SchemeGarbageCollector::the_gc().free_remaining();
    print_goodbye_message();
}
