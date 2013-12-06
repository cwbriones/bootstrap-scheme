#ifndef SCHEME_WRITER_H
#define SCHEME_WRITER_H

#include <iostream>

class SchemeObject;

class SchemeWriter {
public:
    SchemeWriter();

    void write(SchemeObject* obj);
private:
    void write_pair(SchemeObject* obj);
    void write_vector(SchemeObject* obj);
    void write_string(SchemeObject* obj);

    std::ostream& output_stream_;
};

#endif /* SCHEME_WRITER_H */
