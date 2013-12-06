#include "SchemeObject.h"
#include "SchemeObjectCreator.h"
#include "Procedures/TypeConversions.h"

#include <sstream>

namespace TypeConversions {

SchemeObject* char_to_int(SchemeObject* args, SchemeObjectCreator* creator) {
    return creator->make_fixnum(args->car()->fixnum_value());
}

SchemeObject* int_to_char(SchemeObject* args, SchemeObjectCreator* creator) {
    return creator->make_character(args->car()->char_value());
}

SchemeObject* string_to_int(SchemeObject* args, SchemeObjectCreator* creator) {
    std::stringstream ss(args->car()->to_string()->value());

    int value;
    ss >> value;

    return creator->make_fixnum(value);
}

SchemeObject* int_to_string(SchemeObject* args, SchemeObjectCreator* creator) {
    std::stringstream ss;

    int value = args->car()->fixnum_value();
    ss << value;

    return creator->make_string(ss.str());
}
    
SchemeObject* string_to_symbol(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeObject* obj = args->car();
    return creator->make_symbol(obj->to_string()->value());
}

SchemeObject* symbol_to_string(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeObject* obj = args->car();
    return creator->make_string(obj->to_symbol()->value());
}

}   /* namespace TypeConversions */
