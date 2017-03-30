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

    /*!
     * \brief JsonArray Construct an empty JsonArray object.
     */ JsonArray();

    /*!
     * \brief JsonArray Construct an JsonArray object and copy elements from the given JsonArray object.
     */
    JsonArray(const JsonArray& other);

    JsonArray(JsonArray&& lhs);

    JsonArray(const container_type& vec)
        : d_arr(vec)
    {}

    JsonArray(container_type&& vec)
        : d_arr(std::move(vec))
    {}

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
    JsonValue& operator[](std::size_t idx) { return d_arr[idx]; }
    const JsonValue& operator[](std::size_t idx) const { return d_arr[idx]; }

    JsonValue& front() { return d_arr.front(); }
    JsonValue& back() { return d_arr.back(); }

    const JsonValue& front() const { return d_arr.front(); }
    const JsonValue& back() const { return d_arr.back(); }

    JsonValue* data() { return d_arr.data(); }
    const JsonValue* data() const { return d_arr.data(); }

    size_t size() const { return d_arr.size(); }
    bool   empty() const { return d_arr.empty(); }
public:
    void push_back(const JsonValue& value) { d_arr.push_back(value); }
    void push_back(JsonValue&& value) { d_arr.emplace_back(std::move(value)); }
    void pop_back() { d_arr.pop_back(); }

    void clear() { d_arr.clear(); }
    void swap(JsonArray& other) { d_arr.swap(other.d_arr); }
    void swap(std::vector<JsonValue>& other) { d_arr.swap(other); }

    iterator insert(iterator iter, const JsonValue& value) { return d_arr.insert(iter, value); }
    iterator insert(iterator iter, JsonValue&& value) { return d_arr.insert(iter, std::move(value)); }
    void insert(iterator iter, size_t n, const JsonValue& value) { d_arr.insert(iter, n, value); }
    void insert(iterator iter, std::initializer_list<JsonValue> list) { d_arr.insert(iter, list); }

    template<typename InputIterator>
    void insert(iterator iter, InputIterator beg, InputIterator end)
    {
        d_arr.insert(iter, beg, end);
    }

public:
    void parseJsonArray(JsonIStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonIStream& charSeq);
    bool serializeToOStream(std::ostream* os, int tab_size) const;
private:
    container_type d_arr;
};

template<typename InputIterator>
inline JsonArray::JsonArray(InputIterator beg, InputIterator end)
    : d_arr(beg, end)
{
}

//template<typename InputIterator>
//inline JsonArray::iterator JsonArray::insert(JsonArray::iterator iter, InputIterator beg, InputIterator end)
//{
//    return d_arr.insert(iter, beg, end);
//}

END_JSON_NAMESPACE

#endif // JSONARRAY_H
