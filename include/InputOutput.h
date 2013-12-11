/*
 * Copyright (C) Christian Briones, 2013
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INPUT_OUTPUT_H_
#define INPUT_OUTPUT_H_

#include <string>
#include <fstream>

#include "SchemeObject.h"
#include "SchemeWriter.h"
#include "Reader.h"

#include "Procedures/SchemePrimProcedure.h"

//============================================================================
// Input
//============================================================================

class SchemeObjectCreator;

class SchemeInputPort : public SchemeObject {
public:
    SchemeObject* read();
    SchemeObject* read_char();
    SchemeObject* peek_char();

    void close_file();
    bool eof();

    const std::string& filename() const {
        return fname_;
    }
private:
    SchemeInputPort(SchemeObjectCreator* creator);
    SchemeInputPort(SchemeObjectCreator* creator, const std::string& fname);

    std::ifstream input_file_;
    SchemeReader reader_;
    std::string fname_;

    friend class SchemeObjectCreator;
};

namespace InputProcedures {

SchemeObject* load(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* read(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* read_char(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* peek_char(SchemeObject* args, SchemeObjectCreator* creator);

}   /* namespace InputProcedures */

//============================================================================
// Output
//============================================================================

class SchemeOutputPort : public SchemeObject {
public:
    void write(SchemeObject* obj);
    void write_char(SchemeObject* obj);

    void close_file();

    const std::string& filename() const {
        return fname_;
    }
private:
    SchemeOutputPort();
    SchemeOutputPort(const std::string& fname);

    std::ofstream out_;
    SchemeWriter writer_;
    std::string fname_;

    friend class SchemeObjectCreator;
};

namespace OutputProcedures {

SchemeObject* write(SchemeObject* args, SchemeObjectCreator* creator);
SchemeObject* write_char(SchemeObject* args, SchemeObjectCreator* creator);

}

#endif /* INPUT_OUTPUT_H_ */
