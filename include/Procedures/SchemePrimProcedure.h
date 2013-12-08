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

#ifndef SCHEMEPRIMPROCEDURE_H_
#define SCHEMEPRIMPROCEDURE_H_

#include "SchemeObject.h"
#include <functional>

class SchemeObjectCreator;

class SchemePrimProcedure : public SchemeObject {
public:
    typedef std::function<SchemeObject*(SchemeObject*, SchemeObjectCreator*)>
        procedure_t;

    SchemePrimProcedure(SchemeObjectCreator* creator, 
            procedure_t func,
            int argc=-1,
            bool apply=false,
            bool eval=false) :
            
        SchemeObject(PRIMPROCEDURE),
        argc_(argc),
        apply_(apply),
        eval_(eval)
    {
        func_ = std::bind(func, std::placeholders::_1, creator);
    }

    SchemeObject* apply(SchemeObject* args) {
        return func_(args);
    }
    
    bool is_apply() { return apply_; }
    bool is_eval() { return eval_; }
private:
    void check_arg_length(SchemeObject* args);
    std::function<SchemeObject*(SchemeObject*)> func_;
    int argc_;

    const bool apply_ = false;
    const bool eval_ = false;

    friend class SchemeObjectCreator;
};

//============================================================================
// Arithmetic Operators
//============================================================================

namespace ArithmeticProcedures {

SchemeObject* add(SchemeObject*, SchemeObjectCreator*);
SchemeObject* sub(SchemeObject*, SchemeObjectCreator*);
SchemeObject* mul(SchemeObject*, SchemeObjectCreator*);
SchemeObject* div(SchemeObject*, SchemeObjectCreator*);
    
SchemeObject* equals(SchemeObject*, SchemeObjectCreator*);
SchemeObject* less_than(SchemeObject*, SchemeObjectCreator*);
SchemeObject* greater_than(SchemeObject*, SchemeObjectCreator*);

SchemeObject* quotient(SchemeObject*, SchemeObjectCreator*);
SchemeObject* modulo(SchemeObject*, SchemeObjectCreator*);

} /* namespace ArithmeticProcedures */

#include <random>

namespace MiscProcedure {

SchemeObject* null_proc(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* random(SchemeObject* args, SchemeObjectCreator* creator);

}   /* namespace MiscProcedure */

//============================================================================
// List Operations
//============================================================================

namespace ListProcedures {

SchemeObject* length(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* cons(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* car(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* cdr(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* set_car(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* set_cdr(SchemeObject* args, SchemeObjectCreator* creator);
    
} /* namespace ListProcedures */

namespace PredicateProcedures {

SchemeObject* list_check(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* type_check(
        SchemeObject* args, 
        SchemeObjectCreator* creator,
        uint32_t type);

SchemePrimProcedure::procedure_t create_type_check(uint32_t type);

SchemeObject* obj_equiv(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* boolean_not(SchemeObject* args, SchemeObjectCreator* creator);

} /* namespace PredicateProcedures */

#endif /* SCHEMEPRIMPROCEDURE_H_ */
