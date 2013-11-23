#ifndef TYPECONVERSIONS_H_
#define TYPECONVERSIONS_H_

#include "SchemePrimProcedure.h"

class SchemeCharToIntProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeCharToIntProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeIntToCharProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeIntToCharProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeStringToIntProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeStringToIntProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeIntToStringProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeIntToStringProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeStringToSymbolProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeStringToSymbolProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class SchemeSymbolToStringProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    SchemeSymbolToStringProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

#endif /* TYPECONVERSIONS_H_ */
