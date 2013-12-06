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
