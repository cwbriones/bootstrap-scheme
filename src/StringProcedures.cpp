#include "SchemeObjectCreator.h"
#include "Procedures/StringProcedures.h"

namespace StringProcedures {
    
SchemeObject* ListToString(SchemeObject* args, SchemeObjectCreator* creator) {
    std::string str("");

    args = args->car();

    while (!args->is_empty_list()) {
        str += args->car()->char_value();

        args = args->cdr();
    }

    return creator->make_string(str);
}

SchemeObject* StringRef(SchemeObject* args, SchemeObjectCreator* creator) {
    int index = args->cadr()->fixnum_value();
    args = args->car();

    char c = args->to_string()->value().at(index);

    return creator->make_character(c);
}

SchemeObject* StringLength(SchemeObject* args, SchemeObjectCreator* creator) {
    args = args->car();
    int length = args->to_string()->value().size();

    return creator->make_fixnum(length);
}

} /* namespace StringProcedures */
