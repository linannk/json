#include "jsonobject.h"
#include "jsonvalue.h"
#include "jsoncharseq.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE
JsonObject::JsonObject() {}

JsonObject::JsonObject(const JsonObject &other)
    : d_entries(other.d_entries)
{}

JsonObject::JsonObject(JsonObject &&other)
    : d_entries(std::move(other.d_entries))
{}

JsonObject& json::JsonObject::operator =(const JsonObject &other) {
    if (this != &other) {
        this->d_entries = other.d_entries;
    }
    return *this;
}

JsonObject& JsonObject::operator =(JsonObject &&other) {
    if (this != &other) {
        this->d_entries = std::move(other.d_entries);
    }
    return *this;
}

bool JsonObject::operator ==(const JsonObject &other) const
{
    return d_entries == other.d_entries;
}

bool JsonObject::operator !=(const JsonObject &other) const
{
    return d_entries != other.d_entries;
}

bool JsonObject::hasKey(const std::string &key) const
{
    return d_entries.end() != d_entries.find(key);
}

JsonObject::~JsonObject()
{}

void JsonObject::parseJsonObject(JsonCharSeq &charSeq, bool parseLeadingChar)
{
    char c = 0;
    if (parseLeadingChar) {
    //![0] FUNC_STEP0: EXPECT char is '{'
        while((c = charSeq.getChar())) {
            if (c == '{') {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else {
                throw std::logic_error(std::string("Expected '{', but ") + c + " is encountered.");
                return;
            }
        }
    }

FUNC_STEP1:
    while((c = charSeq.getChar())) {
        if (c == '\"') {
            goto FUNC_STEP2;
        }
        else if (isspace(c)) {
            continue;
        }
        else {
            throw std::logic_error(std::string("Expected '\"', but ") + c + " is encountered.");
        }
    }

FUNC_STEP2:
    {
        std::string key;
        while((c = charSeq.getChar())) {
            if (c == '\\') {
                if ((c = charSeq.getChar())) {
                    switch(c) {
                    case '\"':
                        key.push_back('\"');
                        break;
                    case '\\':
                        key.push_back('\\');
                        break;
                    case '/':
                        //! \/ ??
                        key.push_back('/');
                        break;
                    case 'b':
                        key.push_back('\b');
                        break;
                    case 'f':
                        key.push_back('\f');
                        break;
                    case 'n':
                        key.push_back('\n');
                        break;
                    case 'r':
                        key.push_back('\r');
                        break;
                    case 't':
                        key.push_back('\t');
                        break;
                    case 'u':
                        std::cerr << "\\u IS NOT IMPLEMENTED YET" << std::endl;
                        key.push_back('\\');
                        key.push_back('u');
                        break;
                    default:
                        throw std::logic_error(std::string("Unexpected char encountered: ") + c);
                        break;
                    }
                }
                else {
                    throw std::logic_error("Unexpected end of char sequence.");
                }
            }
            else if (c == '\"') {
                goto FUNC_STEP3;
            }
            else {
                key.push_back(c);
            }
        }

FUNC_STEP3:
        while((c = charSeq.getChar())) {
            if (c == ':') {
                goto FUNC_STEP4;
            }
            else if (isspace(c)) {
                continue;
            }
            else {
                throw std::logic_error(std::string("Expected ':', but ") + c + " is encountered.");
            }
        }

FUNC_STEP4:
        JsonValue value;
        value.parseJsonValue(charSeq);
        this->d_entries.emplace(key, value);
    }

//! [5] FUNC_STEP5: EXPECT chars are ',' and '}'
    while((c = charSeq.getChar())) {
        if (c == ',') {
            goto FUNC_STEP1;
        }
        else if (c == '}') {
            goto FUNC_STEP6;
        }
        else if (isspace(c)) {
            continue;
        }
        else {
            throw std::logic_error(std::string("Expected '}' or ',', but ") + c + " is encountered.");
        }
    }

FUNC_STEP6:;
}

JsonValue &JsonObject::operator[](const std::string &key)
{
    return d_entries[key];
}

JsonObject::iterator JsonObject::find(const std::string &key)
{
    return iterator(d_entries.find(key));
}

JsonObject::const_iterator JsonObject::find(const std::string &key) const
{
    return const_iterator(d_entries.find(key));
}

bool JsonObject::parseFromCharSeq(JsonCharSeq &charSeq)
{
    try {
        JsonObject tmpObject;
        tmpObject.parseJsonObject(charSeq);
        *this = std::move(tmpObject);
        return true;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

JsonObject::iterator::iterator()
{}

JsonObject::iterator &JsonObject::iterator::operator =(const JsonObject::iterator &other)
{
    if (this != &other) {
        d_iter = other.d_iter;
    }
    return *this;
}

bool JsonObject::iterator::operator ==(const JsonObject::iterator &other) const
{
    return d_iter == other.d_iter;
}

bool JsonObject::iterator::operator !=(const JsonObject::iterator &other) const
{
    return d_iter != other.d_iter;
}

JsonObject::iterator &JsonObject::iterator::operator++()
{
    ++d_iter; return *this;
}

JsonObject::iterator &JsonObject::iterator::operator--()
{
    --d_iter; return *this;
}

JsonObject::map_type::pointer JsonObject::iterator::operator->()
{
    return d_iter.operator ->();
}

JsonObject::map_type::reference JsonObject::iterator::operator*()
{
    return d_iter.operator *();
}

JsonObject::const_iterator::const_iterator()
{}

JsonObject::const_iterator &JsonObject::const_iterator::operator =(const JsonObject::const_iterator &other)
{
    if (this != &other) {
        d_iter = other.d_iter;
    }
    return *this;
}

bool JsonObject::const_iterator::operator ==(const JsonObject::const_iterator &other) const
{
    return d_iter == other.d_iter;
}

bool JsonObject::const_iterator::operator !=(const JsonObject::const_iterator &other) const
{
    return d_iter != other.d_iter;
}

JsonObject::const_iterator &JsonObject::const_iterator::operator++()
{
    ++d_iter; return *this;
}

JsonObject::const_iterator &JsonObject::const_iterator::operator--()
{
    --d_iter; return *this;
}

JsonObject::map_type::const_pointer JsonObject::const_iterator::operator->()
{
    return d_iter.operator ->();
}

JsonObject::map_type::const_reference JsonObject::const_iterator::operator*()
{
    return d_iter.operator *();
}

END_JSON_NAMESPACE
