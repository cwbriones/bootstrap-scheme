#include "SchemeObject.h"
#include "SchemeVector.h"
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

SchemeObject* VectorRefProcedure::func(SchemeObject* args) {
    std::vector<SchemeObject*>& the_vector = args->car()->to_vector()->data();
    int index = args->cadr()->to_fixnum()->value();

    return the_vector[index];
}

SchemeObject* VectorSetProcedure::func(SchemeObject* args) {
    std::vector<SchemeObject*>& the_vector = args->car()->to_vector()->data();
    int index = args->cadr()->to_fixnum()->value();
    
    the_vector[index] = args->caddr();

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
