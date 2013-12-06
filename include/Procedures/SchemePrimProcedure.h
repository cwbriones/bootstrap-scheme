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
        func_(func),
        argc_(argc),
        obj_creator_(creator) {}

    SchemeObject* apply(SchemeObject* args) {
        return func_(args, obj_creator_);
    }
    
    bool is_apply() { return false; }
    bool is_eval() { return false; }
private:
    bool check_arg_length();

    procedure_t func_;
    int argc_;
    SchemeObjectCreator* obj_creator_;

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
    
} /* namespace ArithmeticProcedures */

class SchemeAddProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeAddProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator) {}
    friend class SchemeObjectCreator;
};

class SchemeSubProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeSubProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator) {}
    friend class SchemeObjectCreator;
};

class SchemeMultProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeMultProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator) {}
    friend class SchemeObjectCreator;
};

class SchemeDivProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeDivProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator) {}
    friend class SchemeObjectCreator;
};

class SchemeLessThanProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeLessThanProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator) {}
    friend class SchemeObjectCreator;
};

class SchemeGreaterThanProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeGreaterThanProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator) {}
    friend class SchemeObjectCreator;
};

class SchemeEqualsProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeEqualsProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator) {}
    friend class SchemeObjectCreator;
};
 
class SchemeQuotientProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeQuotientProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 2) {}
    friend class SchemeObjectCreator;
};

class SchemeModuloProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeModuloProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 2) {}
    friend class SchemeObjectCreator;
};

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

class SchemeLengthProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeLengthProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, -1) {}
    friend class SchemeObjectCreator;
};

class SchemeConsProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeConsProcedure() :
        SchemePrimProcedure(nullptr, 2) {}
    friend class SchemeObjectCreator;
};

class SchemeCarProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeCarProcedure() :
        SchemePrimProcedure(nullptr, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeCdrProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeCdrProcedure() :
        SchemePrimProcedure(nullptr, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeSetCarProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeSetCarProcedure() :
        SchemePrimProcedure(nullptr, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeSetCdrProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeSetCdrProcedure() :
        SchemePrimProcedure(nullptr, 1) {}
    friend class SchemeObjectCreator;
};

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
