#ifndef SCHEMEPRIMPROCEDURE_H_
#define SCHEMEPRIMPROCEDURE_H_

#include "SchemeObject.h"

class SchemeObjectCreator;

class SchemePrimProcedure : public SchemeObject {
public:
    SchemePrimProcedure(SchemeObjectCreator* creator, int argc=-1) :
        SchemeObject(SchemeObject::PRIMPROCEDURE),
        argc_(argc),
        obj_creator_(creator) {}

    virtual ~SchemePrimProcedure(){}
    virtual SchemeObject* func(SchemeObject* args) = 0;

protected:
    int argc_;
    SchemeObjectCreator* obj_creator_ = nullptr;

    bool check_arg_length(SchemeObject* args);
};

//============================================================================
// Arithmetic Operators
//============================================================================

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

// class SchemeNAMEProcedure : public SchemePrimProcedure {
// public:
//     virtual SchemeObject* func(SchemeObject* args);
// private:
//     SchemeNAMEProcedure(SchemeObjectCreator* creator) :
//         SchemePrimProcedure(creator) {}
//     friend class SchemeObjectCreator;
// };

//============================================================================
// List Operations
//============================================================================

class SchemeListProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeListProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, -1) {}
    friend class SchemeObjectCreator;
};

class SchemeConsProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeConsProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 2) {}
    friend class SchemeObjectCreator;
};

class SchemeCarProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeCarProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeCdrProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeCdrProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeSetCarProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeSetCarProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeSetCdrProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeSetCdrProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};
#endif /* SCHEMEPRIMPROCEDURE_H_ */
