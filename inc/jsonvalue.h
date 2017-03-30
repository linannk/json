#ifndef JSONVALUE_H
#define JSONVALUE_H
#include "jsondef.h"
#include <string>

BEGIN_JSON_NAMESPACE
enum JsonValueType {
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_FLOAT,
    JSON_DOUBLE,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
};

class JsonObject;
class JsonArray;
class JsonIStream;

class JsonValue {
public:
    JsonValue();
    JsonValue(const JsonValue& other);
    JsonValue(JsonValue&& other);
    ~JsonValue();
    void clear();

    JsonValue& operator =(const JsonValue& other);
    JsonValue& operator =(JsonValue&& other);

    bool operator ==(const JsonValue& other) const;
    bool operator !=(const JsonValue& other) const;

    //! Call this function will make JsonValue to JsonObject
    JsonValue& operator[](const std::string &key);

    //! If isArray() returns false, call this function maybe cause undefined behavior.
    JsonValue& operator[](std::size_t idx);

    void push_back(const JsonValue& other);
    void push_back(JsonValue&& other);

    void insert(const std::string& key, const JsonValue& value);
    void insert(const std::string& key, JsonValue&& value);

    inline JsonValue& operator =(bool b);
    inline JsonValue& operator =(float f);
    inline JsonValue& operator =(double d);
    inline JsonValue& operator =(const std::string &str);
    inline JsonValue& operator =(std::string &&str);
    JsonValue& operator =(const JsonObject& json_object);
    JsonValue& operator =(const JsonArray& json_array);
    JsonValue& operator =(JsonObject&& json_object);
    JsonValue& operator =(JsonArray&& json_array);

    JsonValueType valueType() const { return d_valueType; }

    inline bool isNull() const;
    inline bool isBoolean() const;
    inline bool isFloat() const;
    inline bool isDouble() const;
    inline bool isString() const;
    inline bool isArray() const;
    inline bool isObject() const;

    bool toBoolean() const;
    float toFloat() const;
    double toDouble() const;
    std::string toString() const;
    JsonArray toArray() const;
    JsonObject toObject() const;

    inline const std::string& const_string() const;
    inline const JsonArray& const_array() const;
    inline const JsonObject& const_object() const;

    inline std::string* mutable_string();
    inline JsonArray*   mutable_array();
    inline JsonObject*  mutable_object();

    void parseJsonValue(JsonIStream& charSeq);
    bool parseFromInputStream(JsonIStream &charSeq);
    bool serializeToOStream(std::ostream* os, int tab_size) const;
private:
    JsonValueType d_valueType;
    union {
        bool         d_boolValue;
        float        d_floatValue;
        double       d_doubleValue;
        JsonObject*  d_objectValue;
        JsonArray*   d_arrayValue;
        std::string* d_stringValue;
    };
};

JsonValue &JsonValue::operator =(bool b)
{
    this->clear();
    this->d_valueType = JSON_BOOLEAN;
    this->d_boolValue = b;
    return *this;
}

JsonValue &JsonValue::operator =(float f)
{
    this->clear();
    this->d_valueType = JSON_FLOAT;
    this->d_floatValue = f;
    return *this;
}

JsonValue &JsonValue::operator =(double d)
{
    this->clear();
    this->d_valueType = JSON_DOUBLE;
    this->d_doubleValue = d;
    return *this;
}

JsonValue &JsonValue::operator =(const std::string &str)
{
    this->clear();
    this->d_valueType = JSON_STRING;
    this->d_stringValue = new std::string(str);
    return *this;
}

JsonValue &JsonValue::operator =(std::string &&str)
{
    this->clear();
    this->d_valueType = JSON_STRING;
    this->d_stringValue = new std::string(std::move(str));
    return *this;
}

bool JsonValue::isNull() const
{
    return d_valueType == JSON_NULL;
}

bool JsonValue::isBoolean() const
{
    return d_valueType == JSON_BOOLEAN;
}

bool JsonValue::isFloat() const
{
    return d_valueType == JSON_FLOAT;
}

bool JsonValue::isDouble() const
{
    return d_valueType == JSON_DOUBLE;
}

bool JsonValue::isString() const
{
    return d_valueType == JSON_STRING;
}

bool JsonValue::isArray() const
{
    return d_valueType == JSON_ARRAY;
}

bool JsonValue::isObject() const
{
    return d_valueType == JSON_OBJECT;
}

const std::string &JsonValue::const_string() const
{
    return *d_stringValue;
}

const JsonArray &JsonValue::const_array() const
{
    return *d_arrayValue;
}

const JsonObject &JsonValue::const_object() const
{
    return *d_objectValue;
}

std::string *JsonValue::mutable_string()
{
    return d_stringValue;
}

JsonArray *JsonValue::mutable_array()
{
    return d_arrayValue;
}

JsonObject *JsonValue::mutable_object()
{
    return d_objectValue;
}

END_JSON_NAMESPACE

JSON_NAMESPACE::JsonValue operator"" _json(const char* json, std::size_t);

#endif // JSONVALUE_H
