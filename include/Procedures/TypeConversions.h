#ifndef TYPECONVERSIONS_H_
#define TYPECONVERSIONS_H_

#include "SchemePrimProcedure.h"

namespace TypeConversions {

SchemeObject* char_to_int(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* int_to_char(SchemeObject* args, SchemeObjectCreator* creator);

SchemeObject* string_to_int(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* int_to_string(SchemeObject* args, SchemeObjectCreator* creator);
    
SchemeObject* string_to_symbol(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* symbol_to_string(SchemeObject* args, SchemeObjectCreator* creator);

} /* namespace TypeConversions */

#endif /* TYPECONVERSIONS_H_ */
