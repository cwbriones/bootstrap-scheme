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
