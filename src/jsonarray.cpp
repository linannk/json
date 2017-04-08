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

JsonArray::JsonArray(const container_type & vec)
    : d_arr(vec)
{
}

JsonArray::JsonArray(container_type && vec)
    : d_arr(std::move(vec))
{
}

JsonArray::JsonArray(const JsonValue &other)
    : JsonArray(other.toArray())
{
}

JsonArray::JsonArray(JsonValue &&other)
    : JsonArray(other.moveToArray())
{
}

JsonArray & JsonArray::operator=(const JsonArray & other)
{
    // TODO: insert return statement here
    //! If other is a child of a parent.
    if (this != &other) {
        this->d_arr = other.d_arr;
    }
    return *this;
}

JsonArray & JsonArray::operator=(JsonArray && other)
{
#ifdef JSON_DEBUG
    if (this->contains_recurse(&other)) {
        return *this;
    }
    else if (other.contains_recurse(this)) {
        return * this;
    }
#endif // JSON_DEBUG
    if (this != &other) {
        this->d_arr = std::move(other.d_arr);
    }
    return *this;
    // TODO: insert return statement here
}

JsonArray &JsonArray::operator =(const JsonValue &value)
{
    if (value.isArray()) {
        d_arr = value.const_array().d_arr;
    }
    return *this;
}

JsonArray &JsonArray::operator =(JsonValue &&value)
{
#ifdef JSON_DEBUG
    if (this->contains_recurse(&value)) {
        return *this;
    }
    else if (value.contains_recurse(this)) {
        return *this;
    }
#endif // JSON_DEBUG
    this->operator=(value.moveToArray());
    return *this;
}

bool JsonArray::operator==(const JsonArray & other) const
{
    if (this == &other) {
        return true;
    }
    return d_arr == other.d_arr;
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

size_t JsonArray::size() const
{
    return d_arr.size();
}

bool JsonArray::empty() const
{
    return d_arr.empty();
}

void JsonArray::push_back(const JsonValue & value)
{
    d_arr.push_back(value);
}

void JsonArray::push_back(JsonValue && value)
{
#ifdef JSON_DEBUG
    if (this->contains_recurse(&value)) {
        return;
    }
    else if (value.contains_recurse(this)) {
        return;
    }
#endif // JSON_DEBUG
    d_arr.push_back(std::move(value));
}

void JsonArray::pop_back()
{
    d_arr.pop_back();
}

JsonArray::iterator JsonArray::insert(JsonArray::iterator iter, const JsonValue & value)
{
    return d_arr.insert(iter, value);
}

JsonArray::iterator JsonArray::insert(JsonArray::iterator iter, JsonValue && value)
{
#ifdef JSON_DEBUG
    if (this->contains_recurse(&value)) {
        return end();
    }
    else if (value.contains_recurse(this)) {
        return end();
    }
#endif // JSON_DEBUG
    return d_arr.insert(iter, value);
}

#ifdef JSON_DEBUG
bool JsonArray::contains_recurse(const JsonValue* value) const
{
    for (std::size_t i = 0; i < d_arr.size(); ++i) {
        if (&d_arr[i] == value) {
            return true;
        }
        else if (d_arr[i].contains_recurse(value)) {
            return true;
        }
    }
    return false;
}
bool JsonArray::contains_recurse(const JsonArray * arr) const
{
    for (std::size_t i = 0; i < d_arr.size(); ++i) {
        if (d_arr[i].contains_recurse(arr)) {
            return true;
        }
    }
    return false;
}
#endif // JSON_DEBUG

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

