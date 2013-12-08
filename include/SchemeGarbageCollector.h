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
