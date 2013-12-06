#ifndef STRING_PROCEDURES_H
#define STRING_PROCEDURES_H

#include "Procedures/SchemePrimProcedure.h"

namespace StringProcedures {

SchemeObject* ListToString(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* StringRef(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* StringLength(SchemeObject* args, SchemeObjectCreator* creator);

} /* namespace StringProcedures */

#endif // STRING_PROCEDURES_H
