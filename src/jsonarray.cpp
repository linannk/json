#include "jsonarray.h"
#include "jsonvalue.h"
#include "io/jsonistream.h"
#include "util/jsonutil.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

JsonArray::JsonArray()
{
}

JsonArray::JsonArray(const JsonArray & other)
    : d_arr(other.d_arr)
{
}

JsonArray::JsonArray(JsonArray && lhs)
    : d_arr(std::move(lhs.d_arr))
{
}

JsonArray::JsonArray(const JsonValue &other)
{
    if (other.isArray()) {
        d_arr = other.const_array().d_arr;
    }
}

JsonArray::JsonArray(JsonValue &&other)
{
    if (other.isArray()) {
        d_arr.swap(other.mutable_array()->d_arr);
    }
}

JsonArray::JsonArray(size_t n, const JsonValue &value)
    : d_arr(n, value)
{
}

JsonArray & JsonArray::operator=(const JsonArray & other)
{
    // TODO: insert return statement here
    //! If other is a child of a parent.
    if (this != &other) {
        container_type tmp(other.d_arr);
        this->d_arr = std::move(tmp);
    }
    return *this;
}

JsonArray & JsonArray::operator=(JsonArray && other)
{
    if (this != &other) {
        container_type tmp(std::move(other.d_arr));
        this->d_arr = std::move(tmp);
    }
    return *this;
    // TODO: insert return statement here
}

JsonArray &JsonArray::operator =(const JsonValue &value)
{
    if (value.isArray()) {
        JsonArray tmp(value.const_array());
        this->d_arr = std::move(tmp.d_arr);
    }
    return *this;
}

JsonArray &JsonArray::operator =(JsonValue &&value)
{
    if (value.isArray()) {
        container_type tmp(std::move(value.mutable_array()->d_arr));
        this->d_arr = std::move(tmp);
    }
    return *this;
}

bool JsonArray::operator==(const JsonArray & other) const
{
    if (this == &other) {
        return true;
    }
    return d_arr == other.d_arr;
}

void JsonArray::parseJsonArray(JsonIStream &charSeq, bool parseLeadingChar)
{
    int c = 0;
    if (parseLeadingChar)
    {
        //![0] FUNC_STEP0: EXPECT char is '['
        //! If the first char is not what I want, report an error.
        for (;;)
        {
            c = charSeq.getChar();
            if (c == JSON_OPENBRACKET) {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else if (c == '/') {
                utilSkipComment(charSeq);
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
            else if (c == '/') {
                utilSkipComment(charSeq);
            }
            else if (isspace(c)) {
                continue;
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

END_JSON_NAMESPACE

