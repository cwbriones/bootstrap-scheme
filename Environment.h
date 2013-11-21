#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>

#include "SchemeObject.h"

class SchemeObject;

class Environment {
public:
    Environment();
    Environment extend();

    bool define_variable_value(SchemeSymbol* symbol, SchemeObject* value);
    bool set_variable_value(SchemeSymbol* symbol, SchemeObject* value);

    SchemeObject* lookup_variable_value(SchemeSymbol* symbol);
    Environment* get_enclosing_env();
private:
    Environment* enclosing_ = nullptr;
    std::unordered_map<std::string, SchemeObject*> frame_bindings_;
};


#endif // ENVIRONMENT_H

