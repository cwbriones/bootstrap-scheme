#ifndef INPUT_OUTPUT_H_
#define INPUT_OUTPUT_H_

#include <string>
#include <fstream>

#include "SchemeObject.h"
#include "SchemeWriter.h"
#include "Reader.h"

#include "Procedures/SchemePrimProcedure.h"

//============================================================================
// Input
//============================================================================

class SchemeObjectCreator;

class SchemeInputPort : public SchemeObject {
public:
    SchemeObject* read();
    SchemeObject* read_char();
    SchemeObject* peek_char();

    void close_file();
    bool eof();
private:
    SchemeInputPort(SchemeObjectCreator* creator);
    SchemeInputPort(SchemeObjectCreator* creator, const std::string& fname);

    std::ifstream input_file_;
    SchemeReader reader_;

    friend class SchemeObjectCreator;
};

class LoadProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    LoadProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

class ReadProcedure : public SchemePrimProcedure {
public:
    virtual SchemeObject* func(SchemeObject* args);
private:
    ReadProcedure(SchemeObjectCreator* creator) :
        SchemePrimProcedure(creator, 1) {}
    friend class SchemeObjectCreator;
};

//============================================================================
// Output
//============================================================================

class SchemeOutputPort : public SchemeObject {
public:
    void write(SchemeObject* obj);
    void write_char(SchemeObject* obj);
private:
    SchemeOutputPort();
    SchemeOutputPort(const std::string& fname);

    std::ofstream out_;
    SchemeWriter writer_;

    friend class SchemeObjectCreator;
};

#endif /* INPUT_OUTPUT_H_ */
