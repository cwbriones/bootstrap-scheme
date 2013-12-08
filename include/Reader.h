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

#ifndef READER_H
#define READER_H 

#include <string>
#include <iostream>

#include "Environment.h"

class Scheme;

class SchemeObject;
class SchemeObjectCreator;

class SchemeReader {
    public:
        SchemeReader(SchemeObjectCreator* objcreator);
        SchemeReader(SchemeObjectCreator* objcreator, std::istream& instream);

        SchemeObject* read();
        SchemeObject* read_char();
        SchemeObject* peek_char();

        SchemeObject* load_into_environment(Environment::Ptr env);
    private:
        std::istream& instream_ = std::cin;
        SchemeObjectCreator* objcreator_ = nullptr;

        SchemeObject* read_pair();
        SchemeObject* read_string();
        SchemeObject* read_character();
        SchemeObject* read_symbol();
        SchemeObject* read_vector();

        Scheme* evaluator_ = nullptr;

        void eat_whitespace();
        void eat_expected_word(std::string word);
        void peek_expecting_delimiter();
        void print_welcome_message();

        bool is_delimiter(char c);
        bool is_start_of_symbol(char c);
};

#endif
