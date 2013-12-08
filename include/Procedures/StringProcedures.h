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

#ifndef STRING_PROCEDURES_H
#define STRING_PROCEDURES_H

#include "Procedures/SchemePrimProcedure.h"

namespace StringProcedures {

SchemeObject* ListToString(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* StringRef(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* StringLength(SchemeObject* args, SchemeObjectCreator* creator);

} /* namespace StringProcedures */

#endif // STRING_PROCEDURES_H
