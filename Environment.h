#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>

class Object;

class Environment {
    public:
        Environment();
        void bind(Object* symbol, Object* value);
        Environment extend();
        Environment* get_enclosing_env();
        Object* get_value_of_symbol(Object* symbol);
    private:
        Environment* enclosing_;

        std::unordered_map<std::string, Object*> bindings_;
};


#endif // ENVIRONMENT_H

