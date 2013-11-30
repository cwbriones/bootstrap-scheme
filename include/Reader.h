#ifndef READER_H
#define READER_H 

#include <string>
#include <iostream>

class SchemeObject;
class SchemeObjectCreator;

class SchemeReader {
    public:
        SchemeReader(SchemeObjectCreator* objcreator);
        SchemeReader(SchemeObjectCreator* objcreator, std::istream& instream);
        SchemeObject* read();
    private:
        std::istream& instream_ = std::cin;
        SchemeObjectCreator* objcreator_ = nullptr;

        SchemeObject* read_pair();
        SchemeObject* read_string();
        SchemeObject* read_character();
        SchemeObject* read_symbol();
        SchemeObject* read_vector();

        void eat_whitespace();
        void eat_expected_word(std::string word);
        void peek_expecting_delimiter();
        void print_welcome_message();

        bool is_delimiter(char c);
        bool is_start_of_symbol(char c);
};

#endif
