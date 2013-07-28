#include "Object.h"
#include "Environment.h"

Environment::Environment(){}

void Environment::bind(Object* symbol, Object* value){
    bindings_[ symbol->data.symbol.value ] = value;
}

bool Environment::set(Object* symbol, Object* value){
    Environment* env = this;

    while (env){
        if (bindings_.find(symbol->data.symbol.value) == bindings_.end()){
            env = env->enclosing_;
        } else {
            bind(symbol, value);
            return true;
        }
    }

    return false;
}

Object* Environment::get_value_of_symbol(Object* symbol){
    Environment* env = this;

    while (env){
        if (env->bindings_.find(symbol->data.symbol.value) == env->bindings_.end()){
            env = env->enclosing_;
        } else {
            return env->bindings_[symbol->data.symbol.value];
        }
    }

    return nullptr;
}
