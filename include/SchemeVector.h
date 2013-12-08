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

#ifndef SCHEMEVECTOR_H_
#define SCHEMEVECTOR_H_

#include <vector>

#include "SchemeObject.h"

class SchemeVector : public SchemeObject {
public:
    std::vector<SchemeObject*>& data() {
        return objects_;
    }
    size_t length() {
        return objects_.size();
    }
    SchemeObject* at(size_t i) {
        return objects_[i];
    }
    void set(size_t i, SchemeObject* obj) {
        if (i < objects_.size()) {
            objects_[i] = obj;
        }
    }
private:
    SchemeVector(std::vector<SchemeObject*>& objects) :
        SchemeObject(VECTOR),
        objects_(objects) {}
    SchemeVector(size_t size, SchemeObject* fill) :
        SchemeObject(VECTOR),
        objects_(size, fill) {}

    std::vector<SchemeObject*> objects_;

    friend class SchemeObjectCreator;
};

#endif /* SCHEMEVECTOR_H_ */
