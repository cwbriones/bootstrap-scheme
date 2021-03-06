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

#include <cstdlib>
#include <unordered_set>
#include <string>
#include <vector>

#include "SchemeEvaluator.h"
#include "Reader.h"
#include "Environment.h"
#include "SchemeObjectCreator.h"

SchemeReader::SchemeReader(SchemeObjectCreator* objcreator){
    objcreator_ = objcreator;
}

SchemeReader::SchemeReader(SchemeObjectCreator* objcreator, std::istream& instream) : 
    objcreator_(objcreator), instream_(instream) {
}

SchemeObject* SchemeReader::load_into_environment(Environment::Ptr env) {
    SchemeEvaluator evaluator(objcreator_);

    SchemeObject* obj = read();
    SchemeObject* result = obj;

    while (!obj->is_eof()) {
        result = evaluator.eval(obj, env);
        obj = read();
    }

    return result;
}

void SchemeReader::eat_whitespace() {
    char c;
    std::string tmp;

	while (c = (instream_ >> std::ws).peek()) {
		if (c == ';'){ // Comments are also whitespace
            std::getline(instream_, tmp);
		} else {
            break;
        }
	}
}

void SchemeReader::eat_expected_word(std::string word){
    int c = 0;
    std::string::iterator iter = word.begin();

    while (iter != word.end()) {
        c = instream_.get();
        if (c != *iter) {
            std::cerr << "unexpected character '" << (char)c << '\'' << std::endl;
            exit(1);
        }
        iter++;
    }
}


bool SchemeReader::is_delimiter(char c) {
	return isspace(c) || c == EOF ||
			c == '('  || c == ')' ||
			c == '"'  || c == ';';
}

bool SchemeReader::is_start_of_symbol(char c) {
    return isdigit(c) || isalpha(c) ||
        ( (c != '#') && (c != '\\') &&
          (c != '`') && !is_delimiter(c) &&
          (c != '\'') );
}

void SchemeReader::peek_expecting_delimiter() {
    if (!is_delimiter(instream_.peek())) {
        std::cerr << "character not followed by delimiter." << std::endl;
        exit(1);
    }
}

SchemeObject* SchemeReader::read() {
	char c;
	eat_whitespace();

    if (!instream_.good()) {
        return objcreator_->make_eof();
    }
	c = instream_.get();

    if (c == '\"') {
        return read_string();
    }
    else if (c == '(') {
        return read_pair();
    }
    else if (c == '#') {
        c = instream_.get();

        if (c == '\\') {
            return read_character();
        } else if (c == '(') {
            return read_vector();
        } else {
            // Read in a boolean.
            switch(c) {
                case 't':
                    return objcreator_->make_boolean(true);
                case 'f':
                    return objcreator_->make_boolean(false);
                default:
                    std::cerr << "unknown boolean literal." << std::endl;
                    exit(1);
            }
        } 
    } 
    else if (isdigit(c) || 
            (c == '.' && isdigit(instream_.peek())) ||
            (c == '-' && (isdigit(instream_.peek()) || instream_.peek() == '.'))) {
        
        std::string number("");
        bool is_float = false;
		// Read in a fixnum.
		if (c == '-') {
            number += c;
        } else {
			instream_.unget();
		}

		while (true) {
            c = instream_.get();

            if (isdigit(c)) {
                number += c;
            } else if (c == '.') {
                is_float = true;
                number += '.';
            } else {
                break;
            }
		}

		if (is_delimiter(c)) {
			instream_.unget();
            if (!is_float) {
                return objcreator_->make_fixnum(std::stol(number));
            } else {
                return objcreator_->make_flonum(std::stof(number));
            }
		} 
		else {
			std::cerr << "number not followed by delimiter" << std::endl;
			exit(1);
		}
	} 
    else if (is_start_of_symbol(c)) {
        // Read in a symbol
        instream_.unget();
        return read_symbol();
    }
    else if (c == '\'' || c == '`') {
        return objcreator_->make_tagged_list("quote", read());
    } else {
		std::cerr << "bad input. unexpected character '" << c << "'." << std::endl;
		exit(1);
	}
	std::cerr << "read illegal state." << std::endl;
	exit(1);
}

