#include "SchemeObject.h"
#include "SchemeEnvironment.h"

#include "Procedures/SchemePrimProcedure.h"
#include "Procedures/SchemeCompoundProcedure.h"

#include <iostream>

int SchemeObject::objects_created_ = 0;
int SchemeObject::objects_destroyed_ = 0;

void SchemeObject::object_summary() {
    std::cout << "Objects Created: " << objects_created_ << std::endl;
    std::cout << "Objects Destroyed: " << objects_destroyed_ << std::endl;

    std::cout << "Unaccounted for: " << 
        objects_created_ - objects_destroyed_ << std::endl;
}

SchemeBoolean SchemeObject::the_false_object_ = 
    SchemeBoolean(SchemeObject::BOOLEAN, false);

SchemeBoolean SchemeObject::the_true_object_ =
    SchemeBoolean(SchemeObject::BOOLEAN, true);

SchemeObject SchemeObject::the_empty_list_ = SchemeObject(EMPTY_LIST);

SchemeObject SchemeObject::the_unspecified_object_ = SchemeObject(UNSPECIFIED);

SchemeObject::SchemeObject(Type t) : type_(t) {
    objects_created_++;

    if (type_ == EMPTY_LIST) {
        is_proper_list_ = true;
        length_as_list_ = 0;
    } else if (type_ == PAIR) {
        length_as_list_ = 0;
    }
}

SchemeObject::~SchemeObject() {
    objects_destroyed_++;
}

SchemeObject::Type SchemeObject::type() const {
    return type_;
}

SchemeFixnum* SchemeObject::to_fixnum() {
    return static_cast<SchemeFixnum*>(this);
}

SchemeFlonum* SchemeObject::to_flonum() {
    return static_cast<SchemeFlonum*>(this);
}

SchemeBoolean* SchemeObject::to_boolean() {
    return static_cast<SchemeBoolean*>(this);
}

SchemeCharacter* SchemeObject::to_character() {
    return static_cast<SchemeCharacter*>(this);
}

SchemeString* SchemeObject::to_string() {
    return static_cast<SchemeString*>(this);
}

SchemeSymbol* SchemeObject::to_symbol() {
    return static_cast<SchemeSymbol*>(this);
}

SchemePrimProcedure* SchemeObject::to_prim_procedure() {
    return static_cast<SchemePrimProcedure*>(this);
}

SchemeCompoundProcedure* SchemeObject::to_comp_procedure() {
    return static_cast<SchemeCompoundProcedure*>(this);
}

SchemeEnvironment* SchemeObject::to_environment() {
    return static_cast<SchemeEnvironment*>(this);
}

int SchemeObject::length_as_list() {
    return length_as_list_;
}

bool SchemeObject::is_true_obj() {
    return this == &the_true_object_;
}

bool SchemeObject::is_false_obj() {
    return this == &the_false_object_;
}

bool SchemeObject::is_empty_list() {
    return this == &the_empty_list_;
}

bool SchemeObject::is_fixnum() {
    return type_ == Type::FIXNUM;
}

bool SchemeObject::is_flonum() {
    return type_ == Type::FLONUM;
}

bool SchemeObject::is_boolean() {
    return type_ == Type::BOOLEAN;
}

bool SchemeObject::is_character() {
    return type_ == Type::CHARACTER;
}

bool SchemeObject::is_string() {
    return type_ == Type::STRING;
}

bool SchemeObject::is_pair() {
    return type_ == Type::PAIR;
}

bool SchemeObject::is_symbol() {
    return type_ == Type::SYMBOL;
}

bool SchemeObject::is_prim_procedure() {
    return type_ == Type::PRIMPROCEDURE;
}

bool SchemeObject::is_comp_procedure() {
    return type_ == Type::COMPPROCEDURE;
}

bool SchemeObject::is_proper_list() {
    return is_empty_list() || (is_pair() && is_proper_list_);
}

bool SchemeObject::is_tagged_list(std::string tag) {
    if (type_ == Type::PAIR) {

        SchemeObject* car = data.pair.car;

        return car->is_symbol()

            && car->to_symbol()->value() == tag;
    }
    return false;
}

bool SchemeObject::is_self_evaluating() {
    switch (type_) {
        case EMPTY_LIST:
        case FIXNUM:
        case FLONUM:
        case BOOLEAN:
        case STRING:
        case CHARACTER:
            return true;
        default:
            return false;
    }
}

bool SchemeObject::is_application() {
    return is_pair();
}

bool SchemeObject::collectible() {
    return !(type_ & (UNSPECIFIED | SYMBOL | EMPTY_LIST | BOOLEAN | ENVIRONMENT));
}

bool SchemeObject::is_true() {
    return !this->is_false_obj();
}

SchemeObject* SchemeObject::car() {
    if (type_ == PAIR) {
        return data.pair.car;
    }
    return nullptr;
}

SchemeObject* SchemeObject::cdr() {
    if (type_ == PAIR) {
        return data.pair.cdr;
    }
    return nullptr;
}

void SchemeObject::set_car(SchemeObject* car) {
    if (type_ == PAIR) {
        data.pair.car = car;
    }
}

void SchemeObject::set_cdr(SchemeObject* cdr) {
    if (type_ == PAIR) {
        data.pair.cdr = cdr;
        is_proper_list_ = cdr->is_proper_list();
        length_as_list_ = cdr->length_as_list() + 1;
    }
}

SchemeObject* SchemeObject::caar() {
    return car()->car();
}

SchemeObject* SchemeObject::cdar() {
    return car()->cdr();
}

SchemeObject* SchemeObject::cadr() {
    return cdr()->car();
}

SchemeObject* SchemeObject::cddr() {
    return cdr()->cdr();
}

SchemeObject* SchemeObject::cdadr() {
    return cdr()->car()->cdr();
}

SchemeObject* SchemeObject::caadr() {
    return cdr()->car()->car();
}

SchemeObject* SchemeObject::caddr() {
    return cdr()->cdr()->car();
}

SchemeObject* SchemeObject::cdddr() {
    return cdr()->cdr()->cdr();
}

//============================================================================
// SchemeSymbol
//============================================================================

std::unordered_map<std::string, size_t> SchemeSymbol::ref_count_;
std::unordered_map<std::string, SchemeSymbol*> SchemeSymbol::symbols_;

SchemeSymbol::SchemeSymbol(std::string& val) :
    SchemeObject(SchemeObject::SYMBOL),
    value_(val)
{
    symbols_[val] = this;
    ref_count_[val] = 1;
}

SchemeSymbol::~SchemeSymbol() {
    // Decrement reference count
    int count = (--ref_count_[value_]);

    if (count == 0) {
        // Remove from the table of symbols
        auto ref_iter = ref_count_.find(value_);
        auto symbol_iter = symbols_.find(value_);

        ref_count_.erase(ref_iter);
        symbols_.erase(symbol_iter);
    }
}

SchemeSymbol* SchemeSymbol::make_symbol(std::string& val) {
    auto found = symbols_.find(val);

    if (found != symbols_.end()) {
        ref_count_[val]++;
        return found->second;
    } else {
        return new SchemeSymbol(val);
    }
}
