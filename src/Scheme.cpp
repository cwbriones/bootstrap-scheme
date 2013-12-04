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

#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>

#include "Scheme.h"
#include "Reader.h"
#include "SchemeEvaluator.h"

#include "SchemeObject.h"
#include "SchemeVector.h"
#include "SchemeEnvironment.h"
#include "SchemeObjectCreator.h"
#include "SchemeGarbageCollector.h"

#include "Environment.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"

Scheme::Scheme(std::istream& instream) : 
    cursor_(">>>"),
    evaluator_(&obj_creator_),
    reader_(&obj_creator_, instream)
{
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
// Print
//============================================================================

void Scheme::write(SchemeObject* obj){
	switch (obj->type()){
        case SchemeObject::UNSPECIFIED:
            std::cout << "#<unspecified>";
            break;
		case SchemeObject::FIXNUM:
			std::cout << obj->fixnum_value();
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
    SchemeObject* obj = file_reader.read();

    while (obj) {
        evaluator_.eval(obj, Environment::get_global_environment());
        obj = file_reader.read();
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
		write(evaluator_.eval_in_global_env(reader_.read()));
		std::cout << std::endl;

        SchemeGarbageCollector::the_gc().add_from_environment(env);
        SchemeGarbageCollector::the_gc().collect();
	}
    SchemeGarbageCollector::the_gc().free_remaining();
    print_goodbye_message();
}
