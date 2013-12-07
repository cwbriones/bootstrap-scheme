#ifndef SCHEMEOBJECTCREATOR_H_
#define SCHEMEOBJECTCREATOR_H_

#include <string>
#include <vector>

#include "SchemeEnvironment.h"
#include "Procedures/SchemePrimProcedure.h"

#include "InputOutput.h"
#include "Environment.h"

class SchemeObject;

class SchemeObjectCreator {
public:
    SchemeObjectCreator();

    SchemeObject* make_unspecified();
    SchemeObject* make_fixnum(long value);
    SchemeObject* make_flonum(double value);
    SchemeObject* make_boolean(bool value);
    SchemeObject* make_character(char value);
    SchemeObject* make_string(std::string value);
    SchemeObject* make_pair(SchemeObject* car, SchemeObject* cdr);
    SchemeObject* make_symbol(std::string value);
    SchemeObject* make_vector(size_t size, SchemeObject* init);
    SchemeObject* make_vector(std::vector<SchemeObject*>& objects);
    SchemeObject* make_empty_list();

    SchemeObject* make_prim_procedure(
            SchemePrimProcedure::procedure_t func, 
            int argc=-1,
            bool apply=false,
            bool eval=false);

    void make_procedure_in_env(
            Environment* env,
            const std::string& var,
            SchemePrimProcedure::procedure_t func,
            int argc=-1);

    SchemeObject* make_environment();
    SchemeObject* make_interaction_environment();
    SchemeObject* make_null_environment();

    // Input/Output
    SchemeObject* make_input_port(const std::string& fname);
    SchemeObject* make_output_port(const std::string& fname);
    SchemeObject* make_standard_input();
    SchemeObject* make_standard_output();

    SchemeObject* make_tagged_list(std::string tag, SchemeObject* obj);
    SchemeObject* make_tagged_list(
            std::string tag, SchemeObject* obj1, SchemeObject* obj2);

    SchemeObject* make_comp_procedure(
            Environment::Ptr& env, SchemeObject* params, SchemeObject* body);
    
    Environment* the_interaction_env() {
        return the_interaction_env_.get().get();
    }
    void setup_environment(Environment* env);
private:
    void init_keywords();
    void init_type_predicates(Environment* env);
    void init_type_conversions(Environment* env);
    void init_input_output(Environment* env);

    // -----------------------------
    // Single Instance Objects
    // -----------------------------
    
    SchemeEnvironment the_interaction_env_;
    SchemeInputPort standard_input_;
    SchemeOutputPort standard_output_;
};

#endif /* SCHEMEOBJECTCREATOR_H_ */
