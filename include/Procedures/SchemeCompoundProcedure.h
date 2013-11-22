#ifndef SCHEME_COMPOUND_PROCEDURE_H_
#define SCHEME_COMPOUND_PROCEDURE_H_

#include "Environment.h"

class SchemeCompoundProcedure : public SchemeObject {
public:
    Environment* env() {
        return env_.get();
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
