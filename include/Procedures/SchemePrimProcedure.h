#ifndef SCHEMEPRIMPROCEDURE_H_
#define SCHEMEPRIMPROCEDURE_H_

#include "SchemeObject.h"
#include <functional>

class SchemeObjectCreator;

class NewPrimProcedure : public SchemeObject {
public:
    typedef std::function<SchemeObject*(SchemeObject*, SchemeObjectCreator*)>
        procedure_t;

    NewPrimProcedure(SchemeObjectCreator* creator, 
            procedure_t func,
            int argc=-1) :
        SchemeObject(NEWPROCEDURE),
        argc_(argc)
    {
        func_ = std::bind2nd(func, creator);
    }

    SchemeObject* apply(SchemeObject* args) {
        return func_(args);
    }
    
    bool is_apply() { return false; }
    bool is_eval() { return false; }
private:
    bool check_arg_length();
    std::function<SchemeObject*(SchemeObject*)> func_;

    int argc_;

    friend class SchemeObjectCreator;
};

class SchemePrimProcedure : public SchemeObject {
public:
    SchemePrimProcedure(SchemeObjectCreator* creator, int argc=-1) :
        SchemeObject(SchemeObject::PRIMPROCEDURE),
        argc_(argc),
        obj_creator_(creator) {}

    virtual ~SchemePrimProcedure(){}
    virtual SchemeObject* func(SchemeObject* args) = 0;

    virtual bool is_apply(){ return false;}
    virtual bool is_eval(){ return false;}
protected:
    int argc_;
    SchemeObjectCreator* obj_creator_ = nullptr;

    bool check_arg_length(SchemeObject* args);
};

//============================================================================
// Arithmetic Operators
//============================================================================

namespace ArithmeticProcedures {

SchemeObject* add(SchemeObject*, SchemeObjectCreator*);
SchemeObject* sub(SchemeObject*, SchemeObjectCreator*);
SchemeObject* mul(SchemeObject*, SchemeObjectCreator*);
SchemeObject* div(SchemeObject*, SchemeObjectCreator*);
    
SchemeObject* equals(SchemeObject*, SchemeObjectCreator*);
SchemeObject* less_than(SchemeObject*, SchemeObjectCreator*);
SchemeObject* greater_than(SchemeObject*, SchemeObjectCreator*);

SchemeObject* quotient(SchemeObject*, SchemeObjectCreator*);
SchemeObject* modulo(SchemeObject*, SchemeObjectCreator*);

} /* namespace ArithmeticProcedures */

class SchemePredicateProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemePredicateProcedure(SchemeObjectCreator* creator, uint16_t target) :
        SchemePrimProcedure(creator, 1),
        target_type_(target) {}

    friend class SchemeObjectCreator;

    uint16_t target_type_;
};

#include <random>

class RandomProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    RandomProcedure();
    std::default_random_engine generator_;

    friend class SchemeObjectCreator;
};

//============================================================================
// List Operations
//============================================================================

namespace ListProcedures {

SchemeObject* length(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* cons(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* car(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* cdr(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* set_car(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* set_cdr(SchemeObject* args, SchemeObjectCreator* creator);
    
} /* namespace ListProcedures */

//============================================================================
// Polymorphic Equality Testing
//============================================================================

class SchemePolyEqProcedure : public SchemePrimProcedure { 
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemePolyEqProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 2) {}
    friend class SchemeObjectCreator;
};

//============================================================================
// Boolean Operations
//============================================================================

class SchemeNotProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeNotProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

//============================================================================
// Apply and Eval
//============================================================================

class SchemeApplyProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
    virtual bool is_apply(){ return true;}
private:
    SchemeApplyProcedure() :
        SchemePrimProcedure(nullptr, -1) {}
    friend class SchemeObjectCreator;
};

class SchemeEvalProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
    virtual bool is_eval(){ return true;}
private:
    SchemeEvalProcedure() :
        SchemePrimProcedure(nullptr, 2) {}
    friend class SchemeObjectCreator;
};
#endif /* SCHEMEPRIMPROCEDURE_H_ */
