#include <iostream>
#include <string>
#include <vector>

#include "SchemeObject.h"
#include "SchemeWriter.h"
#include "SchemeVector.h"

SchemeWriter::SchemeWriter() : output_stream_(std::cout)
{}

void SchemeWriter::write(SchemeObject* obj) {

	switch (obj->type()){
        case SchemeObject::UNSPECIFIED:
            std::cout << "#<unspecified>";
            break;
		case SchemeObject::FIXNUM:
			std::cout << obj->fixnum_value();
			break;
        case SchemeObject::FLONUM:
            std::cout <<  obj->flonum_value();
            break;
        case SchemeObject::BOOLEAN:
            std::cout << '#' << ((obj->is_true_obj()) ? 't' : 'f');
            break;
        case SchemeObject::CHARACTER:
            std::cout << "#\\";

            switch(obj->char_value()) {
                case '\n':
                    std::cout << "newline";
                    break;
                case ' ':
                    std::cout << "space";
                    break;
                case '\t':
                    std::cout << "tab";
                    break;
                default:
                    std::cout << obj->char_value();
            }
            break;
        case SchemeObject::STRING:
            write_string(obj);
            break;
        case SchemeObject::PAIR:
            std::cout << "(";
            write_pair(obj);
            std::cout << ")";
            break;
        case SchemeObject::EMPTY_LIST:
            std::cout << "()";
            break;
        case SchemeObject::SYMBOL:
            std::cout << obj->to_symbol()->value();
            break;
        case SchemeObject::PRIMPROCEDURE:
            std::cout << "#<compiled-procedure>";
            break;
        case SchemeObject::COMPPROCEDURE:
            std::cout << "#<compound-procedure>";
            break;
        case SchemeObject::ENVIRONMENT:
            std::cout << "#<environment>";
            break;
        case SchemeObject::VECTOR:
            std::cout << "#(";
            write_vector(obj);
            std::cout << ")";
            break;
		default:
			std::cerr << "unknown type, cannot write." << std::endl;
			exit(1);
	}
}

void SchemeWriter::write_pair(SchemeObject* pair) {
    SchemeObject* car_obj = pair->car();
    SchemeObject* cdr_obj = pair->cdr();

    write(car_obj);

    if (cdr_obj->is_pair()){
        std::cout << " ";
        write_pair(cdr_obj);
        return;
    } 
    else if (cdr_obj->is_empty_list()){
        return;
    } 
    else {
        std::cout << " . ";
        write(cdr_obj);
    }
}

void SchemeWriter::write_vector(SchemeObject* obj) {
    std::vector<SchemeObject*>& the_vector = obj->to_vector()->data();

    for (int i = 0; i < the_vector.size() - 1; i++) {
        write(the_vector[i]);
        std::cout << " ";
    }
    write(the_vector.back());
 }

void SchemeWriter::write_string(SchemeObject* obj) {
    const std::string& str = obj->to_string()->value();
        
    std::cout << '\"';
    auto iter = str.cbegin();

    while (iter != str.cend()){
        switch(*iter){
            case '\n':
                std::cout << "\\n";
                break;
            case '\t':
                std::cout << "\\t";
                break;
            case '\"':
                std::cout << "\\\"";
                break;
            default:
                std::cout << *iter;
        }
        iter++;
    }
    std::cout << '\"';
}
