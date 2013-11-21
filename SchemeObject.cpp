#include "SchemeObject.h"

SchemeObject::SchemeObject(Type type) : type_(type) {}

SchemeObject::Type SchemeObject::type() const {
    return type_;
}

inline SchemeFixnum* SchemeObject::as_fixnum() {
    return static_cast<FixnumObject*>(this);
}

inline SchemeBoolean* SchemeObject::as_boolean() {
    return static_cast<BoolObject*>(this);
}

inline SchemeCharacter* SchemeObject::as_character() {
    return static_cast<CharObject*>(this);
}

inline SchemeString* SchemeObject::as_string() {
    return static_cast<StringObject*>(this);
}

inline SchemePair* SchemeObject::as_pair() {
    return static_cast<SchemePair*>(this);
}

inline SchemeSymbol* SchemeObject::as_symbol() {
    return static_cast<SymbolSymbol*>(this);
}

//==============================================================================
// SchemePair
//==============================================================================

SchemePair::SchemePair() :
    SchemeObject(EMPTY_LIST),
    car_(nullptr),
    cdr_(nullptr),
    proper_list_(true),
{}

SchemePair::SchemePair(SchemeObject* car, SchemeObject* cdr) :
    SchemeObject(PAIR),
    car_(car),
    cdr_(cdr),
    proper_list_(false)
{
    int cdr_type = cdr->type();

    if (cdr_type == Type::EMPTY_LIST) {
        proper_list_ = true;
    } else if (cdr_type == Type::PAIR) {
        proper_list_ = cdr->as_pair()->is_proper_list();
    }
}

inline bool SchemePair::is_proper_list() {
    return proper_list_;
}

//==============================================================================
// SchemeProcedure
//==============================================================================

