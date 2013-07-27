#ifndef READER_H
#define READER_H 

#include <string>
#include <iostream>

class Object;
class ObjectCreator;

class SchemeReader {
    public:
        SchemeReader(ObjectCreator* objcreator);
        SchemeReader(ObjectCreator* objcreator, std::istream& instream);
        Object* read();
    private:
        std::istream& instream_ = std::cin;
        ObjectCreator* objcreator_ = nullptr;

        Object* read_pair();
        Object* read_string();
        Object* read_character();
        Object* read_symbol();

        void eat_whitespace();
        void eat_expected_word(std::string word);
        void peek_expecting_delimiter();
        void print_welcome_message();
        bool is_delimiter(char c);
};

#endif
