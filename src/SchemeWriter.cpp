#include <iostream>
#include <string>
#include <vector>

#include "SchemeObject.h"
#include "SchemeWriter.h"
#include "SchemeVector.h"

SchemeWriter::SchemeWriter() : out_(std::cout) 
{}

SchemeWriter::SchemeWriter(std::ostream& out) : out_(out)
{}

void SchemeWriter::write(SchemeObject* obj) {

	switch (obj->type()){
        case SchemeObject::UNSPECIFIED:
            out_ << "#<unspecified>";
            break;
		case SchemeObject::FIXNUM:
			out_ << obj->fixnum_value();
			break;
        case SchemeObject::FLONUM:
            out_ <<  obj->flonum_value();
            break;
        case SchemeObject::BOOLEAN:
            out_ << '#' << ((obj->is_true_obj()) ? 't' : 'f');
            break;
        case SchemeObject::CHARACTER:
            out_ << "#\\";

            switch(obj->char_value()) {
                case '\n':
                    out_ << "newline";
                    break;
                case ' ':
                    out_ << "space";
                    break;
                case '\t':
                    out_ << "tab";
                    break;
                default:
                    out_ << obj->char_value();
            }
            break;
        case SchemeObject::STRING:
            write_string(obj);
            break;
        case SchemeObject::PAIR:
            out_ << "(";
            write_pair(obj);
            out_ << ")";
            break;
        case SchemeObject::EMPTY_LIST:
            out_ << "()";
            break;
        case SchemeObject::SYMBOL:
            out_ << obj->to_symbol()->value();
            break;
        case SchemeObject::NEWPROCEDURE:
        case SchemeObject::PRIMPROCEDURE:
            out_ << "#<compiled-procedure>";
            break;
        case SchemeObject::COMPPROCEDURE:
            out_ << "#<compound-procedure>";
            break;
        case SchemeObject::ENVIRONMENT:
            out_ << "#<environment>";
            break;
        case SchemeObject::VECTOR:
            out_ << "#(";
            write_vector(obj);
            out_ << ")";
            break;
		default:
			std::cerr << "unknown type, cannot write." << std::endl;
			exit(1);
	}
}

void SchemeWriter::write_char(SchemeObject* obj) {
    if (obj->is_character()) {
        out_ << obj->char_value();
    }
    // There should be an error if otherwise
}

void SchemeWriter::write_pair(SchemeObject* pair) {
    SchemeObject* car_obj = pair->car();
    SchemeObject* cdr_obj = pair->cdr();

    write(car_obj);

    if (cdr_obj->is_pair()){
        out_ << " ";
        write_pair(cdr_obj);
        return;
    } 
    else if (cdr_obj->is_empty_list()){
        return;
    } 
    else {
        out_ << " . ";
        write(cdr_obj);
    }
}

void SchemeWriter::write_vector(SchemeObject* obj) {
    std::vector<SchemeObject*>& the_vector = obj->to_vector()->data();

    for (int i = 0; i < the_vector.size() - 1; i++) {
        write(the_vector[i]);
        out_ << " ";
    }
    write(the_vector.back());
 }

void SchemeWriter::write_string(SchemeObject* obj) {
    const std::string& str = obj->to_string()->value();
        
    out_ << '\"';
    auto iter = str.cbegin();

    while (iter != str.cend()){
        switch(*iter){
            case '\n':
                out_ << "\\n";
                break;
            case '\t':
                out_ << "\\t";
                break;
            case '\"':
                out_ << "\\\"";
                break;
            default:
                out_ << *iter;
        }
        iter++;
    }
    out_ << '\"';
}
