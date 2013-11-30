#ifndef SCHEMEVECTOR_H_
#define SCHEMEVECTOR_H_

#include <vector>

#include "SchemeObject.h"

class SchemeVector : public SchemeObject {
public:
    std::vector<SchemeObject*>& data() {
        return objects_;
    }
private:
    SchemeVector(size_t size, SchemeObject* fill) :
        SchemeObject(VECTOR),
        objects_(size, fill) {}

    std::vector<SchemeObject*> objects_;

    friend class SchemeObjectCreator;
};

#endif /* SCHEMEVECTOR_H_ */
