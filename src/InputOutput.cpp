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

SchemeInputPort::SchemeInputPort(
        SchemeObjectCreator* creator, 
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

namespace InputProcedures {
    
SchemeObject* load(SchemeObject* args, SchemeObjectCreator* creator) {
    std::string fname = args->car()->to_string()->value();
    std::ifstream input_stream(fname);

    if (input_stream) {
        SchemeReader reader(creator, input_stream);
        bool success =
            reader.load_into_environment(Environment::get_global_environment());
        if (success) {
            return creator->make_symbol(fname.append(" loaded."));
        }
    }
    return creator->make_symbol("failed to load file " + fname);
}

SchemeObject* read(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeInputPort* port;

    if (args->is_empty_list()) {
        port = creator->make_standard_input()->to_input_port();
    } else {
        port = args->car()->to_input_port();
    }

    return port->read();
}

SchemeObject* read_char(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeInputPort* port;

    if (args->is_empty_list()) {
        // Default to console
        port = creator->make_standard_input()->to_input_port();
    } else {
        port = args->car()->to_input_port();
    }

    return port->read_char();
}

SchemeObject* peek_char(SchemeObject* args, SchemeObjectCreator* creator) {
    SchemeInputPort* port;

    if (args->is_empty_list()) {
        port = creator->make_standard_input()->to_input_port();
    } else {
        port = args->car()->to_input_port();
    }

    return port->peek_char();
}

} /* namespace InputProcedures */

//============================================================================
// Output
//============================================================================

SchemeOutputPort::SchemeOutputPort() :
    SchemeObject(SchemeObject::OUTPUT_PORT),
    out_(),
    writer_()
{}

SchemeOutputPort::SchemeOutputPort(const std::string& fname) :
    SchemeObject(SchemeObject::OUTPUT_PORT),
    out_(fname),
    writer_(out_)
{}

void SchemeOutputPort::write(SchemeObject* obj) {
    writer_.write(obj);
}

void SchemeOutputPort::write_char(SchemeObject* obj) {
    writer_.write_char(obj);
}

void SchemeOutputPort::close_file() {
    out_.close();
}
