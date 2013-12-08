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

#ifndef SCHEME_EVALUATOR_H_
#define SCHEME_EVALUATOR_H_

#include "Environment.h"

class SchemeObject;
class SchemeCreator;

class SchemeEvaluator {
public:
    SchemeEvaluator(SchemeObjectCreator* creator) :
        obj_creator_(creator) {}

    SchemeObject* eval(SchemeObject* exp, Environment::Ptr env);
    SchemeObject* eval_in_global_env(SchemeObject* exp);
private:
    std::string make_string(
            const std::string& prefix, size_t suffix, size_t maxlen);

    SchemeObject* cons(SchemeObject* car, SchemeObject* cdr);
    SchemeObject* get_value_of_args(SchemeObject* args, Environment::Ptr env);
    
    SchemeObject* eval_let_form(SchemeObject* args, Environment::Ptr env);
    SchemeObject* convert_let_star(SchemeObject* args, Environment::Ptr env);
    SchemeObject* convert_named_let(SchemeObject* args, Environment::Ptr env);
    SchemeObject* convert_letrec(SchemeObject* args);

    SchemeObject* prepare_apply_args(SchemeObject* args_to_apply);

    SchemeObjectCreator* obj_creator_;
};

#endif /* SCHEME_EVALUATOR_H_ */
