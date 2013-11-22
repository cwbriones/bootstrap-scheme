#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>

#include "SchemeObject.h"

class SchemeObject;
class SchemeObjectCreator;

class Environment {
public:
    typedef std::shared_ptr<Environment> Ptr;

    Environment() :
        enclosing_(nullptr) {}

    Environment(Environment* enclosing) : 
        enclosing_(enclosing) {}
    ~Environment();

    Environment::Ptr extend(SchemeObject* vars, SchemeObject* vals);

    void define_variable_value(SchemeSymbol* symbol, SchemeObject* value);
    bool set_variable_value(SchemeSymbol* symbol, SchemeObject* value);
    bool variable_is_defined(std::string var);

    SchemeObject* lookup_variable_value(SchemeSymbol* symbol);
    Environment* get_enclosing_env();
private:
    Environment* enclosing_;
    std::unordered_map<std::string, SchemeObject*> frame_bindings_;
};


#endif // ENVIRONMENT_H

