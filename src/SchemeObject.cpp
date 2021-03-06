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

#include "SchemeObject.h"
#include "SchemeVector.h"
#include "SchemeEnvironment.h"
#include "InputOutput.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"

#include <iostream>

int SchemeObject::objects_created_ = 0;
int SchemeObject::objects_destroyed_ = 0;

void SchemeObject::object_summary() {
    std::cout << "Objects Created: " << objects_created_ << std::endl;
    std::cout << "Objects Destroyed: " << objects_destroyed_ << std::endl;

    std::cout << "Unaccounted for: " << 
        objects_created_ - objects_destroyed_ << std::endl;
}

SchemeObject SchemeObject::the_false_object_ = 
    SchemeObject(SchemeObject::BOOLEAN);

SchemeObject SchemeObject::the_true_object_ =
    SchemeObject(SchemeObject::BOOLEAN);

SchemeObject SchemeObject::the_empty_list_ = SchemeObject(EMPTY_LIST);
SchemeObject SchemeObject::the_unspecified_object_ = SchemeObject(UNSPECIFIED);
SchemeObject SchemeObject::the_eof_object_ = SchemeObject(EOF_OBJECT);

SchemeObject::SchemeObject() : SchemeObject(UNKNOWN) {}

SchemeObject::SchemeObject(Type t) : type_(t) {
    objects_created_++;

    if (type_ == EMPTY_LIST) {
        is_proper_list_ = true;
        length_as_list_ = 0;
    } else if (type_ == PAIR) {
        length_as_list_ = 0;
    }

    if (type_ & (SYMBOL | BOOLEAN | UNSPECIFIED | EMPTY_LIST | EOF_OBJECT)) {
        protect_from_gc();
    }
}

SchemeObject::~SchemeObject() {
    objects_destroyed_++;
}

SchemeObject::Type SchemeObject::type() const {
    return type_;
}

long SchemeObject::fixnum_value() {
    if (type_ == FIXNUM) {
        return data.fixnum.value;
    } else if (type_ == FLONUM) {
        return static_cast<long>(data.flonum.value);
    } else if (type_ == CHARACTER) {
        return static_cast<long>(data.character.value);
    }
    return 0;
}

bool SchemeObject::boolean_value() {
    if (type_ == BOOLEAN) {
        return data.boolean.value;
    }
    return false;
}

char SchemeObject::char_value() {
    if (type_ == CHARACTER) {
        return data.character.value;
    } else if (type_ == FIXNUM) {
        return static_cast<char>(data.fixnum.value);
    }
    return '\0';
}

double SchemeObject::flonum_value() {
    if (type_ == FLONUM) {
        return data.flonum.value;
    } else if (type_ == FIXNUM) {
        return static_cast<double>(data.fixnum.value);
    }
    return 0;
}

SchemeString* SchemeObject::to_string() {
    return static_cast<SchemeString*>(this);
}

SchemeSymbol* SchemeObject::to_symbol() {
    return static_cast<SchemeSymbol*>(this);
}

SchemePrimProcedure* SchemeObject::to_prim_procedure() {
    return static_cast<SchemePrimProcedure*>(this);
}

SchemeCompoundProcedure* SchemeObject::to_comp_procedure() {
    return static_cast<SchemeCompoundProcedure*>(this);
}

SchemeVector* SchemeObject::to_vector() {
    return static_cast<SchemeVector*>(this);
}

SchemeEnvironment* SchemeObject::to_environment() {
    return static_cast<SchemeEnvironment*>(this);
}

SchemeInputPort* SchemeObject::to_input_port() {
    return static_cast<SchemeInputPort*>(this);
}

SchemeOutputPort* SchemeObject::to_output_port() {
    return static_cast<SchemeOutputPort*>(this);
}

int SchemeObject::length_as_list() {
    if (is_proper_list_) {
        return length_as_list_;
    }
    return -1;
}

bool SchemeObject::is_true_obj() {
    return this == &the_true_object_;
}

bool SchemeObject::is_false_obj() {
    return this == &the_false_object_;
}

bool SchemeObject::is_empty_list() {
    return this == &the_empty_list_;
}

bool SchemeObject::is_eof() {
    return this == &the_eof_object_;
}

bool SchemeObject::is_fixnum() {
    return type_ == Type::FIXNUM;
}

bool SchemeObject::is_flonum() {
    return type_ == Type::FLONUM;
}

bool SchemeObject::is_boolean() {
    return type_ == Type::BOOLEAN;
}

