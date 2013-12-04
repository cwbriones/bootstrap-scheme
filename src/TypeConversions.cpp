#include "SchemeObject.h"
#include "SchemeObjectCreator.h"
#include "Procedures/TypeConversions.h"

#include <sstream>

SchemeObject* SchemeCharToIntProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    int value = static_cast<int>(obj->char_value());

    return obj_creator_->make_fixnum(value);
}

SchemeObject* SchemeIntToCharProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    char value = static_cast<char>(obj->fixnum_value());

    return obj_creator_->make_character(value);
}

SchemeObject* SchemeStringToIntProcedure::func(SchemeObject* args) {
    std::stringstream ss(args->car()->to_string()->value());
    int value;

    ss >> value;

    return obj_creator_->make_fixnum(value);
}

SchemeObject* SchemeIntToStringProcedure::func(SchemeObject* args) {
    std::stringstream ss;
    int value = args->car()->fixnum_value();
    ss << value;

    return obj_creator_->make_string(ss.str());
}

SchemeObject* SchemeStringToSymbolProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    return obj_creator_->make_symbol(obj->to_string()->value());
}

SchemeObject* SchemeSymbolToStringProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    return obj_creator_->make_string(obj->to_symbol()->value());
}
