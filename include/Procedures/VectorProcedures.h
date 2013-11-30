#ifndef VECTOR_PROCEDURES_H_
#define VECTOR_PROCEDURES_H_

#include "Procedures/SchemePrimProcedure.h"

class SchemeObjectCreator;

class MakeVectorProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    MakeVectorProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 2) {}
    friend class SchemeObjectCreator;
};

class VectorRefProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    VectorRefProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 2) {}
    friend class SchemeObjectCreator;
};

class VectorSetProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    VectorSetProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 3) {}
    friend class SchemeObjectCreator;
};

class VectorToListProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    VectorToListProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class VectorLengthProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    VectorLengthProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};
#endif /* VECTOR_PROCEDURES_H_ */
