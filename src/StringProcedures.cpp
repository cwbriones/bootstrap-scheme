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

#include "SchemeObjectCreator.h"
#include "Procedures/StringProcedures.h"

namespace StringProcedures {
    
SchemeObject* ListToString(SchemeObject* args, SchemeObjectCreator* creator) {
    std::string str("");

    args = args->car();

    while (!args->is_empty_list()) {
        str += args->car()->char_value();

        args = args->cdr();
    }

    return creator->make_string(str);
}

SchemeObject* StringRef(SchemeObject* args, SchemeObjectCreator* creator) {
    int index = args->cadr()->fixnum_value();
    args = args->car();

    char c = args->to_string()->value().at(index);

    return creator->make_character(c);
}

SchemeObject* StringLength(SchemeObject* args, SchemeObjectCreator* creator) {
    args = args->car();
    int length = args->to_string()->value().size();

    return creator->make_fixnum(length);
}

} /* namespace StringProcedures */
