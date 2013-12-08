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
