#ifndef SCHEME_WRITER_H
#define SCHEME_WRITER_H

#include <iostream>

class SchemeObject;

class SchemeWriter {
public:
    SchemeWriter();
    SchemeWriter(std::ostream& out);

    void write(SchemeObject* obj);
    void write_char(SchemeObject* obj);
private:
    void write_pair(SchemeObject* obj);
    void write_vector(SchemeObject* obj);
    void write_string(SchemeObject* obj);

    std::ostream& out_;
};

#endif /* SCHEME_WRITER_H */
