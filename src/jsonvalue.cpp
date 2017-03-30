#include "jsonvalue.h"
#include "jsonarray.h"
#include "jsonobject.h"
#include "io/jsonistream.h"
#include "io/jsonstringistream.h"
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

JsonValue::JsonValue(bool value)
    : d_valueType(JSON_BOOLEAN)
    , d_boolValue(value)
{
}

JsonValue::JsonValue(float value)
    : d_valueType(JSON_FLOAT)
    , d_boolValue(value)
{
}

JsonValue::JsonValue(double value)
    : d_valueType(JSON_DOUBLE)
    , d_boolValue(value)
{
}

JsonValue::JsonValue(const std::string &str)
    : d_valueType(JSON_STRING)
    , d_stringValue(new std::string(str))
{
}

JsonValue::JsonValue(std::string &&str)
    : d_valueType(JSON_STRING)
    , d_stringValue(new std::string(std::move(str)))
{
}

JsonValue::JsonValue(const JsonObject &obj)
    : d_valueType(JSON_OBJECT)
    , d_objectValue(new JsonObject(obj))
{
}

JsonValue::JsonValue(JsonObject &&obj)
    : d_valueType(JSON_OBJECT)
    , d_objectValue(new JsonObject(std::move(obj)))
{
}

JsonValue::JsonValue(const JsonArray &arr)
    : d_valueType(JSON_ARRAY)
    , d_arrayValue(new JsonArray(arr))
{
}

