#include "SchemeGarbageCollector.h"

#include "SchemeObject.h"
#include "SchemeEnvironment.h"
#include "Procedures/SchemeCompoundProcedure.h"
#include "Environment.h"

SchemeGarbageCollector::Ptr SchemeGarbageCollector::instance_ = nullptr;

SchemeGarbageCollector::SchemeGarbageCollector() {}

SchemeGarbageCollector& SchemeGarbageCollector::the_gc() {
    if (!instance_) {
        instance_.reset(new SchemeGarbageCollector());
    }
    return *instance_;
}

void SchemeGarbageCollector::add(SchemeObject* obj) {
    the_white_set_.insert(obj);
}

void SchemeGarbageCollector::add_from_environment(Environment* env) {
    for (auto& binding : env->get_bindings()) {
        grey_object(binding.second);
    }
}

void SchemeGarbageCollector::collect() {
    while (!the_grey_set_.empty()) {

    }
    free();
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
            add_from_environment(obj->to_comp_procedure()->env());
            break;
        case SchemeObject::ENVIRONMENT:
            // This is gross
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
    for (auto& obj : the_white_set_) {
        delete obj;
    }
    the_white_set_.clear();

    for (auto& obj : the_black_set_) {
        the_white_set_.insert(obj);
    }
    the_black_set_.clear();
}
