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

#endif /* VECTOR_PROCEDURES_H_ */
