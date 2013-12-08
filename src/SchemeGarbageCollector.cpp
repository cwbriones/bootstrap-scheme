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

#include <vector>
#include <iostream>

#include "SchemeGarbageCollector.h"

#include "SchemeObject.h"
#include "SchemeVector.h"
#include "SchemeEnvironment.h"
#include "Procedures/SchemeCompoundProcedure.h"
#include "Environment.h"

SchemeGarbageCollector SchemeGarbageCollector::instance_;

SchemeGarbageCollector::SchemeGarbageCollector() {}

SchemeGarbageCollector& SchemeGarbageCollector::the_gc() {
    return instance_;
}

void SchemeGarbageCollector::add(SchemeObject* obj) {
    the_white_set_.insert(obj);
}

void SchemeGarbageCollector::add_from_environment(Environment* env) {
    if (checked_env_.find(env) == checked_env_.end()) {
        for (auto& binding : env->get_bindings()) {
            grey_object(binding.first);
            grey_object(binding.second);
        }
        checked_env_.insert(env);
    }
}

void SchemeGarbageCollector::collect() {
    while (!the_grey_set_.empty()) {
        // Grey all objects each object in the grey set
        // references
        for (auto iter = the_grey_set_.begin(); iter != the_grey_set_.end();) {
            // Grey all white objects this refers to
            follow(*iter);

            // Blacken this checked object
            the_black_set_.insert(*iter);
            iter = the_grey_set_.erase(iter);
        }
    }
    // White set now contains objects suitable for collection
    free();
    checked_env_.clear();
}

void SchemeGarbageCollector::follow(SchemeObject* obj) {
    switch (obj->type()) {
        case SchemeObject::PAIR:
            grey_object(obj->car());
            grey_object(obj->cdr());
            break;
        case SchemeObject::COMPPROCEDURE:
            grey_object(obj->to_comp_procedure()->body());
            grey_object(obj->to_comp_procedure()->params());
            // gross
            add_from_environment(obj->to_comp_procedure()->env().get());
            break;
        case SchemeObject::ENVIRONMENT:
            // This is also gross
            add_from_environment(obj->to_environment()->get().get());
            break;
        case SchemeObject::VECTOR:
            follow_vector(obj->to_vector());
            break;
        default:
            break;
    }
}

void SchemeGarbageCollector::follow_vector(SchemeVector* the_vector) {
    for (auto& obj : the_vector->data()) {
        grey_object(obj);
    }
}

void SchemeGarbageCollector::grey_object(SchemeObject* obj) {
    the_grey_set_.insert(obj);

    auto iter = the_white_set_.find(obj);
    if (iter != the_white_set_.end()) {
        the_white_set_.erase(iter);
    }
}

void SchemeGarbageCollector::free() {
    // Clear objects now without references in the white set
    for (auto& obj : the_white_set_) {
        if (obj->collectible()) {
            delete obj;
        }
    }
    the_white_set_.clear();

    // Move objects back to the white set to prepare for the next round
    for (auto& obj : the_black_set_) {
        the_white_set_.insert(obj);
    }

    the_black_set_.clear();
}

void SchemeGarbageCollector::free_remaining() {
    free();
    SchemeSymbol::clear_symbols();
}
