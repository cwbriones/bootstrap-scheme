#ifndef STRING_PROCEDURES_H
#define STRING_PROCEDURES_H

#include "Procedures/SchemePrimProcedure.h"

class StringRefProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    StringRefProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 2) {}
    friend class SchemeObjectCreator;
};

class StringLengthProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    StringLengthProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

#endif // STRING_PROCEDURES_H
