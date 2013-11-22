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

#include "Scheme.h"
#include "Reader.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"
#include "Environment.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>

Scheme::Scheme(std::istream& instream) : 
    cursor_(">>>"),
    obj_creator_(), 
    reader_(&obj_creator_, instream)
{ 
    // Populate environment with bindings to built-ins
    the_global_environment_ = std::make_shared<Environment>();
    obj_creator_.setup_environment(the_global_environment_.get());
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

SchemeObject* Scheme::eval(SchemeObject* exp, Environment::Ptr env){
    assert(the_global_environment_);

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

    } else if (exp->is_tagged_list("lambda")) {
   
        return obj_creator_.make_comp_procedure(
                env,
                exp->cadr(),
                exp->cddr());

    } else if (exp->is_tagged_list("if")) {
        if (exp->length_as_list() == 4) {

            exp = exp->cdr();

            if (exp->car()->is_true_obj()) {
                exp = exp->cadr();
            } else {
                exp = exp->caddr();
            }
            goto tailcall;

        } else {
            std::cerr << "Error: cannot evaluate if form" << std::endl;
            exit(1);
        }
    } else if (exp->is_tagged_list("quote")){
        return exp->cadr();
    } else if (exp->is_tagged_list("define")){
        if (exp->length_as_list() == 3){
            env->define_variable_value(
                    exp->cadr()->to_symbol(), 
                    eval(exp->caddr(), env));
            return obj_creator_.make_symbol("ok");
        } else {
            std::cerr << "Error: cannot evaluate def form" << std::endl;
            exit(1);
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

        if (proc->is_prim_procedure()) {

            return proc->to_prim_procedure()->func(args);

        } else if (proc->is_comp_procedure()) {

            SchemeCompoundProcedure* comp = proc->to_comp_procedure();
            env = env->extend(comp->params(), args);

            exp = comp->body();
            while (exp->length_as_list() > 1) {
                eval(exp->car(), env);
                exp = exp->cdr();
            }
            exp = exp->car();
            goto tailcall;

        } else {
            std::cerr << "Error: Cannot apply object." << std::endl;
            exit(1);
        }
    } else { 
        std::cerr << "Error: cannot evaluate unknown expression type." << std::endl;
        exit(1);
    }
    
    std::cerr << "Error: eval illegal state." << std::endl;
    exit(1);
}

SchemeObject* Scheme::get_value_of_args(SchemeObject* args, Environment::Ptr& env) {
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

//============================================================================
// Print
//============================================================================

void Scheme::write(SchemeObject* obj){
	switch (obj->type()){
		case SchemeObject::FIXNUM:
			std::cout << obj->to_fixnum()->value();
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
            write_pair(obj->to_pair());
            std::cout << ")";
            break;
        case SchemeObject::EMPTY_LIST:
            std::cout << "()";
            break;
        case SchemeObject::SYMBOL:
            std::cout << obj->to_symbol()->value();
            break;
        case SchemeObject::PRIMPROCEDURE:
            std::cout << "#<primitive-procedure>";
            break;
        case SchemeObject::COMPPROCEDURE:
            std::cout << "#<compound-procedure>";
            break;
		default:
			std::cerr << "unknown type, cannot write." << std::endl;
			exit(1);
	}
}

void Scheme::write_pair(SchemePair* pair){
    SchemeObject* car_obj = pair->car();
    SchemeObject* cdr_obj = pair->cdr();

    write(car_obj);

    if (cdr_obj->is_pair()){
        std::cout << " ";
        write_pair(cdr_obj->to_pair());
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

void Scheme::main_loop(){
	print_welcome_message();
	while (true) {
		std::cout << cursor_ << ' ';
		write(eval(reader_.read(), the_global_environment_));
		std::cout << std::endl;
	}
    print_goodbye_message();
}
