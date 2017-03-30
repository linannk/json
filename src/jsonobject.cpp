#include "jsonobject.h"
#include "jsonvalue.h"
#include "io/jsonistream.h"
#include "util/jsonutil.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

JsonObject::JsonObject()
{
}

JsonObject::JsonObject(const JsonObject & other)
    : d_map(other.d_map)
{
}

JsonObject::JsonObject(JsonObject && other)
    : d_map(std::move(other.d_map))
{
}

JsonObject & JsonObject::operator=(const JsonObject & other)
{
    // TODO: insert return statement here
    if (this != &other) {
        d_map = other.d_map;
    }
    return *this;
}

JsonObject & JsonObject::operator=(JsonObject && other)
{
    // TODO: insert return statement here
    if (this != &other) {
        d_map = std::move(other.d_map);
    }
    return *this;
}

JsonObject::JsonObject(const JsonValue & value)
{
    if (value.isObject()) {
        d_map = value.const_object().d_map;
    }
}

JsonObject::JsonObject(JsonValue && value)
{
    if (value.isObject()) {
        d_map = std::move(value.mutable_object()->d_map);
    }
}

JsonObject & JsonObject::operator=(const JsonValue & value)
{
    // TODO: insert return statement here
    if (value.isObject()) {
        d_map = value.const_object().d_map;
    }
    return *this;
}

JsonObject & JsonObject::operator=(JsonValue && value)
{
    // TODO: insert return statement here
    if (value.isObject()) {
        d_map = std::move(value.mutable_object()->d_map);
    }
    return *this;
}

bool JsonObject::operator==(const JsonObject & other) const
{
    return d_map == other.d_map;
}

JsonObject::iterator JsonObject::insert(const std::string & key, const JsonValue & value)
{
    d_map.insert(std::make_pair(key, value));
    return iterator();
}

JsonObject::iterator JsonObject::find(const std::string & key)
{
    return d_map.find(key);
}

JsonObject::const_iterator JsonObject::find(const std::string & key) const
{
    return d_map.find(key);
}

JsonValue & JsonObject::operator[](const std::string & key)
{
    // TODO: insert return statement here
    return d_map[key];
}

bool JsonObject::hasKey(const std::string & key) const
{
    return end() != find(key);
}

void JsonObject::parseJsonObject(JsonIStream &charSeq, bool parseLeadingChar)
{
    char c = 0;
    if (parseLeadingChar)
    {
        //![0] FUNC_STEP0: EXPECT char is '{'
        for (;;)
        {
            c = charSeq.getChar();
            if (c == JSON_OPENBRACE) {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else if (c == '/') {
                utilSkipComment(charSeq);
                continue;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::runtime_error("Expected '{', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
            }
        }
    }

FUNC_STEP1:
    for (;;)
    {
        c = charSeq.getChar();
        if (c == JSON_QUOTE) {
            goto FUNC_STEP2;
        }
        else if (c == JSON_CLOSEBRACE) {
            goto FUNC_STEP6;
        }
        else if (isspace(c)) {
            continue;
        }
        else if (c == '/') {
            utilSkipComment(charSeq);
            continue;
        }
        else if (c == -1) {
            throw std::runtime_error("Unexpected end of JsonInputStream");
        }
        else {
            throw std::runtime_error(std::string("Expected '\"' or '}', but \"") + charSeq.json_invalid_chars(c) + "\" is encountered.");
        }
    }

FUNC_STEP2:
    {
        std::string key = utilGetString(charSeq);

//! FUNC_STEP3:
        for (;;) {
            c = charSeq.getChar();
            if (c == ':') {
                goto FUNC_STEP4;
            }
            else if (isspace(c)) {
                continue;
            }
            else if (c == '/') {
                utilSkipComment(charSeq);
                continue;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::runtime_error("Expected ':', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
            }
        }

FUNC_STEP4:
        JsonValue value;
        value.parseJsonValue(charSeq);
        this->insert(std::move(key), std::move(value));
    }

    //! [5] FUNC_STEP5: EXPECT chars are ',' and '}'
    for (;;) {
        c = charSeq.getChar();
        if (c == JSON_COMMA) {
            goto FUNC_STEP1;
        }
        else if (c == JSON_CLOSEBRACE) {
            goto FUNC_STEP6;
        }
        else if (isspace(c)) {
            continue;
        }
        else if (c == '/') {
            utilSkipComment(charSeq);
            continue;
        }
        else if (c == -1) {
            throw std::runtime_error("Unexpected end of JsonInputStream");
        }
        else {
            throw std::runtime_error("Expected '}' or ',', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
        }
    }

FUNC_STEP6:
    //! Todo: Check following char sequence.
    ;
}

bool JsonObject::parseFromInputStream(JsonIStream &charSeq)
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

bool JsonObject::serializeToOStream(std::ostream * os, int tab_size) const
{
    *os << '{' << std::endl;
    for (auto i = this->begin(); i != this->end(); ++i) {
        for (int j = 0; j < tab_size + JSON_TAB_OFFSET; ++j) {
            *os << ' ';
        }
        *os << '\"';
        for (auto ch : i->first) {
            if (ch == '\"') {
                *os << "\\\"";
            }
            else if (ch == '\\') {
                *os << "\\\\";
            }
            else {
                *os << ch;
            }
        }
        *os << "\": ";
        i->second.serializeToOStream(os, tab_size);
        
        auto tmp = i;
        ++tmp;
        if (tmp != this->end()) {
            *os << ',';
        }
        *os << std::endl;
    }
    for (int j = 0; j < tab_size; ++j) {
        *os << ' ';
    }
    *os << '}';
    return os->good();
}

END_JSON_NAMESPACE
