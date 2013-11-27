#ifndef SCHEME_ENVIRONMENT_H
#define SCHEME_ENVIRONMENT_H

#include "SchemeObject.h"
#include "Environment.h"

class SchemeEnvironment : public SchemeObject {
public:
    SchemeEnvironment(Environment::Ptr env) :
        SchemeObject(Type::ENVIRONMENT),
        env_(env) {}

    Environment* get() {
        return env_.get();
    }
private:
    Environment::Ptr env_;
    friend class SchemeObjectCreator;
};

#endif // SCHEME_ENVIRONMENT_H
