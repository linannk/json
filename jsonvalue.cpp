#include "jsonvalue.h"
#include "jsonarray.h"
#include "jsonobject.h"
#include "jsoncharseq.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE
JsonValue::JsonValue()
    : d_valueType(JSON_NULL)
{
}

JsonValue::JsonValue(const JsonValue &other)
    : d_valueType(other.d_valueType)
{
    switch (d_valueType) {
    case JSON_OBJECT:
        this->d_objectValue = new JsonObject(*other.d_objectValue);
        break;
    case JSON_ARRAY:
        this->d_arrayValue = new JsonArray(*other.d_arrayValue);
        break;
    case JSON_STRING:
        this->d_stringValue = new std::string(*other.d_stringValue);
        break;
    case JSON_BOOLEAN:
        this->d_boolValue = other.d_boolValue;
        break;
    case JSON_FLOAT:
        this->d_floatValue = other.d_floatValue;
        break;
    case JSON_DOUBLE:
        this->d_doubleValue = other.d_doubleValue;
        break;
    default:
        break;
    }
}

JsonValue::JsonValue(JsonValue &&other)
    : d_valueType(other.d_valueType)
{
    switch (d_valueType) {
    case JSON_OBJECT:
        this->d_objectValue = other.d_objectValue;
        break;
    case JSON_ARRAY:
        this->d_arrayValue = other.d_arrayValue;
        break;
    case JSON_STRING:
        this->d_stringValue = other.d_stringValue;
        break;
    case JSON_BOOLEAN:
        this->d_boolValue = other.d_boolValue;
        break;
    case JSON_FLOAT:
        this->d_floatValue = other.d_floatValue;
        break;
    case JSON_DOUBLE:
        this->d_doubleValue = other.d_doubleValue;
        break;
    default:
        break;
    }
    other.d_valueType = JSON_NULL;
}

JsonValue::~JsonValue()
{
    this->clear();
}

JsonValue &JsonValue::operator =(const JsonValue &other)
{
    if (this == &other) {
        return *this;
    }

    this->clear();
    this->d_valueType = other.d_valueType;
    switch (d_valueType) {
    case JSON_OBJECT:
        this->d_objectValue = new JsonObject(*other.d_objectValue);
        break;
    case JSON_ARRAY:
        this->d_arrayValue = new JsonArray(*other.d_arrayValue);
        break;
    case JSON_STRING:
        this->d_stringValue = new std::string(*other.d_stringValue);
        break;
    case JSON_BOOLEAN:
        this->d_boolValue = other.d_boolValue;
        break;
    case JSON_FLOAT:
        this->d_floatValue = other.d_floatValue;
        break;
    case JSON_DOUBLE:
        this->d_doubleValue = other.d_doubleValue;
        break;
    default:
        break;
    }
    return *this;
}

JsonValue &JsonValue::operator =(JsonValue &&other)
{
    if (this == &other) {
        return *this;
    }

    this->clear();
    this->d_valueType = other.d_valueType;
    switch (d_valueType) {
    case JSON_OBJECT:
        this->d_objectValue = other.d_objectValue;
        break;
    case JSON_ARRAY:
        this->d_arrayValue = other.d_arrayValue;
        break;
    case JSON_STRING:
        this->d_stringValue = other.d_stringValue;
        break;
    case JSON_BOOLEAN:
        this->d_boolValue = other.d_boolValue;
        break;
    case JSON_FLOAT:
        this->d_floatValue = other.d_floatValue;
        break;
    case JSON_DOUBLE:
        this->d_doubleValue = other.d_doubleValue;
        break;
    default:
        break;
    }
    other.d_valueType = JSON_NULL;
    return *this;
}

void JsonValue::clear()
{
    switch (d_valueType) {
    case JSON_OBJECT:
        delete d_objectValue;
        break;
    case JSON_ARRAY:
        delete d_arrayValue;
        break;
    case JSON_STRING:
        delete d_stringValue;
        break;
    default:
        break;
    }
    d_valueType = JSON_NULL;
}

bool JsonValue::operator ==(const JsonValue &other) const
{
    if (this == &other) {
        return true;
    }
    if (d_valueType != other.d_valueType) {
        return false;
    }
    switch (d_valueType) {
    case JSON_NULL:
        return true;
        break;
    case JSON_BOOLEAN:
        return d_boolValue == other.d_boolValue;
        break;
    case JSON_FLOAT:
        return d_floatValue == other.d_floatValue;
        break;
    case JSON_DOUBLE:
        return d_doubleValue == other.d_doubleValue;
        break;
    case JSON_STRING:
        if (d_stringValue && other.d_stringValue) {
            return *d_stringValue == *d_stringValue;
        }
        return d_stringValue == other.d_stringValue;
        break;
    case JSON_ARRAY:
        if (d_arrayValue && other.d_arrayValue) {
            return *d_arrayValue == *other.d_arrayValue;
        }
        return d_arrayValue == other.d_arrayValue;
        break;
    case JSON_OBJECT:
        if (d_objectValue && other.d_objectValue) {
            return *d_objectValue == *other.d_objectValue;
        }
        return d_objectValue == other.d_objectValue;
        break;
    default:
        return false;
        break;
    }
}

bool JsonValue::operator !=(const JsonValue &other) const
{
    return !(*this == other);
}

