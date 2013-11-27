#ifndef SCHEME_GARBAGE_COLLECTOR_H_
#define SCHEME_GARBAGE_COLLECTOR_H_

#include <memory>
#include <unordered_set>

class SchemeObject;
class Environment;

class SchemeGarbageCollector {
public:
    typedef std::unique_ptr<SchemeGarbageCollector> Ptr;
    static SchemeGarbageCollector& the_gc();

    void add(SchemeObject* obj);
    void add_from_environment(Environment* env);
    void collect();
private:
    static Ptr instance_;

    SchemeGarbageCollector();
    void follow(SchemeObject* obj);
    void grey_object(SchemeObject* obj);
    void free();

    std::unordered_set<SchemeObject*> the_white_set_;
    std::unordered_set<SchemeObject*> the_grey_set_;
    std::unordered_set<SchemeObject*> the_black_set_;
};

#endif /* SCHEME_GARBAGE_COLLECTOR_H_ */
