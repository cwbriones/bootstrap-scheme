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
#include <cassert>

#include "Scheme.h"
#include "SchemeObject.h"
#include "SchemeGarbageCollector.h"

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
		writer_.write(evaluator_.eval_in_global_env(reader_.read()));
		std::cout << std::endl;

        SchemeGarbageCollector::the_gc().add_from_environment(env);
        SchemeGarbageCollector::the_gc().collect();
	}
    SchemeGarbageCollector::the_gc().free_remaining();
    print_goodbye_message();
}
