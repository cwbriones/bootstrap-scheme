#include "Environment.h"
#include "SchemeObject.h"

Environment::Environment() {}

bool Environment::define_variable_value(SchemeSymbol* symbol, SchemeObject* value) {

    Environment* env = this;

    while (env){
        auto found = env->frame_bindings_.find(symbol->value());

        if (found == frame_bindings_.end()){
            env = env->enclosing_;
        } else {
            set_variable_value(symbol, value);
            return true;
        }
    }

    return false;
}

bool Environment::set_variable_value(SchemeSymbol* symbol, SchemeObject* value) {
    auto found = frame_bindings_.find(symbol->value());
    if (found == frame_bindings_.end()) {
        return false;
    }

    found->second = value;

    return true;
}

SchemeObject* Environment::lookup_variable_value(SchemeSymbol* symbol) {

    Environment* env = this;

    while (env){
        auto found = env->frame_bindings_.find(symbol->value());

        if (found == env->frame_bindings_.end()){
            env = env->enclosing_;
        } else {
            return env->frame_bindings_[symbol->value()];
        }
    }

    return nullptr;
}
