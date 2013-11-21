#ifndef SCHEMEOBJECT_H_
#define SCHEMEOBJECT_H_

#include <unordered_set>
#include <string>
#include <memory>

class SchemeObject {
public:
    typedef std::unique_ptr<SchemeObject> Ptr;

    enum Type {
        UNKNOWN,
        FIXNUM,
        BOOLEAN,
        CHARACTER,
        STRING,
        EMPTY_LIST,
        PAIR,
        SYMBOL,
        PROCEDURE
    };
    Type type();

    bool is_tagged_list(std::string tag);
    bool is_tagged_list(SchemeObject* tag);
    bool is_self_evaluating();

    SchemeFixnum* as_fixnum();
    SchemeBoolean* as_boolean();
    SchemeCharacter* as_character();
    SchemeString* as_string();
    SchemePair* as_pair();
    SchemeSymbol* as_symbol();
private:
    SchemeObject(Type type);
    Type type_ = UNKNOWN;

    friend class SchemeObjectCreator;
};

template <class T>
class PrimitiveObject : public SchemeObject {
public:
    PrimitiveObject(T value) :
        SchemeObject(type),
        value_(value) {}

    T value() const { return value_; }
private:
    T value_;
};

typedef PrimitiveObject<int> SchemeFixnum;
typedef PrimitiveObject<bool> SchemeBoolean;
typedef PrimitiveObject<char> SchemeCharacter;
typedef PrimitiveObject<std::string> SchemeString;

class SchemeSymbol : public SchemeObject {
public:
    static const std::unordered_set<std::string>& all_symbols() {
        return symbols_;
    }
private:
    SchemeSymbol(std::string value) :
        SchemeObject(SchemeObject::SYMBOL),
        value_(value)
    {
        symbols_.insert(value);
    }

    std::string value_;
    static std::unordered_set<std::string> symbols_;
};

class SchemePair : public SchemeObject {
public:
    SchemeObject* car(){
        return car_;
    }

    SchemeObject* cdr() {
        return cdr_;
    }

    bool is_proper_list();
private:
    SchemeObject* car_;
    SchemeObject* cdr_;

    bool proper_list_;
};

typedef SchemeObject* (*Procedure)(SchemeObject*);

class SchemePrimProcedure : public SchemeObject {
public:
    SchemeProcedure(Procedure fn) : func_(fn) {}
    int argc() {
        return argc_;
    }

    bool is_apply(){ return false; }
    bool is_eval(){ return false; }
private:
    int argc_;
    Procedure func_;
};

#endif /* SCHEMEOBJECT_H_ */
