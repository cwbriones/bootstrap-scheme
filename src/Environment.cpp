#include "Environment.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

#include <iostream>

Environment::~Environment() {}

void Environment::define_variable_value(SchemeSymbol* symbol, SchemeObject* value) {
    Environment* env = this;

    while (env) {
        auto found = env->frame_bindings_.find(symbol->value());

        /*
         * Check if the symbol already exists.
         *
         * If the symbol is not found, we try to continue up
         * and if there is no way up we create it
         * in the initial frame
         */

        if (found == env->frame_bindings_.end()) {
            // Try to go up
            if (env->enclosing_) {
                env = env->enclosing_;
            } else {
                // Relent and create the binding
                this->frame_bindings_[symbol->value()] = value;
                return;
            }
        } else {
            found->second = value;
            return;
        }
    }
}

bool Environment::variable_is_defined(std::string var) {
    return frame_bindings_.find(var) != frame_bindings_.end();
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
        if (found == env->frame_bindings_.end()){
            if (env->enclosing_) {
                env = env->enclosing_;
            }
            break;
        } else {
            found->second = value;
            return true;
        }
    }
    return false;
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

Environment::Ptr Environment::extend(SchemeObject* vars, SchemeObject* vals) {
    
    Environment::Ptr extension(new Environment(this));

    while (!vars->is_empty_list()) {
        // Bind new variables
        extension->define_variable_value(
                vars->car()->to_symbol(),
                vals->car()
                );

        vals = vals->cdr();
        vars = vars->cdr();
    }
    return std::move(extension);
}

Environment* Environment::get_enclosing_env() {
    return enclosing_;
}