JsonValue::JsonValue(JsonArray &&arr)
    : d_valueType(JSON_ARRAY)
    , d_arrayValue(new JsonArray(std::move(arr)))
{
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
    JsonValue tmp(other);
    swap(tmp);
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

void JsonValue::swap(JsonValue &other)
{
    if (this == &other) {
        return;
    }
    JsonValue tmp(std::move(other));
    other = std::move(*this);
    *this = std::move(tmp);
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

JsonValue & JsonValue::operator[](const std::string & key)
{
    // TODO: insert return statement here
    if (!isObject()) {
        this->clear();
        this->d_objectValue = new JsonObject();
    }
    return d_objectValue->operator[](key);
}

JsonValue & JsonValue::operator[](std::size_t idx)
{
    // TODO: insert return statement here
    return this->d_arrayValue->operator[](idx);
}

void JsonValue::push_back(const JsonValue & other)
{
    if (!isArray()) {
        this->clear();
        this->d_arrayValue = new JsonArray();
    }
    this->d_arrayValue->push_back(other);
}

void JsonValue::push_back(JsonValue && other)
{
    if (!isArray()) {
        this->clear();
        this->d_arrayValue = new JsonArray();
    }
    this->d_arrayValue->push_back(std::move(other));
}

void JsonValue::insert(const std::string & key, const JsonValue & value)
{
    if (!isObject()) {
        this->clear();
        this->d_objectValue = new JsonObject();
    }
    this->d_objectValue->emplace(key, value);
}

void JsonValue::insert(const std::string & key, JsonValue && value)
{
    if (!isObject()) {
        this->clear();
        this->d_objectValue = new JsonObject();
    }
    this->d_objectValue->emplace(key, std::move(value));
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

bool JsonValue::parseFromInputStream(JsonIStream &charSeq)
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

bool JsonValue::serializeToOStream(std::ostream * os, int tab_size) const
{
    switch (this->d_valueType) {
    case JSON_NULL:
        *os << "null";
        break;
    case JSON_BOOLEAN:
        if (this->d_boolValue) {
            *os << "true";
        }
        else {
            *os << "false";
        }
        break;
    case JSON_FLOAT:
        *os << std::to_string(this->d_floatValue) + 'f';
        break;
    case JSON_DOUBLE:
        *os << std::to_string(this->d_doubleValue);
        break;
    case JSON_ARRAY:
        this->d_arrayValue->serializeToOStream(os, tab_size + JSON_TAB_OFFSET);
        break;
    case JSON_OBJECT:
        this->d_objectValue->serializeToOStream(os, tab_size + JSON_TAB_OFFSET);
        break;
    case JSON_STRING:
        for (auto ch : *d_stringValue) {
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
        break;
    default:
        break;
    }
    return os->good();
}

void JsonValue::parseJsonValue(JsonIStream &charSeq)
{
    char c = 0;
    //! [0] FUNC_STEP0: READ FIRST CHAR TO CHECK VALUE TYPE.
    for (;;) {
        c = charSeq.getChar();
        if (c == -1) {
            throw std::runtime_error("Unexpected end of JsonInputStream");
        }
        switch (c) {
        case JSON_OPENBRACE:
            this->d_objectValue = new JsonObject;
            this->d_valueType = JSON_OBJECT;
            this->d_objectValue->parseJsonObject(charSeq, false);
            goto FUNC_STEP1;
        case JSON_OPENBRACKET:
            this->d_arrayValue = new JsonArray;
            this->d_valueType = JSON_ARRAY;
            this->d_arrayValue->parseJsonArray(charSeq, false);
            goto FUNC_STEP1;
        case JSON_QUOTE: {
            std::string str;
            for (;;) {
                c = charSeq.getChar();
                if (c == -1) {
                    throw std::runtime_error("Unexpected end of JsonInputStream");
                }
                if (c == '\\') {
                    c = charSeq.getChar();
                    if (c != -1) {
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
                            throw std::runtime_error(std::string("Unexpected char encountered: ") + charSeq.json_invalid_chars(c));
                            break;
                        }
                    }
                    else {
                        throw std::runtime_error("Unexpected end of JsonInputStream");
                    }
                }
                else if (c == '\"') {
                    this->d_stringValue = new std::string(std::move(str));
                    this->d_valueType = JSON_STRING;
                    goto FUNC_STEP1;
                }
                else {
                    str.push_back(c);
                    const int char_count = charSeq.encode_char_count(c);
                    for (int i = 1; i < char_count; ++i) {
                        c = charSeq.getChar();
                        if (c == -1) {
                            throw std::runtime_error("Unexpected end of JsonInputStream");
                        }
                        else {
                            str.push_back(c);
                        }
                    }
                }
            }
        }
            break;
        case 't':
            if ((c= charSeq.getChar()) != 'r') {
                throw std::runtime_error("Expected \"r\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            if ((c = charSeq.getChar()) != 'u') {
                throw std::runtime_error("Expected \"u\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            if ((c = charSeq.getChar()) != 'e') {
                throw std::runtime_error("Expected \"e\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            this->d_valueType = JSON_BOOLEAN;
            this->d_boolValue = true;
            goto FUNC_STEP1;
            break;
        case 'f':
            if ((c = charSeq.getChar()) != 'a') {
                throw std::runtime_error("Expected \"a\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            if ((c = charSeq.getChar()) != 'l') {
                throw std::runtime_error("Expected \"l\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            if ((c = charSeq.getChar()) != 's') {
                throw std::runtime_error("Expected \"s\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            if ((c = charSeq.getChar()) != 'e') {
                throw std::runtime_error("Expected \"e\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            this->d_valueType = JSON_BOOLEAN;
            this->d_boolValue = false;
            goto FUNC_STEP1;
        case 'n':
            if ((c = charSeq.getChar()) != 'u') {
                throw std::runtime_error("Expected \"r\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            if ((c = charSeq.getChar()) != 'l') {
                throw std::runtime_error("Expected \"r\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            if ((c = charSeq.getChar()) != 'l') {
                throw std::runtime_error("Expected \"r\", but \"" + charSeq.json_invalid_chars(c) + "\" is encontered!");
            }
            this->d_valueType = JSON_NULL;
            goto FUNC_STEP1;
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
                std::string number;
                number.reserve(128);
                if (c == '-') {
                    number.push_back('-');
                    c = charSeq.getChar();
                }
                if (c == '\0') {
                    number.push_back('\0');
                    c = charSeq.getChar();
                    goto FUNC_NUMBER_STEP1;
                }
                else if (isdigit(c)) {
                    number.push_back(c);
                    for (;;) {
                        c = charSeq.getChar();
                        if (isdigit(c)) {
                            number.push_back(c);
                            continue;
                        }
                        goto FUNC_NUMBER_STEP1;
                    }
                }
                else {
                    throw std::runtime_error("Expect digit between 0 ~ 9, but " + charSeq.json_invalid_chars(c) + " is encountered.");
                }

FUNC_NUMBER_STEP1:
                if (c == '.') {
                    c = charSeq.getChar();
                    if (isdigit(c)) {
                        number.push_back(c);
                    }
                    else if (c == 'f' || c == 'F') {
                        number.push_back(c);
                        goto FUNC_NUMBER_FLOAT;
                    }
                    else {
                        throw std::runtime_error("Expect 0 ~ 9, f, F, but " + charSeq.json_invalid_chars(c) + "is encountered.");
                    }
                    for (;;) {
                        c = charSeq.getChar();
                        if (isdigit(c)) {
                            number.push_back(c);
                            continue;
                        }
                        else if (c == 'f' || c == 'F') {
                            goto FUNC_NUMBER_FLOAT;
                        }
                        else if (c == 'e' || c == 'E') {
                            goto FUNC_NUMBER_STEP2;
                        }
                        else {
                            goto FUNC_NUMBER_DOUBLE;
                        }
                    }
                }

FUNC_NUMBER_STEP2:
                if (c == 'e' || c == 'E') {
                    number.push_back(c);
                    c = charSeq.getChar();
                    if (isdigit(c)) {
                        number.push_back(c);
                    }
                    else if (c == '+' || c == '-') {
                        number.push_back(c);
                    }
                    else {
                        throw std::runtime_error("Expect \"+\" or \"-\", but " + charSeq.json_invalid_chars(c) + " is encountered.");
                    }
                    for(;;) {
                        c = charSeq.getChar();
                        if (isdigit(c)) {
                            number.push_back(c);
                        }
                        else if (c == 'f' || c == 'F') {
                            number.push_back(c);
                            goto FUNC_NUMBER_FLOAT;
                        }
                        else {
                            goto FUNC_NUMBER_DOUBLE;
                        }
                    }
                }

                goto FUNC_NUMBER_DOUBLE;

FUNC_NUMBER_FLOAT:
                this->d_valueType = JSON_FLOAT;
                this->d_floatValue = std::stof(number);
                goto FUNC_STEP1;

FUNC_NUMBER_DOUBLE:
                charSeq.ungetChar();
                this->d_valueType = JSON_DOUBLE;
                this->d_doubleValue = std::stod(number);
                goto FUNC_STEP1;
            }
            break;
        case '/': {
            c = charSeq.getChar();
            if (c == '/') {
                //! Line comment
                for (;;) {
                    c = charSeq.getChar();
                    if (c != -1) {
                        const int char_count = charSeq.encode_char_count(c);
                        for (int i = 1; i < char_count; ++i) {
                            c = charSeq.getChar();
                            if (c == -1) {
                                throw std::runtime_error("Unexpected end of JsonInputStream");
                            }
                        }
                        if (c == '\n' || c == '\r') {
                            goto FUNC_COMMENT_JUMP;
                        }
                    }
                    throw std::runtime_error("Unexpected end of JsonInputStream");
                }
            }
            else if (c == '*') {
                //! Block comment
                for (;;) {
                    c = charSeq.getChar();
                    if (c == -1) {

                    }
                    if (c == '*') {
                        c = charSeq.getChar();
                        if (c =='/' ) {
                            goto FUNC_COMMENT_JUMP;
                        }
                    }
                }
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::runtime_error("Unexpected '/'");
            }
            break;
        }
        default:
            if (isspace(c)) {
                continue;
            }
            else {
                throw std::runtime_error("Unexpected char encountered.");
            }
            break;
        }
FUNC_COMMENT_JUMP:
    }
FUNC_STEP1:;
}

JsonValue &JsonValue::operator =(JsonArray &&json_array)
{
    this->clear();
    this->d_arrayValue = new JsonArray(std::move(json_array));
    this->d_valueType = JSON_ARRAY;
    return *this;
}

JsonValue &JsonValue::operator =(JsonObject &&json_object)
{
    this->clear();
    this->d_objectValue = new JsonObject(std::move(json_object));
    this->d_valueType = JSON_OBJECT;
    return *this;
}

JsonValue &JsonValue::operator =(const JsonArray &json_array)
{
    this->clear();
    this->d_arrayValue = new JsonArray(json_array);
    this->d_valueType = JSON_ARRAY;
    return *this;
}

JsonValue &JsonValue::operator =(const JsonObject &json_object)
{
    this->clear();
    this->d_objectValue = new JsonObject(json_object);
    this->d_valueType = JSON_OBJECT;
    return *this;
}

END_JSON_NAMESPACE

JSON_NAMESPACE::JsonValue operator"" _json(const char *json, std::size_t size)
{
    JSON_NAMESPACE::JsonCStrWrapperIStream jcsis(json, size);
    JSON_NAMESPACE::JsonValue json_value;
    json_value.parseFromInputStream(jcsis);
    return json_value;
}

