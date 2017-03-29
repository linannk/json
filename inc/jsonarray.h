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
    JsonArray();
    JsonArray(const JsonArray& other);
    JsonArray(JsonArray&& lhs);

    JsonArray(const JsonValue& other);
    JsonArray(JsonValue &&other);

    JsonArray(size_t n, const JsonValue& value);

    template<typename InputIterator> inline
    JsonArray(InputIterator beg, InputIterator end);

    JsonArray& operator = (const JsonArray& other);
    JsonArray& operator = (JsonArray&& other);

    JsonArray& operator = (const JsonValue& value);
    JsonArray& operator = (JsonValue&& value);

    bool operator == (const JsonArray& other) const;

public:
    typedef std::vector<JsonValue>::iterator iterator;
    typedef std::vector<JsonValue>::reverse_iterator reverse_iterator;
    typedef std::vector<JsonValue>::const_iterator const_iterator;
    typedef std::vector<JsonValue>::const_reverse_iterator const_reverse_iterator;

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

    JsonValue* data();
    const JsonValue* data() const;

    size_t size() const;
public:
    void push_back(const JsonValue& value);
    void push_back(JsonValue&& value);
    void clear() { d_arr.clear(); }
    void swap(JsonArray& other);
    void insert(iterator iter, const JsonValue& value);
    void insert(iterator iter, JsonValue&& value);
    void insert(iterator iter, size_t n, const JsonValue& value);
    template<typename InputIterator>
    void insert(iterator iter, InputIterator beg, InputIterator end);
public:
    void parseJsonArray(JsonIStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonIStream& charSeq);
    bool serializeToOStream(std::ostream* os, int tab_size) const;
private:
    std::vector<JsonValue> d_arr;
};

template<typename InputIterator>
inline JsonArray::JsonArray(InputIterator beg, InputIterator end)
    : d_arr(beg, end)
{
}

template<typename InputIterator>
inline void JsonArray::insert(iterator iter, InputIterator beg, InputIterator end)
{
    d_arr.insert(iter, beg, end);
}

END_JSON_NAMESPACE

#endif // JSONARRAY_H
