#ifndef SCHEME_GARBAGE_COLLECTOR_H_
#define SCHEME_GARBAGE_COLLECTOR_H_

#include <unordered_set>

class SchemeObject;
class SchemeVector;
class Environment;

class SchemeGarbageCollector {
public:
    static SchemeGarbageCollector& the_gc();

    void add(SchemeObject* obj);
    void add_from_environment(Environment* env);
    void collect();

    void free_remaining();
private:
    static SchemeGarbageCollector instance_;

    SchemeGarbageCollector();
    void follow(SchemeObject* obj);
    void follow_vector(SchemeVector* the_vector);
    void grey_object(SchemeObject* obj);
    void free();

    std::unordered_set<SchemeObject*> the_white_set_;
    std::unordered_set<SchemeObject*> the_grey_set_;
    std::unordered_set<SchemeObject*> the_black_set_;

    std::unordered_set<Environment*> checked_env_;
};

#endif /* SCHEME_GARBAGE_COLLECTOR_H_ */
