#ifndef ENVIRONMENT_PROCEDURES_H
#define ENVIRONMENT_PROCEDURES_H

#include "Procedures/SchemePrimProcedure.h"

class InteractionEnvironmentProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    InteractionEnvironmentProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 0) {}
    friend class SchemeObjectCreator;
};

class NullEnvironmentProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    NullEnvironmentProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 0) {}
    friend class SchemeObjectCreator;
};

class StandardEnvironmentProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    StandardEnvironmentProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 0) {}
    friend class SchemeObjectCreator;
};

#endif // ENVIRONMENT_PROCEDURES_H
