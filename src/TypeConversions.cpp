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
#include "SchemeObjectCreator.h"
#include "Procedures/TypeConversions.h"

#include <sstream>

namespace TypeConversions {

SchemeObject* char_to_int(SchemeObject* args, SchemeObjectCreator* creator) {
    return creator->make_fixnum(args->car()->fixnum_value());
}

SchemeObject* int_to_char(SchemeObject* args, SchemeObjectCreator* creator) {
    return creator->make_character(args->car()->char_value());
}

SchemeObject* string_to_int(SchemeObject* args, SchemeObjectCreator* creator) {
    std::stringstream ss(args->car()->to_string()->value());

    int value;
    ss >> value;

    return creator->make_fixnum(value);
}

SchemeObject* int_to_string(SchemeObject* args, SchemeObjectCreator* creator) {
    std::stringstream ss;

    int value = args->car()->fixnum_value();
    ss << value;

    return creator->make_string(ss.str());
}
    
SchemeObject* string_to_symbol(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeObject* obj = args->car();
    return creator->make_symbol(obj->to_string()->value());
}

SchemeObject* symbol_to_string(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeObject* obj = args->car();
    return creator->make_string(obj->to_symbol()->value());
}

}   /* namespace TypeConversions */
