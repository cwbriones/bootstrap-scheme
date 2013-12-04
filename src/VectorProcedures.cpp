#include "SchemeObject.h"
#include "SchemeVector.h"
#include "SchemeObjectCreator.h"
#include "Procedures/VectorProcedures.h"

SchemeObject* MakeVectorProcedure::func(SchemeObject* args) {
    int size = args->car()->fixnum_value();

    SchemeObject* init;
    if (args->cdr()->is_empty_list()) {
        init = obj_creator_->make_boolean(false);
    } else {
        init = args->cadr();
    }

    return obj_creator_->make_vector(size, init);
}

SchemeObject* VectorRefProcedure::func(SchemeObject* args) {
    SchemeVector* the_vector = args->car()->to_vector();
    int index = args->cadr()->fixnum_value();

    return the_vector->at(index);
}

SchemeObject* VectorSetProcedure::func(SchemeObject* args) {
    SchemeVector* the_vector = args->car()->to_vector();
    int index = args->cadr()->fixnum_value();
    
    the_vector->set(index, args->caddr());

    return obj_creator_->make_symbol("ok");
}

SchemeObject* VectorToListProcedure::func(SchemeObject* args) {
    std::vector<SchemeObject*> the_vector = args->car()->to_vector()->data();
    SchemeObject* the_list = obj_creator_->make_empty_list();

    for (auto iter = the_vector.rbegin(); iter != the_vector.rend(); ++iter) {
        the_list = obj_creator_->make_pair(*iter, the_list);
    }
    return the_list;
}

SchemeObject* VectorLengthProcedure::func(SchemeObject* args) {
    int length = args->car()->to_vector()->length();

    return obj_creator_->make_fixnum(length);
}