SchemeObject* SchemeReader::read_pair() {
    SchemeObject* car_obj = nullptr;
    SchemeObject* cdr_obj = nullptr;

    eat_whitespace();
    char c = instream_.peek();
    if (c == ')') {
        instream_.get();
        return objcreator_->make_empty_list();
    }

    car_obj = read();

    eat_whitespace();
    c = instream_.get();
    if (c == '.') {
        // Dot notation, cons cell
        if (!is_delimiter(instream_.peek())) {
            std::cerr << "Unexpected character \"" << c 
                << ". Was expecting delimiter." << std::endl;
            exit(1);
        }

        cdr_obj = read();

        eat_whitespace();
        c = instream_.get();

        if (c != ')') {
            std::cerr << "Unexpected character \"" << c 
                << ". Was expecting closing ')'" << std::endl;
            exit(1);
        }
    } else {
        instream_.unget();
        cdr_obj = read_pair();
    }

    return objcreator_->make_pair(car_obj, cdr_obj);
}

SchemeObject* SchemeReader::read_vector() {

    char c;
    std::vector<SchemeObject*> objects;

    while (true) {
        eat_whitespace();
        c = instream_.peek();
        if (c == ')') {
            instream_.get();
            return objcreator_->make_vector(objects);
        }
        SchemeObject* obj = read();
        objects.push_back(obj);
    }
}

SchemeObject* SchemeReader::read_string() {
    std::string buffer;
    char c = '\0';

    while( (c = instream_.get()) != '\"') {

        if (c == '\\') {
            // Escaped characters
            c = instream_.get();
            switch(c){
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
            }
        }
        if (c == EOF) {
            std::cerr << "non-terminated string literal." << std::endl;
            exit(1);
        }

        buffer += c;
    }
    
    return objcreator_->make_string(buffer);
}

SchemeObject* SchemeReader::read_character() {
    int c;
    c = instream_.get();

    switch(c) {
        case EOF:
            std::cerr << "incomplete character literal." << std::endl;
            exit(1);
        case 's':
            if (instream_.peek() == 'p') {
                eat_expected_word("pace");
                peek_expecting_delimiter();
                return objcreator_->make_character(' ');
            }
            break;
        case 'n':
            if (instream_.peek() == 'e') {
                eat_expected_word("ewline");
                peek_expecting_delimiter();
                return objcreator_->make_character('\n');
            }
            break;
        case 't':
            if (instream_.peek() == 'a') {
                eat_expected_word("ab");
                peek_expecting_delimiter();
                return objcreator_->make_character('\t');
            }
    }
    peek_expecting_delimiter();
    return objcreator_->make_character(c);
}

SchemeObject* SchemeReader::read_char() {
    if (instream_ == std::cin) {
        instream_.get();
    }
    char c = instream_.get();
    if (instream_.eof()) {
        return objcreator_->make_eof();
    } else {
        return objcreator_->make_character(c);
    }
}

SchemeObject* SchemeReader::peek_char() {
    char c = instream_.peek();
    if (c == EOF) {
        return objcreator_->make_eof();
    }
    return objcreator_->make_character(c);
}

SchemeObject* SchemeReader::read_symbol() {
    // Allowable Symbol characters:
    // a-z A-Z + - . * / < = > ! ? : $ % _ & ~ ^ .
    std::string buffer;
    std::unordered_set<char> allowed_chars = { '+', '-', '.', '*', '/', '<', '=', 
        '>', '!', '?', ':', '$', '%', '_', '&', '~', '^', '.' , '#' };
    
    char c;
    while ( (c = instream_.get()) && !is_delimiter(c) ) {
        buffer += c;
    }
    if (is_delimiter(c)){
        instream_.unget();
    }

    // Ensure validity
    // Also note that # is allowed anywhere so long as it is not preceded by %
    int i = 0;
    for (auto c : buffer) {
        if (!isalpha(c) && !isdigit(c) && (allowed_chars.find(c) == allowed_chars.end())) {
            std::cerr << "Unexpected character \'" << c 
                      << "\' not allowed in symbol." << std::endl;
            exit(1);
        }
        else if (c == '#' && buffer[i - 1] == '%') {
        // This check is okay because read_symbol can't be called if the first char is #
            std::cerr << "# cannot follow a % in a symbol." << std::endl;
            exit(1);
        }
        i++;
    }

    return objcreator_->make_symbol(buffer);
}
