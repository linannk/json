#include "jsonistream.h"
BEGIN_JSON_NAMESPACE

int JsonIStream::encode_char_count(int c)
{
    return 1;
}

std::string JsonIStream::json_invalid_chars(int c) {
    std::string invalid_char;
    invalid_char.reserve(128);
    invalid_char.push_back(c);
    for (int i = 1; i < encode_char_count(c); ++i) {
        invalid_char.push_back(this->getChar());
    }
    return invalid_char;
}

END_JSON_NAMESPACE
