#include "SchemeObjectCreator.h"
#include "Procedures/StringProcedures.h"

SchemeObject* ListStringProcedure::func(SchemeObject* args) {
    std::string str("");

    args = args->car();

    while (!args->is_empty_list()) {
        str += args->car()->char_value();

        args = args->cdr();
    }

    return obj_creator_->make_string(str);
}

SchemeObject* StringRefProcedure::func(SchemeObject* args) {
    int index = args->cadr()->fixnum_value();
    args = args->car();

    char c = args->to_string()->value().at(index);

    return obj_creator_->make_character(c);
}

SchemeObject* StringLengthProcedure::func(SchemeObject* args) {
    args = args->car();
    int length = args->to_string()->value().size();

    return obj_creator_->make_fixnum(length);
}
