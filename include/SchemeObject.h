#ifndef SCHEMEOBJECT_H_
#define SCHEMEOBJECT_H_

#include <unordered_map>
#include <string>
#include <memory>

// Forward Declarations needed
template <class T>
class SchemePrimitive;

typedef SchemePrimitive<int> SchemeFixnum;
typedef SchemePrimitive<double> SchemeFlonum;
typedef SchemePrimitive<bool> SchemeBoolean;
typedef SchemePrimitive<char> SchemeCharacter;
typedef SchemePrimitive<std::string> SchemeString;

class SchemeSymbol;
class SchemePrimProcedure;
class SchemeCompoundProcedure;
class SchemeEnvironment;

//============================================================================
// SchemeObject
//============================================================================

class SchemeObject {
public:
    typedef std::unique_ptr<SchemeObject> Ptr;
    virtual ~SchemeObject();

    enum Type {
          UNSPECIFIED = 1,
               FIXNUM = 1 << 1,
               FLONUM = 1 << 2,
              BOOLEAN = 1 << 3,
            CHARACTER = 1 << 4,
               STRING = 1 << 5,
           EMPTY_LIST = 1 << 6,
                 PAIR = 1 << 7,
               SYMBOL = 1 << 8,
        PRIMPROCEDURE = 1 << 9,
        COMPPROCEDURE = 1 << 10,
          ENVIRONMENT = 1 << 11
    };
    Type type() const;

    // Casts
    SchemeFixnum* to_fixnum();
    SchemeFlonum* to_flonum();
    SchemeBoolean* to_boolean();
    SchemeCharacter* to_character();
    SchemeString* to_string();
    SchemeSymbol* to_symbol();
    SchemePrimProcedure* to_prim_procedure();
    SchemeCompoundProcedure* to_comp_procedure();
    SchemeEnvironment* to_environment();

    // Boolean operations
    bool is_tagged_list(std::string tag);
    bool is_tagged_list(SchemeObject* tag);
    bool is_self_evaluating();

    bool is_true_obj();
    bool is_false_obj();
    bool is_empty_list();

    bool is_fixnum();
    bool is_flonum();
    bool is_boolean();
    bool is_character();
    bool is_string();
    bool is_pair();
    bool is_symbol();
    bool is_prim_procedure();
    bool is_comp_procedure();
    bool is_proper_list();

    bool is_application();
    bool is_true();
    bool collectible();

    void protect_from_gc();

    // Specific checks for object equality

    // Pair Operations
    SchemeObject* car();
    SchemeObject* cdr();
    void set_car(SchemeObject* car);
    void set_cdr(SchemeObject* cdr);

    SchemeObject* caar();
    SchemeObject* cdar();
    SchemeObject* cadr();
    SchemeObject* cddr();
    SchemeObject* caadr();
    SchemeObject* caddr();
    SchemeObject* cdadr();
    SchemeObject* cdddr();

    int length_as_list();

    static void object_summary();
protected:
    SchemeObject(SchemeObject::Type t);
    Type type_ = UNSPECIFIED;

    int mark_ = 0;
    int length_as_list_ = -1;
    bool is_proper_list_ = false;
    bool collectible_ = true;

    union {
        struct {
            bool value;
        } boolean;
        struct {
            char value;
        } character;
        struct {
            long value;
        } fixnum;
        struct {
            double value;
        } flonum;
        struct {
            SchemeObject* car;
            SchemeObject* cdr;
        } pair;
    } data;

    // Singletons
    static SchemeObject the_unspecified_object_;
    static SchemeBoolean the_true_object_;
    static SchemeBoolean the_false_object_;
    static SchemeObject the_empty_list_;

    friend class SchemeObjectCreator;

    static int objects_created_;
    static int objects_destroyed_;
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

#endif /* SCHEMEOBJECT_H_ */