bool SchemeObject::is_character() {
    return type_ == Type::CHARACTER;
}

bool SchemeObject::is_string() {
    return type_ == Type::STRING;
}

bool SchemeObject::is_pair() {
    return type_ == Type::PAIR;
}

bool SchemeObject::is_symbol() {
    return type_ == Type::SYMBOL;
}

bool SchemeObject::is_prim_procedure() {
    return type_ == Type::PRIMPROCEDURE;
}

bool SchemeObject::is_comp_procedure() {
    return type_ == Type::COMPPROCEDURE;
}

bool SchemeObject::is_input_port() {
    return type_ == Type::INPUT_PORT;
}

bool SchemeObject::is_output_port() {
    return type_ == Type::OUTPUT_PORT;
}

bool SchemeObject::is_proper_list() {
    return is_empty_list() || (is_pair() && is_proper_list_);
}

bool SchemeObject::is_tagged_list(std::string tag) {
    if (type_ == Type::PAIR) {

        SchemeObject* car = data.pair.car;

        return car->is_symbol()

            && car->to_symbol()->value() == tag;
    }
    return false;
}

bool SchemeObject::is_self_evaluating() {
    switch (type_) {
        case EMPTY_LIST:
        case FIXNUM:
        case FLONUM:
        case BOOLEAN:
        case STRING:
        case CHARACTER:
        case VECTOR:
        case UNSPECIFIED:
            return true;
        default:
            return false;
    }
}

bool SchemeObject::is_application() {
    return is_pair();
}

void SchemeObject::protect_from_gc() {
    collectible_ = false;
}

bool SchemeObject::collectible() {
    return collectible_;
}

bool SchemeObject::is_true() {
    return !(this == &the_false_object_);
}

SchemeObject* SchemeObject::car() {
    if (type_ == PAIR) {
        return data.pair.car;
    }
    return nullptr;
}

SchemeObject* SchemeObject::cdr() {
    if (type_ == PAIR) {
        return data.pair.cdr;
    }
    return nullptr;
}

void SchemeObject::set_car(SchemeObject* car) {
    if (type_ == PAIR) {
        data.pair.car = car;
    }
}

void SchemeObject::set_cdr(SchemeObject* cdr) {
    if (type_ == PAIR) {
        data.pair.cdr = cdr;
        is_proper_list_ = cdr->is_proper_list();
        length_as_list_ = cdr->length_as_list() + 1;
    }
}

SchemeObject* SchemeObject::caar() {
    return car()->car();
}

SchemeObject* SchemeObject::cdar() {
    return car()->cdr();
}

SchemeObject* SchemeObject::cadr() {
    return cdr()->car();
}

SchemeObject* SchemeObject::cddr() {
    return cdr()->cdr();
}

SchemeObject* SchemeObject::cdadr() {
    return cdr()->car()->cdr();
}

SchemeObject* SchemeObject::caadr() {
    return cdr()->car()->car();
}

SchemeObject* SchemeObject::caddr() {
    return cdr()->cdr()->car();
}

SchemeObject* SchemeObject::cdddr() {
    return cdr()->cdr()->cdr();
}

void SchemeObject::init_fixnum(long value) {
    type_ = FIXNUM;
    data.fixnum.value = value;
}

void SchemeObject::init_flonum(double value) {
    type_ = FLONUM;
    data.flonum.value = value;
}

void SchemeObject::init_char(char value) {
    type_ = CHARACTER;
    data.character.value = value;
}

void SchemeObject::init_pair(SchemeObject* car, SchemeObject* cdr) {
    type_ = PAIR;

    data.pair.car = car;
    data.pair.cdr = cdr;

    is_proper_list_ = cdr->is_proper_list_;
    length_as_list_ = -1;

    if (is_proper_list_) {
        length_as_list_ = cdr->length_as_list_ + 1;
    }
}

//============================================================================
// SchemeSymbol
//============================================================================

std::unordered_map<std::string, SchemeSymbol::Ptr> SchemeSymbol::symbols_;

void SchemeSymbol::clear_symbols() {
    symbols_.clear();
}

SchemeSymbol::SchemeSymbol(std::string& val) :
    SchemeObject(SchemeObject::SYMBOL),
    value_(val)
{}

SchemeSymbol* SchemeSymbol::make_symbol(std::string& val) {
    auto found = symbols_.find(val);

    if (found == symbols_.end()) {
        symbols_[val].reset(new SchemeSymbol(val));
    }

    return symbols_[val].get();
}
