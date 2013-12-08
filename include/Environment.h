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

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>

#include "SchemeObject.h"

class SchemeObject;
class SchemeSymbol;
class SchemeObjectCreator;

class Environment {
public:
    typedef std::shared_ptr<Environment> Ptr;

    Environment() :
        enclosing_(nullptr) {}

    Environment(Environment::Ptr enclosing, 
            SchemeObject* vars, SchemeObject* vals);

    ~Environment();

    static Environment::Ptr get_global_environment() {
        return the_global_environment_;
    }

    void define_variable_value(SchemeSymbol* symbol, SchemeObject* value);
    bool set_variable_value(SchemeSymbol* symbol, SchemeObject* value);
    bool variable_is_defined(SchemeSymbol* var);

    const std::unordered_map<SchemeSymbol*, SchemeObject*>& get_bindings() const;

    SchemeObject* lookup_variable_value(SchemeSymbol* symbol);
    Environment* enclosing();
private:
    Environment::Ptr enclosing_;
    std::unordered_map<SchemeSymbol*, SchemeObject*> frame_bindings_;

    static Environment::Ptr the_global_environment_;
};


#endif // ENVIRONMENT_H

