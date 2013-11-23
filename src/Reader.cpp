#include "Reader.h"
#include "SchemeObjectCreator.h"

#include <unordered_set>

SchemeReader::SchemeReader(SchemeObjectCreator* objcreator){
    objcreator_ = objcreator;
}

SchemeReader::SchemeReader(SchemeObjectCreator* objcreator, std::istream& instream) : 
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

bool SchemeReader::is_start_of_symbol(char c){
    return isdigit(c) || isalpha(c) ||
        ( (c != '#') && (c != '\\') &&
          (c != '`') && !is_delimiter(c) &&
          (c != '\'') );
}

void SchemeReader::peek_expecting_delimiter(){
    if (!is_delimiter(instream_.peek())){
        std::cerr << "character not followed by delimiter." << std::endl;
        exit(1);
    }
}

SchemeObject* SchemeReader::read(){
	char c;
	short sign = 1;
	long num = 0;

	eat_whitespace();
	c = instream_.get();

    if (c == '\"'){
        return read_string();
    }
    else if (c == '('){
        return read_pair();
    }
    else if (c == '#'){
        c = instream_.get();

        if (c == '\\'){
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
    } 
    else if (isdigit(c) || (c == '-' && isdigit(instream_.peek())) ){
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
	} 
    else if (is_start_of_symbol(c)){
        // Read in a symbol
        instream_.unget();
        return read_symbol();
    }
    else if (c == '\'' || c == '`'){
        return objcreator_->make_tagged_list("quote", read());
    }
    else {
		std::cerr << "bad input. unexpected character '" << c << "'." << std::endl;
		exit(1);
	}
	std::cerr << "read illegal state." << std::endl;
	exit(1);
}

SchemeObject* SchemeReader::read_pair(){
    SchemeObject* car_obj = nullptr;
    SchemeObject* cdr_obj = nullptr;

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
            std::cerr << "Unexpected character \"" << c 
                << ". Was expecting delimiter." << std::endl;
            exit(1);
        }

        cdr_obj = read();

        eat_whitespace();
        c = instream_.get();

        if (c != ')'){
            std::cerr << "Unexpected character \"" << c 
                << ". Was expecting closing ')'" << std::endl;
            exit(1);
        }
    } else {
        instream_.unget();
        cdr_obj = read_pair();
    }

    return objcreator_->make_pair(car_obj, cdr_obj);
}

SchemeObject* SchemeReader::read_string(){
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

SchemeObject* SchemeReader::read_character(){
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

SchemeObject* SchemeReader::read_symbol(){
    // Allowable Symbol characters:
    // a-z A-Z + - . * / < = > ! ? : $ % _ & ~ ^ .
    std::string buffer;
    std::unordered_set<char> allowed_chars = { '+', '-', '.', '*', '/', '<', '=', 
        '>', '!', '?', ':', '$', '%', '_', '&', '~', '^', '.' , '#' };
    
    char c;
    while ( (c = instream_.get()) && !is_delimiter(c) ){
        buffer += c;
    }
    if (is_delimiter(c)){
        instream_.unget();
    }

    // Ensure validity
    // Also note that # is allowed anywhere so long as it is not preceded by %
    int i = 0;
    for (auto c : buffer){
        if (!isalpha(c) && !isdigit(c) && (allowed_chars.find(c) == allowed_chars.end())){
            std::cerr << "Unexpected character \'" << c 
                      << "\' not allowed in symbol." << std::endl;
            exit(1);
        }
        else if (c == '#' && buffer[i - 1] == '%'){
        // This check is okay because read_symbol can't be called if the first char is #
            std::cerr << "# cannot follow a % in a symbol." << std::endl;
            exit(1);
        }
        i++;
    }

    return objcreator_->make_symbol(buffer);
}