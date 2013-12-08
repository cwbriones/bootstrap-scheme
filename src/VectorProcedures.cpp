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
#include "SchemeObjectCreator.h"
#include "Procedures/VectorProcedures.h"

namespace VectorProcedures {

SchemeObject* make_vec(SchemeObject* args, SchemeObjectCreator* creator) {
    int size = args->car()->fixnum_value();

    SchemeObject* init;
    if (args->cdr()->is_empty_list()) {
        init = creator->make_boolean(false);
    } else {
        init = args->cadr();
    }

    return creator->make_vector(size, init);
}

SchemeObject* vec_ref(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeVector* the_vector = args->car()->to_vector();
    int index = args->cadr()->fixnum_value();

    return the_vector->at(index);
}

SchemeObject* vec_set(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeVector* the_vector = args->car()->to_vector();
    int index = args->cadr()->fixnum_value();
    
    the_vector->set(index, args->caddr());

    return creator->make_symbol("ok");
}

SchemeObject* vec_to_list(SchemeObject* args, SchemeObjectCreator* creator) {
    std::vector<SchemeObject*> the_vector = args->car()->to_vector()->data();
    SchemeObject* the_list = creator->make_empty_list();

    for (auto iter = the_vector.rbegin(); iter != the_vector.rend(); ++iter) {
        the_list = creator->make_pair(*iter, the_list);
    }
    return the_list;
}

SchemeObject* vec_length(SchemeObject* args, SchemeObjectCreator* creator) {
    int length = args->car()->to_vector()->length();

    return creator->make_fixnum(length);
}
    
} /* namespace VectorProcedures */
