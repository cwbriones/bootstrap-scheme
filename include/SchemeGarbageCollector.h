#ifndef SCHEME_GARBAGE_COLLECTOR_H_
#define SCHEME_GARBAGE_COLLECTOR_H_

#include <list>
#include <unordered_map>

class SchemeObject;
class Environment;

class SchemeGarbageCollector {
public:
    SchemeGarbageCollector& the_gc();
    void set_global_environment(Environment* env);

    SchemeObject* get_object();
private:
    void mark_all_objects();
    void sweep();
    void reset_mark();

    SchemeGarbageCollector();

    SchemeGarbageCollector instance_;
    int current_mark_ = 0;

    std::vector<SchemeObject*> white_set_;
    std::vector<SchemeObject*> grey_set_;
    std::vector<SchemeObject*> black_set_;

    Environment* the_global_env = nullptr;
};

#endif /* SCHEME_GARBAGE_COLLECTOR_H_ */
