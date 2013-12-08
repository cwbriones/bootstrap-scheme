/*
 * Copyright (C) Christian Briones, 2013
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCHEMEOBJECT_H_
#define SCHEMEOBJECT_H_

#include <unordered_map>
#include <string>
#include <memory>

// Forward Declarations needed
template <class T>
class SchemePrimitive;
typedef SchemePrimitive<std::string> SchemeString;

class SchemeSymbol;
class SchemeVector;
class SchemePrimProcedure;
class NewPrimProcedure;
class SchemeCompoundProcedure;
class SchemeEnvironment;
class SchemeInputPort;
class SchemeOutputPort;

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
          ENVIRONMENT = 1 << 11,
               VECTOR = 1 << 12,
           INPUT_PORT = 1 << 13,
          OUTPUT_PORT = 1 << 14,
           EOF_OBJECT = 1 << 15,
              UNKNOWN = 1 << 16
    };
    Type type() const;

    // Casts
    long fixnum_value();
    double flonum_value();
    char char_value();
    bool boolean_value();

    SchemeString* to_string();
    SchemeSymbol* to_symbol();

    // Compound data types
    SchemePrimProcedure* to_prim_procedure();
    SchemeCompoundProcedure* to_comp_procedure();
    SchemeVector* to_vector();
    SchemeEnvironment* to_environment();

    // IO Objects
    SchemeInputPort* to_input_port();
    SchemeOutputPort* to_output_port();

    // Boolean operations
    bool is_tagged_list(std::string tag);
    bool is_tagged_list(SchemeObject* tag);
    bool is_self_evaluating();

    // Single Instance Check
    bool is_true_obj();
    bool is_false_obj();
    bool is_empty_list();
    bool is_eof();

    // Type check
    bool is_fixnum();
    bool is_flonum();
    bool is_boolean();
    bool is_character();
    bool is_string();
    bool is_pair();
    bool is_symbol();
    bool is_prim_procedure();
    bool is_comp_procedure();
    bool is_input_port();
    bool is_output_port();
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
    SchemeObject();
    SchemeObject(SchemeObject::Type t);

    // Initialization
    void init_fixnum(long value);
    void init_flonum(double value);
    void init_char(char value);
    void init_pair(SchemeObject* car, SchemeObject* cdr);

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

    // Single Instances of their types
    static SchemeObject the_unspecified_object_;
    static SchemeObject the_true_object_;
    static SchemeObject the_false_object_;
    static SchemeObject the_empty_list_;
    static SchemeObject the_eof_object_;

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
    typedef std::unique_ptr<SchemeSymbol> Ptr;

    static SchemeSymbol* make_symbol(std::string& val);
    static void clear_symbols();

    std::string value(){ return value_; }
private:
    SchemeSymbol(std::string& value);

    std::string value_;
    static std::unordered_map<std::string, SchemeSymbol::Ptr> symbols_;
};

#endif /* SCHEMEOBJECT_H_ */
