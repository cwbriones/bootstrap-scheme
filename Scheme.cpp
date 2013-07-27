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

// TODO: Use more C++-style code to read in single character input,
//       like cin >> c; instead of checking against EOF
//       then I'd only need iostream and string

#include "Scheme.h"
#include "Reader.h"
#include "Object.h"
#include "ObjectCreator.h"
#include "Environment.h"

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstring>

Scheme::Scheme(std::istream& instream) : objcreator_(), 
    reader_(&objcreator_, instream), cursor_(">>>"){ 
}

void Scheme::print_welcome_message(){
	std::cout << "Welcome to Bootstrap Scheme. " << std::endl;
	std::cout << "Use ctrl-c to exit." << std::endl;
}

/********************** EVALUATE *****************************/

// For now we just echo until we have lists and symbols
Object* Scheme::eval(Object* exp){
    // Empty list sent here is an error
    if (exp->is_self_evaluating()){
        return exp;
    } 
    // else if (exp->is_tagged_list(quote_symbol)){
    //     return exp->cadr();
    // } 
    else {
        std::cerr << "Error: cannot evaluate unknown expression type." << std::endl;
        exit(1);
    }
    
    std::cerr << "Error: eval illegal state." << std::endl;
    exit(1);
}

Object* Scheme::cons(Object* exp1, Object* exp2){
    return objcreator_.make_pair(exp1, exp2);
}

/*********************** PRINT *******************************/

void Scheme::write(Object* obj){
	switch (obj->type){
		case Object::FIXNUM:
			std::cout << obj->data.fixnum.value;
			break;
        case Object::BOOLEAN:
            std::cout << '#' << ((obj->is_true_obj()) ? 't' : 'f');
            break;
        case Object::CHARACTER:
            std::cout << "#\\";
            switch(obj->data.character.value){
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
                    std::cout << obj->data.character.value;
            }
            break;
        case Object::STRING:
            write_string(obj->data.string.value);
            break;
        case Object::PAIR:
            std::cout << "(";
            write_pair(obj);
            std::cout << ")";
            break;
        case Object::EMPTY_LIST:
            std::cout << "()";
            break;
        case Object::SYMBOL:
            std::cout << obj->data.symbol.value;
            break;
		default:
			std::cerr << "unknown type, cannot write." << std::endl;
			exit(1);
	}
}

void Scheme::write_pair(Object* pair){
    Object* car_obj = pair->data.pair.car;
    Object* cdr_obj = pair->data.pair.cdr;

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

/************************ REPL *******************************/

void Scheme::main_loop(){
	print_welcome_message();
	while (true) {
		std::cout << cursor_ << ' ';
		write(eval( reader_.read() ));
		std::cout << std::endl;
	}
}

