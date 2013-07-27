#include "Reader.h"
#include "ObjectCreator.h"

#include <unordered_set>

SchemeReader::SchemeReader(ObjectCreator* objcreator){
    objcreator_ = objcreator;
}

SchemeReader::SchemeReader(ObjectCreator* objcreator, std::istream& instream) : 
    objcreator_(objcreator), instream_(instream) {
}

void SchemeReader::eat_whitespace(){
    char c;
    std::string tmp;

	while (c = (instream_ >> std::ws).peek()){
		if (c == ';'){ // Comments are also whitespace
            std::getline(instream_, tmp);
		} else {
            break;
        }
	}
}

void SchemeReader::eat_expected_word(std::string word){
    int c = 0;
    std::string::iterator iter = word.begin();

    while (iter != word.end()){
        c = instream_.get();
        if (c != *iter){
            std::cerr << "unexpected character '" << (char)c << '\'' << std::endl;
            exit(1);
        }
        iter++;
    }
}


bool SchemeReader::is_delimiter(char c){
	return isspace(c) || c == EOF ||
			c == '('  || c == ')' ||
			c == '"'  || c == ';';
}

void SchemeReader::peek_expecting_delimiter(){
    if (!is_delimiter(instream_.peek())){
        std::cerr << "character not followed by delimiter." << std::endl;
        exit(1);
    }
}

Object* SchemeReader::read(){
	char c;
	short sign = 1;
	long num = 0;

	eat_whitespace();

	c = instream_.get();
    if (c == '\"'){
        // Read in a string.
        return read_string();
    }
    else if (c == '('){
        return read_pair();
    }
    else if (c == '#'){
        c = instream_.get();

        if (c == '\\'){
            // Read in a character literal.
            return read_character();
        } else {
            // Read in a boolean.
            switch(c){
                case 't':
                    return objcreator_->make_boolean(true);
                case 'f':
                    return objcreator_->make_boolean(false);
                default:
                    std::cerr << "unknown boolean literal." << std::endl;
                    exit(1);
            }
        } 
    } else if (isdigit(c) || (c == '-' && isdigit(instream_.peek())) ){
		// Read in a fixnum.
		if (c == '-'){
			sign = -1;
		}
		else {
			instream_.unget();
		}

		while (isdigit(c = instream_.get())){
			num = (num * 10) + (c - '0');
		}
		num *= sign;
		if (is_delimiter(c)){
			instream_.unget();
			return objcreator_->make_fixnum(num);
		}
		else {
			std::cerr << "number not followed by delimiter" << std::endl;
			exit(1);
		}
	} else if (!is_delimiter(c)){
        // Read in a symbol
        instream_.unget();
        return read_symbol();
    } 
    else {
		std::cerr << "bad input. unexpected character '" << c << "'." << std::endl;
		exit(1);
	}
	std::cerr << "read illegal state." << std::endl;
	exit(1);
}

Object* SchemeReader::read_pair(){
    Object* car_obj = nullptr;
    Object* cdr_obj = nullptr;

    eat_whitespace();
    char c = instream_.peek();
    if (c == ')'){
        instream_.get();
        return objcreator_->make_empty_list();
    }

    car_obj = read();

    eat_whitespace();
    c = instream_.get();
    if (c == '.'){
        // Dot notation, cons cell
        if (!is_delimiter( instream_.peek() )){
            std::cerr << "Unexpected character \"" << c << ". Was expecting delimiter." << std::endl;
            exit(1);
        }

        cdr_obj = read();

        eat_whitespace();
        c = instream_.get();

        if (c != ')'){
            std::cerr << "Unexpected character \"" << c << ". Was expecting closing ')'" << std::endl;
            exit(1);
        }
    } else {
        instream_.unget();
        cdr_obj = read_pair();
    }

    return objcreator_->make_pair(car_obj, cdr_obj);
}

Object* SchemeReader::read_string(){
    std::string buffer;
    char c = '\0';

    while( (c = instream_.get()) != '\"') {

        if (c == '\\'){
            // Escaped characters
            c = instream_.get();
            switch(c){
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
            }
        }
        if (c == EOF) {
            std::cerr << "non-terminated string literal." << std::endl;
            exit(1);
        }

        buffer += c;
    }
    
    return objcreator_->make_string(buffer);
}

Object* SchemeReader::read_character(){
    int c;
    c = instream_.get();

    switch(c){
        case EOF:
            std::cerr << "incomplete character literal." << std::endl;
            exit(1);
        case 's':
            if (instream_.peek() == 'p') {
                eat_expected_word("pace");
                peek_expecting_delimiter();
                return objcreator_->make_character(' ');
            }
            break;
        case 'n':
            if (instream_.peek() == 'e'){
                eat_expected_word("ewline");
                peek_expecting_delimiter();
                return objcreator_->make_character('\n');
            }
            break;
        case 't':
            if (instream_.peek() == 'a'){
                eat_expected_word("ab");
                peek_expecting_delimiter();
                return objcreator_->make_character('\t');
            }
    }
    peek_expecting_delimiter();
    return objcreator_->make_character(c);
}

Object* SchemeReader::read_symbol(){
    // Allowable Symbol characters:
    // a-z A-Z + - . * / < = > ! ? : $ % _ & ~ ^ .
    std::string buffer;
    instream_ >> buffer; // Read in the symbol without whitespace

    std::unordered_set<char> allowed_chars = { '+', '-', '.', '*', '/', '<', '=', 
        '>', '!', '?', ':', '$', '%', '_', '&', '~', '^', '.' };
    
    // Ensure validity
    int i = 0;
    for (auto c : buffer){
        if (c == ';'){
            // Remove trailing comment if there was no space
            buffer = buffer.substr(0, i);
            std::string tmp;
            std::getline(instream_, tmp);
            break;
        } 
        if (is_delimiter(c)){
            std::cerr << "Unexpected delimiter \"" << c << "\"" << std::endl;
            exit(1);
        } else if (!isalpha(c) && allowed_chars.find(c) == allowed_chars.end()){
            std::cerr << "Unexpected character \'" << c << "\' not allowed in symbol." << std::endl;
            exit(1);
        }
        i++;
    }

    return objcreator_->make_symbol(buffer);
}
