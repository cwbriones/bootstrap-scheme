#include <vector>
#include <iostream>

#include "SchemeGarbageCollector.h"

#include "SchemeObject.h"
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
        default:
            break;
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
