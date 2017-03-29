#include "jsonarray.h"
#include "jsonvalue.h"
#include "io/jsonistream.h"
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
    other.clear();
}

JsonArray::JsonArray(size_t n, const JsonValue &value)
    : d_arr(n, value)
{
}

JsonArray & JsonArray::operator=(const JsonArray & other)
{
    // TODO: insert return statement here
    if (this != &other) {
        JsonArray tmp(other);
        swap(tmp);
    }
    return *this;
}

JsonArray & JsonArray::operator=(JsonArray && other)
{
    if (this != &other) {
        JsonArray tmp(std::move(other));
        swap(tmp);
    }
    return *this;
    // TODO: insert return statement here
}

JsonArray &JsonArray::operator =(const JsonValue &value)
{
    if (value.isArray()) {
        JsonArray tmp(value.const_array());
        swap(tmp);
    }
    return *this;
}

JsonArray &JsonArray::operator =(JsonValue &&value)
{
    if (value.isArray()) {
        JsonArray tmp(std::move(*value.mutable_array()));
        swap(tmp);
    }
    value.clear();
    return *this;
}

bool JsonArray::operator==(const JsonArray & other) const
{
    if (this == &other) {
        return true;
    }
    return d_arr == other.d_arr;
}

void JsonArray::push_back(const JsonValue & value)
{
    this->d_arr.push_back(value);
}

void JsonArray::push_back(JsonValue && value)
{
    this->d_arr.emplace_back(std::move(value));
}

void JsonArray::swap(JsonArray & other)
{
    if (this != &other) {
        this->d_arr.swap(other.d_arr);
    }
}

void JsonArray::insert(iterator iter, const JsonValue & value)
{
    d_arr.insert(iter, value);
}

void JsonArray::insert(iterator iter, JsonValue && value)
{
    d_arr.emplace(iter, std::move(value));
}

void JsonArray::insert(iterator iter, size_t n, const JsonValue & value)
{
    d_arr.insert(iter, n, value);
}

JsonValue & JsonArray::operator[](std::size_t idx)
{
    // TODO: insert return statement here
    return d_arr[idx];
}

const JsonValue & JsonArray::operator[](std::size_t idx) const
{
    // TODO: insert return statement here
    return d_arr[idx];
}

JsonValue & JsonArray::front()
{
    // TODO: insert return statement here
    return d_arr.front();
}

JsonValue & JsonArray::back()
{
    // TODO: insert return statement here
    return d_arr.back();
}

const JsonValue & JsonArray::front() const
{
    // TODO: insert return statement here
    return d_arr.front();
}

const JsonValue & JsonArray::back() const
{
    // TODO: insert return statement here
    return d_arr.back();
}

JsonValue * JsonArray::data()
{
    return d_arr.data();
}

const JsonValue * JsonArray::data() const
{
    return d_arr.data();
}

size_t JsonArray::size() const
{
    return d_arr.size();
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

END_JSON_NAMESPACE

