#ifndef SCHEMEOBJECT_H_
#define SCHEMEOBJECT_H_

#include <unordered_map>
#include <string>
#include <memory>

// Forward Declarations needed
template <class T>
class SchemePrimitive;

typedef SchemePrimitive<int> SchemeFixnum;
typedef SchemePrimitive<bool> SchemeBoolean;
typedef SchemePrimitive<char> SchemeCharacter;
typedef SchemePrimitive<std::string> SchemeString;

class SchemePair;
class SchemeSymbol;
class SchemePrimProcedure;
class SchemeCompoundProcedure;

//============================================================================
// SchemeObject
//============================================================================

class SchemeObject {
public:
    typedef std::unique_ptr<SchemeObject> Ptr;
    virtual ~SchemeObject(){}

    enum Type {
              UNKNOWN = 1,
               FIXNUM = 1 << 1,
              BOOLEAN = 1 << 2,
            CHARACTER = 1 << 3,
               STRING = 1 << 4,
           EMPTY_LIST = 1 << 5,
                 PAIR = 1 << 6,
               SYMBOL = 1 << 7,
        PRIMPROCEDURE = 1 << 8,
        COMPPROCEDURE = 1 << 9
    };
    Type type() const;

    // Casts
    SchemeFixnum* to_fixnum();
    SchemeBoolean* to_boolean();
    SchemeCharacter* to_character();
    SchemeString* to_string();
    SchemePair* to_pair();
    SchemeSymbol* to_symbol();
    SchemePrimProcedure* to_prim_procedure();
    SchemeCompoundProcedure* to_comp_procedure();

    // Boolean operations
    bool is_tagged_list(std::string tag);
    bool is_tagged_list(SchemeObject* tag);
    bool is_self_evaluating();

    bool is_true_obj();
    bool is_false_obj();
    bool is_empty_list();

    bool is_fixnum();
    bool is_boolean();
    bool is_character();
    bool is_string();
    bool is_pair();
    bool is_symbol();
    bool is_prim_procedure();
    bool is_comp_procedure();
    bool is_proper_list();

    bool is_application();

    // Specific checks for object equality

    // Pair Operations
    SchemeObject* car();
    SchemeObject* cdr();

    SchemeObject* caar();
    SchemeObject* cdar();
    SchemeObject* cadr();
    SchemeObject* cddr();
    SchemeObject* caadr();
    SchemeObject* caddr();
    SchemeObject* cdadr();
    SchemeObject* cdddr();

    int length_as_list();
    void set_mark(int mark) {
        mark_ = mark;
    }
    int mark() {
        return mark_;
    }
protected:
    SchemeObject(SchemeObject::Type t);
    Type type_ = UNKNOWN;

    int mark_ = 0;

    // Singletons
    static SchemeBoolean the_true_object_;
    static SchemeBoolean the_false_object_;
    static SchemePair the_empty_list_;

    friend class SchemeObjectCreator;
};

//============================================================================
// SchemePrimitive (int, bool, char, string), saves some typing
//============================================================================

template <class T>
class SchemePrimitive : public SchemeObject {
public:
    T value() const { return value_; }
private:
    SchemePrimitive(SchemeObject::Type t, T val) :
        SchemeObject(t),
        value_(val) {}

    T value_;

    friend class SchemeObject;
    friend class SchemeObjectCreator;
};

//============================================================================
// SchemeSymbol
//============================================================================

class SchemeSymbol : public SchemeObject {
public:
    static SchemeSymbol* make_symbol(std::string& val);
    static const std::unordered_map<std::string, SchemeSymbol*>& all_symbols() {
        return symbols_;
    }
    std::string value(){ return value_; }
private:
    SchemeSymbol(std::string& value);

    std::string value_;
    static std::unordered_map<std::string, SchemeSymbol*> symbols_;
};

//============================================================================
// SchemePair
//============================================================================

class SchemePair : public SchemeObject {
public:
    SchemeObject* car(){
        return car_;
    }

    SchemeObject* cdr(){
        return cdr_;
    }

    SchemeObject* set_cdr(SchemeObject* cdr) {
        cdr_ = cdr;
    }
    
    SchemeObject* set_car(SchemeObject* car) {
        car_ = car;
    }
    int length() {
        return length_;
    }

    bool is_proper_list() {
        return proper_list_;
    }
private:
    SchemePair();
    SchemePair(SchemeObject* car, SchemeObject* cdr);

    SchemeObject* car_;
    SchemeObject* cdr_;

    bool proper_list_;
    int length_;

    friend class SchemeObject;
    friend class SchemeObjectCreator;
};

#endif /* SCHEMEOBJECT_H_ */
