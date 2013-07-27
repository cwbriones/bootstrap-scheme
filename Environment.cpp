#include "Object.h"
#include "Environment.h"

Environment::Environment(){}

void Environment::bind(Object* symbol, Object* value){
    bindings_[ symbol->data.symbol.value ] = value;
}

Object* Environment::get_value_of_symbol(Object* symbol){
    if (bindings_.find(symbol->data.symbol.value) == bindings_.end()){
        if (enclosing_ != nullptr){
            return enclosing_->get_value_of_symbol(symbol);
        } else {
            return nullptr;
        }
    } else {
        return bindings_[symbol->data.symbol.value];
    }
}
