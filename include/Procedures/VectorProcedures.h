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

#ifndef VECTOR_PROCEDURES_H_
#define VECTOR_PROCEDURES_H_

#include "Procedures/SchemePrimProcedure.h"

class SchemeObjectCreator;

namespace VectorProcedures {

SchemeObject* make_vec(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* vec_ref(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* vec_set(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* vec_to_list(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* vec_length(SchemeObject* args, SchemeObjectCreator* creator);
    
} /* namespace VectorProcedures */

#endif /* VECTOR_PROCEDURES_H_ */
