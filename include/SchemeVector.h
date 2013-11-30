#ifndef SCHEMEVECTOR_H_
#define SCHEMEVECTOR_H_

#include <vector>

#include "SchemeObject.h"

class SchemeVector : public SchemeObject {
public:
    std::vector<SchemeObject*>& data() {
        return objects_;
    }
    size_t length() {
        return objects_.size();
    }
    SchemeObject* at(size_t i) {
        return objects_[i];
    }
    void set(size_t i, SchemeObject* obj) {
        if (i < objects_.size()) {
            objects_[i] = obj;
        }
    }
private:
    SchemeVector(std::vector<SchemeObject*>& objects) :
        SchemeObject(VECTOR),
        objects_(objects) {}
    SchemeVector(size_t size, SchemeObject* fill) :
        SchemeObject(VECTOR),
        objects_(size, fill) {}

    std::vector<SchemeObject*> objects_;

    friend class SchemeObjectCreator;
};

#endif /* SCHEMEVECTOR_H_ */
