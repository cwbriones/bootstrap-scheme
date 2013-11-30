#ifndef INPUT_OUTPUT_H_
#define INPUT_OUTPUT_H_

#include <string>
#include <fstream>

#include "SchemeObject.h"
#include "Reader.h"

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

#endif /* INPUT_OUTPUT_H_ */
