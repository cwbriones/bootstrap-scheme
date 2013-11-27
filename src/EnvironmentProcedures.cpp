#include "Procedures/EnvironmentProcedures.h"
#include "SchemeObjectCreator.h"

SchemeObject* NullEnvironmentProcedure::func(SchemeObject* args) {
    return obj_creator_->make_null_environment();
}

SchemeObject* InteractionEnvironmentProcedure::func(SchemeObject* args) {
    return obj_creator_->make_interaction_environment();
}

SchemeObject* StandardEnvironmentProcedure::func(SchemeObject* args) {
    return obj_creator_->make_environment();
}
