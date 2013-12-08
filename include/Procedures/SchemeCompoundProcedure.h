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

#ifndef SCHEME_COMPOUND_PROCEDURE_H_
#define SCHEME_COMPOUND_PROCEDURE_H_

#include "Environment.h"

class SchemeCompoundProcedure : public SchemeObject {
public:
    Environment::Ptr env() {
        return env_;
    }

    SchemeObject* params() {
        return params_;
    }

    SchemeObject* body() {
        return body_;
    }
private:
    SchemeCompoundProcedure(
            Environment::Ptr& env, SchemeObject* params, SchemeObject* body) :
        SchemeObject(COMPPROCEDURE),
        params_(params),
        body_(body)
    {
        env_ = env;
    }

    SchemeObject* params_;
    SchemeObject* body_;

    Environment::Ptr env_;

    friend class SchemeObjectCreator;
};

#endif /* SCHEME_COMPOUND_PROCEDURE_H_ */
