#ifndef JSONARRAY_H
#define JSONARRAY_H
#include "jsondef.h"
#include <vector>
#include <ostream>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonIStream;

class JsonArray
{
public:
    typedef std::vector<JsonValue> container_type;

    JsonArray();
    JsonArray(const JsonArray& other);
    JsonArray(JsonArray&& lhs);
    JsonArray(const container_type& vec);
    JsonArray(container_type&& vec);

    JsonArray(const JsonValue& other);
    JsonArray(JsonValue &&other);

    JsonArray& operator = (const JsonArray& other);
    JsonArray& operator = (JsonArray&& other);

    JsonArray& operator = (const JsonValue& value);
    JsonArray& operator = (JsonValue&& value);

    bool operator == (const JsonArray& other) const;

public:
    typedef container_type::iterator iterator;
    typedef container_type::reverse_iterator reverse_iterator;
    typedef container_type::const_iterator const_iterator;
    typedef container_type::const_reverse_iterator const_reverse_iterator;

    iterator begin() { return d_arr.begin(); }
    iterator end() { return d_arr.end(); }
    reverse_iterator rbegin() { return d_arr.rbegin(); }
    reverse_iterator rend() { return d_arr.rend(); }

    const_reverse_iterator rbegin() const { return d_arr.rbegin(); }
    const_reverse_iterator rend() const { return d_arr.rend(); }
    const_reverse_iterator crbegin() const { return d_arr.crbegin(); }
    const_reverse_iterator crend() const { return d_arr.crend(); }

    const_iterator begin() const { return d_arr.begin(); }
    const_iterator end() const { return d_arr.end(); }
    const_iterator cbegin() const { return d_arr.cbegin(); }
    const_iterator cend() const { return d_arr.cend(); }

public:
    JsonValue& operator[](std::size_t idx);
    const JsonValue& operator[](std::size_t idx) const;
    JsonValue& front();
    JsonValue& back();
    const JsonValue& front() const;
    const JsonValue& back() const;
    size_t size() const;
    bool empty() const;
public:
    void push_back(const JsonValue& value);
    void push_back(JsonValue&& value);
    void pop_back();

    iterator insert(iterator iter, const JsonValue& value);
    iterator insert(iterator iter, JsonValue&& value);

#ifdef JSON_DEBUG
    bool contains_recurse(const JsonValue* value) const;
    bool contains_recurse(const JsonArray* arr) const;
#endif // JSON_DEBUG

public:
    void parseJsonArray(JsonIStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonIStream& charSeq);
    bool serializeToOStream(std::ostream* os, int tab_size) const;
private:
    container_type d_arr;
};

END_JSON_NAMESPACE

#endif // JSONARRAY_H
