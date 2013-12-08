/*
 * Copyright (C) Christian Briones, 2013
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "Environment.h"
#include "SchemeObject.h"
#include "SchemeObjectCreator.h"

#include <iostream>

Environment::Ptr Environment::the_global_environment_ = 
    std::make_shared<Environment>();

Environment::Environment(Environment::Ptr enclosing,
            SchemeObject* vars, SchemeObject* vals) 
{
    enclosing_ = enclosing;

    // List of arguments stored into the only variable
    // Otherwise pair off each var/val into a binding
    // TODO: Check that the correct number of arguments was passed

    while (!vars->is_empty_list()) {
        // Bind new variables
        if (vars->is_symbol()) {
            define_variable_value(
                    vars->to_symbol(),
                    vals);
            break;
        } else {
            define_variable_value(
                    vars->car()->to_symbol(),
                    vals->car()
                );
        }

        vals = vals->cdr();
        vars = vars->cdr();
    }
}

Environment::~Environment() {}

void Environment::define_variable_value(SchemeSymbol* symbol, SchemeObject* value) {
    auto found = frame_bindings_.find(symbol);

    if (found == frame_bindings_.end()) {
        frame_bindings_[symbol] = value;
    } else {
        found->second = value;
    }
}

bool Environment::variable_is_defined(SchemeSymbol* var) {
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
        auto found = env->frame_bindings_.find(symbol);
        if (found != env->frame_bindings_.end()){
            found->second = value;
            return true;
        }
        env = env->enclosing();
    }
    return false;
}


const std::unordered_map<SchemeSymbol*, SchemeObject*>& Environment::get_bindings() const {
    return frame_bindings_;
}

SchemeObject* Environment::lookup_variable_value(SchemeSymbol* symbol) {

    Environment* env = this;

    while (env){
        auto found = env->frame_bindings_.find(symbol);
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
