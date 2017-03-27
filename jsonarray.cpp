#include "jsonarray.h"
#include "jsonvalue.h"
#include "jsoncharseq.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE
JsonArray::JsonArray()
{
}

JsonArray::JsonArray(const JsonArray &other)
    : d_entries(other.d_entries)
{
}

JsonArray::JsonArray(JsonArray &&other)
    : d_entries(std::move(other.d_entries))
{
}

JsonArray &JsonArray::operator =(const JsonArray &other)
{
    if (this != &other) {
        this->d_entries = other.d_entries;
    }
    return *this;
}

JsonArray &JsonArray::operator =(JsonArray &&other)
{
    if (this != &other) {
        this->d_entries = std::move(other.d_entries);
    }
    return *this;
}

bool JsonArray::operator ==(const JsonArray &other)
{
    return d_entries == other.d_entries;
}

bool JsonArray::operator !=(const JsonArray &other)
{
    return d_entries != other.d_entries;
}

std::size_t JsonArray::size() const
{
    return static_cast<int>(this->d_entries.size());
}

void JsonArray::parseJsonArray(JsonCharSeq &charSeq, bool parseLeadingChar)
{
    char c = 0;
    if (parseLeadingChar) {
    //![0] FUNC_STEP0: EXPECT char is '['
        while ((c = charSeq.getChar())) {
            if (isspace(c)) {
                continue;
            }
            else if (c == '[') {
                goto FUNC_STEP1;
            }
            else {
                throw std::logic_error(std::string("Unexpected char: '") + c + "' encountered.");
            }
        }
    }

FUNC_STEP1:
    {
        JsonValue value;
        value.parseJsonValue(charSeq);
        this->d_entries.push_back(std::move(value));
        while ((c = charSeq.getChar())) {
            if (c == ']') {
                goto FUNC_STEP2;
            }
            else if (c == ',') {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else {
                throw std::logic_error(std::string("Unexpected char: '") + c + "' encountered.");
            }
        }
    }

FUNC_STEP2:;
}

bool JsonArray::parseFromCharSeq(JsonCharSeq &charSeq)
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

const JsonValue &JsonArray::operator[](size_t index) const
{
    return d_entries[index];
}

JsonValue &JsonArray::operator[](size_t index)
{
    return d_entries[index];
}

void JsonArray::push_back(const JsonValue &value)
{
    d_entries.push_back(value);
}

void JsonArray::push_back(JsonValue &&value)
{
    d_entries.push_back(std::move(value));
}

void JsonArray::pop_back()
{
    d_entries.pop_back();
}

void JsonArray::insert(JsonArray::iterator iter, const JsonValue &value)
{
    d_entries.insert(iter.d_iter, value);
}

void JsonArray::insert(JsonArray::iterator iter, JsonValue &&value)
{
    d_entries.insert(iter.d_iter, std::move(value));
}

JsonArray::iterator &JsonArray::iterator::operator =(const JsonArray::iterator &other)
{
    if (this != &other) {
        d_iter = other.d_iter;
    }
    return *this;
}

bool JsonArray::iterator::operator ==(const JsonArray::iterator &other) const
{
    return d_iter == other.d_iter;
}

bool JsonArray::iterator::operator !=(const JsonArray::iterator &other) const
{
    return d_iter != other.d_iter;
}

JsonArray::iterator &JsonArray::iterator::operator++()
{
    ++d_iter;
    return *this;
}

JsonArray::iterator &JsonArray::iterator::operator--()
{
    --d_iter;
    return *this;
}

JsonArray::vec_type::pointer JsonArray::iterator::operator->()
{
    return d_iter.operator ->();
}

JsonArray::vec_type::reference JsonArray::iterator::operator*()
{
    return d_iter.operator *();
}

JsonArray::const_iterator &JsonArray::const_iterator::operator =(const JsonArray::const_iterator &other)
{
    if (this != &other) {
        d_iter = other.d_iter;
    }
    return *this;
}

bool JsonArray::const_iterator::operator ==(const JsonArray::const_iterator &other) const
{
    return d_iter == other.d_iter;
}

bool JsonArray::const_iterator::operator !=(const JsonArray::const_iterator &other) const
{
    return d_iter != other.d_iter;
}

JsonArray::const_iterator &JsonArray::const_iterator::operator++()
{
    ++d_iter;
    return *this;
}

JsonArray::const_iterator &JsonArray::const_iterator::operator--()
{
    --d_iter;
    return *this;
}

JsonArray::vec_type::const_pointer JsonArray::const_iterator::operator->()
{
    return d_iter.operator ->();
}

JsonArray::vec_type::const_reference JsonArray::const_iterator::operator*()
{
    return d_iter.operator *();
}

END_JSON_NAMESPACE
