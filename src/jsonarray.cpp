#include "jsonarray.h"
#include "jsonvalue.h"
#include "io/jsonistream.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

JsonArray::JsonArray()
{}

JsonArray::JsonArray(const JsonValue &other)
    : std::vector<JsonValue>(other.toArray())
{}

JsonArray::JsonArray(JsonValue &&other)
{
    if (other.isArray()) {
        this->swap(*other.mutable_array());
    }
}

JsonArray::JsonArray(size_t n, const JsonValue &value)
    : std::vector<JsonValue>(n, value)
{
}

JsonArray &JsonArray::operator =(const JsonValue &value)
{
    *this = value.toArray();
    return *this;
}

JsonArray &JsonArray::operator =(JsonValue &&value)
{
    this->clear();
    if (value.isArray()) {
        this->swap(*value.mutable_array());
    }
    return *this;
}

void JsonArray::parseJsonArray(JsonIStream &charSeq, bool parseLeadingChar)
{
    int c = 0;
    if (parseLeadingChar)
    {
        //![0] FUNC_STEP0: EXPECT char is '['
        for (;;)
        {
            c = charSeq.getChar();
            if (c == JSON_OPENBRACKET) {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::runtime_error("Unexpected char: \"" + charSeq.json_invalid_chars(c) + "\"");
            }
        }
    }

FUNC_STEP1:
    {
        JsonValue value;
        value.parseJsonValue(charSeq);
        this->push_back(std::move(value));
        for (;;) {
            c = charSeq.getChar();
            if (c == JSON_COMMA) {
                goto FUNC_STEP1;
            }
            else if (c == JSON_CLOSEBRACKET) {
                goto FUNC_STEP2;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::logic_error("Unexpected char: \"" + charSeq.json_invalid_chars(c) + "\"");
            }
        }
    }

FUNC_STEP2:;
}

bool JsonArray::parseFromInputStream(JsonIStream &charSeq)
{
    try {
        JsonArray tmpArray;
        tmpArray.parseJsonArray(charSeq);
        *this = std::move(tmpArray);
        return true;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool JsonArray::serializeToOStream(std::ostream * os, int tab_size) const
{
    *os << '[' << std::endl;
    for (std::size_t i = 0; i < this->size() - 1; ++i) {
        for (int j = 0; j < tab_size + JSON_TAB_OFFSET; ++j) {
            *os << ' ';
        }
        this->operator[](i).serializeToOStream(os, tab_size + JSON_TAB_OFFSET);
        *os << ',' << std::endl;
    }
    for (int j = 0; j < tab_size + JSON_TAB_OFFSET; ++j) {
        *os << ' ';
    }
    this->operator[](this->size() - 1).serializeToOStream(os, tab_size + JSON_TAB_OFFSET);
    *os << std::endl;
    for (int j = 0; j < tab_size; ++j) {
        *os << ' ';
    }
    *os << " ]";
    return os->good();
}

std::ostream& operator << (std::ostream& os, const JSON_NAMESPACE::JsonArray& array)
{
    return os;
}

END_JSON_NAMESPACE
