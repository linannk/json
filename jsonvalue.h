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
class JsonValue;
class JsonArray;
class JsonCharSeq;

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

    void parseJsonValue(JsonCharSeq& charSeq);
    bool parseFromCharSeq(JsonCharSeq &charSeq);
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
END_JSON_NAMESPACE

#endif // JSONVALUE_H