#include "Environment.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

#include <iostream>

Environment::Environment(Environment::Ptr enclosing,
            SchemeObject* vars, SchemeObject* vals) 
{
    enclosing_ = enclosing;

    while (!vars->is_empty_list()) {
        // Bind new variables
        define_variable_value(
                vars->car()->to_symbol(),
                vals->car()
            );

        vals = vals->cdr();
        vars = vars->cdr();
    }
}

Environment::~Environment() {}

void Environment::define_variable_value(SchemeSymbol* symbol, SchemeObject* value) {
    auto found = frame_bindings_.find(symbol->value());

    if (found == frame_bindings_.end()) {
        frame_bindings_[symbol->value()] = value;
    } else {
        found->second = value;
    }
}

bool Environment::variable_is_defined(std::string var) {
    Environment* env = this;
    while (env) {
        if (env->frame_bindings_.find(var) != env->frame_bindings_.end()) {
            return true;
        } else {
            env = env->enclosing();
        }
    }
    return false;
}

bool Environment::set_variable_value(SchemeSymbol* symbol, SchemeObject* value) {

    Environment* env = this;

    /*
     * Check if the symbol already exists.
     *
     * If the symbol is not found, we try to continue up
     * and if there is no way up we relent and return failure
     */
    while (env){
        auto found = env->frame_bindings_.find(symbol->value());
        if (found != env->frame_bindings_.end()){
            found->second = value;
            return true;
        }
        env = env->enclosing();
    }
    return false;
}

SchemeObject* Environment::lookup_variable_value(SchemeSymbol* symbol) {

    Environment* env = this;

    while (env){
        auto found = env->frame_bindings_.find(symbol->value());
        if (found != env->frame_bindings_.end()){
            return found->second;
        }
        env = env->enclosing();
    }
    return nullptr;
}

Environment* Environment::enclosing() {
    return enclosing_.get();
}
