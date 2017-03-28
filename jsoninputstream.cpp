#include "jsoninputstream.h"
BEGIN_JSON_NAMESPACE

int JsonInputStream::json_char_count(int c)
{
    return 1;
}

std::string JsonInputStream::json_invalid_chars(int c) {
    std::string invalid_char;
    invalid_char.reserve(128);
    invalid_char.push_back(c);
    for (int i = 1; i < json_char_count(c); ++i) {
        invalid_char.push_back(this->getChar());
    }
    return invalid_char;
}

END_JSON_NAMESPACE
