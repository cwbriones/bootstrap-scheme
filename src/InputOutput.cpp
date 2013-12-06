#include <iostream>
#include <fstream>

#include "SchemeObject.h"
#include "SchemeObjectCreator.h"
#include "InputOutput.h"

//============================================================================
// SchemeInputPort
//============================================================================

SchemeInputPort::SchemeInputPort(SchemeObjectCreator* creator)  :
    SchemeObject(INPUT_PORT),
    reader_(creator) {}

SchemeInputPort::SchemeInputPort(SchemeObjectCreator* creator, 
        const std::string& fname) :
    SchemeObject(INPUT_PORT),
    input_file_(fname),
    reader_(creator, input_file_) {}

SchemeObject* SchemeInputPort::read() 
{
    return reader_.read();
}

SchemeObject* SchemeInputPort::read_char() {
    return reader_.read_char();
}

SchemeObject* SchemeInputPort::peek_char() {
    return reader_.peek_char();
}

bool SchemeInputPort::eof() {
    if (input_file_.is_open()) {
        return input_file_.eof();
    }
    return false;
}

void SchemeInputPort::close_file() {
    input_file_.close();
}

#include <iostream>

SchemeObject* LoadProcedure::func(SchemeObject* args) {
    std::string fname = args->car()->to_string()->value();
    std::ifstream input_stream(fname);

    if (input_stream) {
        SchemeReader reader(obj_creator_, input_stream);
        bool success =
            reader.load_into_environment(Environment::get_global_environment());
        if (success) {
            return obj_creator_->make_symbol(fname.append(" loaded."));
        }
    }
    return obj_creator_->make_symbol("failed to load file " + fname);
}

SchemeObject* ReadProcedure::func(SchemeObject* args) {

    return obj_creator_->make_symbol("okay");
}

//============================================================================
// Output
//============================================================================

SchemeOutputPort::SchemeOutputPort() :
    SchemeObject(SchemeObject::OUTPUT_PORT),
    out_(),
    writer_()
{}

SchemeOutputPort::SchemeOutputPort(const std::string& fname) :
    out_(fname),
    writer_(out_)
{}

void SchemeOutputPort::write(SchemeObject* obj) {
    writer_.write(obj);
}

void SchemeOutputPort::write_char(SchemeObject* obj) {
    writer_.write_char(obj);
}
