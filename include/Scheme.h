/*
 * Copyright (C) Christian Briones, 2013
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCHEME_H
#define SCHEME_H 

#include <string>
#include <iostream>
#include <unordered_map>

#include "Environment.h"

#include "SchemeEvaluator.h"
#include "Reader.h"
#include "SchemeWriter.h"

#include "SchemeObjectCreator.h"

class SchemeObject;

class Scheme {
public:
	Scheme(std::istream& instream);

	void main_loop();
    bool load_file(std::string fname);
private:
	const std::string cursor_;
    
    // Model
    SchemeObjectCreator obj_creator_;
    
    // Read
    SchemeReader reader_;
    
    // Evaluate
    SchemeEvaluator evaluator_;

    // Print
    SchemeWriter writer_;

	void write(SchemeObject* obj);
    void write_pair(SchemeObject* obj);
    void write_vector(SchemeObject* obj);
    void write_string(std::string);

    void print_welcome_message();
    void print_goodbye_message();
};


#endif // SCHEME_H
