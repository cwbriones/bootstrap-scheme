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

#ifndef SCHEME_ENVIRONMENT_H
#define SCHEME_ENVIRONMENT_H

#include "SchemeObject.h"
#include "Environment.h"

class SchemeEnvironment : public SchemeObject {
public:
    SchemeEnvironment(Environment::Ptr env) :
        SchemeObject(Type::ENVIRONMENT),
        env_(env) {}

    Environment::Ptr get() {
        return env_;
    }
private:
    Environment::Ptr env_;
    friend class SchemeObjectCreator;
};

#endif // SCHEME_ENVIRONMENT_H
