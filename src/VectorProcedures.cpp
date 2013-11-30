#include "SchemeObject.h"
#include "SchemeObjectCreator.h"
#include "Procedures/VectorProcedures.h"

SchemeObject* MakeVectorProcedure::func(SchemeObject* args) {
    int size = args->car()->to_fixnum()->value();

    SchemeObject* init;
    if (args->cdr()->is_empty_list()) {
        init = obj_creator_->make_boolean(false);
    } else {
        init = args->cadr();
    }

    return obj_creator_->make_vector(size, init);
}
