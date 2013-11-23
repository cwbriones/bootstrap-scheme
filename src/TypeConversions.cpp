#include "SchemeObject.h"
#include "SchemeObjectCreator.h"
#include "Procedures/TypeConversions.h"

SchemeObject* SchemeCharToIntProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    int value = static_cast<int>(obj->to_character()->value());

    return obj_creator_->make_fixnum(value);
}

SchemeObject* SchemeIntToCharProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    char value = static_cast<char>(obj->to_fixnum()->value());

    return obj_creator_->make_character(value);
}

SchemeObject* SchemeStringToIntProcedure::func(SchemeObject* args) {
}

SchemeObject* SchemeIntToStringProcedure::func(SchemeObject* args) {
}

SchemeObject* SchemeStringToSymbolProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    return obj_creator_->make_symbol(obj->to_string()->value());
}

SchemeObject* SchemeSymbolToStringProcedure::func(SchemeObject* args) {
    SchemeObject* obj = args->car();
    return obj_creator_->make_string(obj->to_symbol()->value());
}