bool JsonValue::toBoolean() const
{
    if (d_valueType == JSON_BOOLEAN) {
        return d_boolValue;
    }
    return false;
}

float JsonValue::toFloat() const
{
    if (d_valueType == JSON_FLOAT) {
        return d_floatValue;
    }
    else if (d_valueType == JSON_DOUBLE) {
        return static_cast<float>(d_doubleValue);
    }
    return 0.0f;
}

double JsonValue::toDouble() const
{
    if (d_valueType == JSON_DOUBLE) {
        return d_doubleValue;
    }
    else if (d_valueType == JSON_FLOAT) {
        return static_cast<double>(d_floatValue);
    }
    return 0.0;
}

std::string JsonValue::toString() const
{
    if (d_valueType == JSON_STRING) {
        return *d_stringValue;
    }
    return std::string();
}

JsonArray JsonValue::toArray() const
{
    if (d_valueType == JSON_ARRAY) {
        return *d_arrayValue;
    }
    return JsonArray();
}

JsonObject JsonValue::toObject() const
{
    if (d_valueType == JSON_OBJECT) {
        return *d_objectValue;
    }
    return JsonObject();
}

bool JsonValue::parseFromCharSeq(JsonCharSeq &charSeq)
{
    try {
        JsonValue tmpValue;
        tmpValue.parseJsonValue(charSeq);
        *this = std::move(tmpValue);
        return true;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void JsonValue::parseJsonValue(JsonCharSeq &charSeq)
{
    char c = 0;
    //! [0] FUNC_STEP0: READ FIRST CHAR TO CHECK VALUE TYPE.
    while((c = charSeq.getChar())) {
        if (isspace(c)) {
            continue;
        }
        else if (c == '\"') {
            std::string str;
            while((c = charSeq.getChar())) {
                if (c == '\\') {
                    if ((c = charSeq.getChar())) {
                        switch(c) {
                        case '\"':
                            str.push_back('\"');
                            break;
                        case '\\':
                            str.push_back('\\');
                            break;
                        case '/':
                            //! \/ ??
                            str.push_back('/');
                            break;
                        case 'b':
                            str.push_back('\b');
                            break;
                        case 'f':
                            str.push_back('\f');
                            break;
                        case 'n':
                            str.push_back('\n');
                            break;
                        case 'r':
                            str.push_back('\r');
                            break;
                        case 't':
                            str.push_back('\t');
                            break;
                        case 'u':
                            std::cerr << "\\u IS NOT IMPLEMENTED NOW" << std::endl;
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
                    this->d_stringValue = new std::string(std::move(str));
                    this->d_valueType = JSON_STRING;
                    goto FUNC_STEP1;
                }
                else {
                    str.push_back(c);
                }
            }
        }
        else if (c == '[') {
            this->d_arrayValue = new JsonArray;
            this->d_valueType = JSON_ARRAY;
            this->d_arrayValue->parseJsonArray(charSeq, false);
            goto FUNC_STEP1;
        }
        else if (c == '{') {
            this->d_objectValue = new JsonObject;
            this->d_valueType = JSON_OBJECT;
            this->d_objectValue->parseJsonObject(charSeq, false);
            goto FUNC_STEP1;
        }
        else if (c == '-') {

        }
        else if (c == '+') {

        }
        else if (c == '.') {

        }
        else if (c == 't') {
            if ('r' != charSeq.getChar()) {

            }
            if ('u' != charSeq.getChar()) {

            }
            if ('e' != charSeq.getChar()) {

            }
            this->d_valueType = JSON_BOOLEAN;
            this->d_boolValue = true;
            goto FUNC_STEP1;
        }
        else if (c == 'f') {
            if ('a' != charSeq.getChar()) {

            }
            if ('l' != charSeq.getChar()) {

            }
            if ('s' != charSeq.getChar()) {

            }
            if ('e' != charSeq.getChar()) {

            }
            this->d_valueType = JSON_BOOLEAN;
            this->d_boolValue = true;
            goto FUNC_STEP1;
        }
        else if (c == 'n') {
            if ('u' != charSeq.getChar()) {

            }
            if ('l' != charSeq.getChar()) {

            }
            if ('l' != charSeq.getChar()) {

            }
            this->d_valueType = JSON_NULL;
            goto FUNC_STEP1;
        }
        else {
            throw std::logic_error(std::string("Unexpected char: '") + c + "' encountered.");
        }
    }

FUNC_STEP1:;
}

JsonValue &JsonValue::operator =(JsonArray &&json_array)
{
    this->clear();
    this->d_valueType = JSON_ARRAY;
    this->d_arrayValue = new JsonArray(std::move(json_array));
    return *this;
}

JsonValue &JsonValue::operator =(JsonObject &&json_object)
{
    this->clear();
    this->d_valueType = JSON_OBJECT;
    this->d_objectValue = new JsonObject(std::move(json_object));
    return *this;
}

JsonValue &JsonValue::operator =(const JsonArray &json_array)
{
    this->clear();
    this->d_valueType = JSON_ARRAY;
    this->d_arrayValue = new JsonArray(json_array);
    return *this;
}

JsonValue &JsonValue::operator =(const JsonObject &json_object)
{
    this->clear();
    this->d_valueType = JSON_OBJECT;
    this->d_objectValue = new JsonObject(json_object);
    return *this;
}

END_JSON_NAMESPACE
