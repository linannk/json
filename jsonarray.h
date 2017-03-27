#ifndef JSONARRAY_H
#define JSONARRAY_H
#include "jsondef.h"
#include <vector>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonCharSeq;

class JsonArray {
public:
    typedef std::vector<JsonValue> vec_type;

    /*!
     * \brief The iterator class
     */
    class iterator {
    public:
        iterator();
        iterator(const iterator& other) : d_iter(other.d_iter) {}
        iterator(vec_type::iterator iter) : d_iter(iter) {}
        iterator& operator =(const iterator& other);
        bool operator ==(const iterator& other) const;
        bool operator !=(const iterator& other) const;
        iterator& operator++();
        iterator& operator--();
        iterator& operator+=(std::size_t offset);
        iterator& operator-=(std::size_t offset);
        vec_type::pointer operator->();
        vec_type::reference operator*();
    private:
        vec_type::iterator d_iter;
        friend class JsonArray;
    };

    /*!
     * \brief The const_iterator class
     */
    class const_iterator {
    public:
        const_iterator();
        const_iterator(const const_iterator& other) : d_iter(other.d_iter) {}
        const_iterator(vec_type::const_iterator iter) : d_iter(iter) {}
        const_iterator& operator =(const const_iterator& other);
        bool operator ==(const const_iterator& other) const;
        bool operator !=(const const_iterator& other) const;
        const_iterator& operator++();
        const_iterator& operator--();
        const_iterator& operator+=(std::size_t offset);
        const_iterator& operator-=(std::size_t offset);
        vec_type::const_pointer operator->();
        vec_type::const_reference operator*();
    private:
        vec_type::const_iterator d_iter;
        friend class JsonArray;
    };

    JsonArray();
    JsonArray(const JsonArray &other);
    JsonArray(JsonArray &&other);
    JsonArray& operator =(const JsonArray &other);
    JsonArray& operator =(JsonArray &&other);

    bool operator ==(const JsonArray& other);
    bool operator !=(const JsonArray& other);

    std::size_t size() const;
    const JsonValue& operator[](std::size_t index) const;
    JsonValue& operator[](std::size_t index);

    void push_back(const JsonValue& value);
    void push_back(JsonValue&& value);
    void pop_back();

    void insert(iterator iter, JsonValue&& value);
    void insert(iterator iter, const JsonValue& value);
    void insert(iterator iter, std::size_t n, const JsonValue& value);

    inline iterator begin();
    inline iterator end();
    inline const_iterator begin() const;
    inline const_iterator end() const;
    inline const_iterator cbegin() const;
    inline const_iterator cend() const;

    void parseJsonArray(JsonCharSeq& charSeq, bool parseLeadingChar = true);
    bool parseFromCharSeq(JsonCharSeq& charSeq);
private:
    vec_type d_entries;
};

JsonArray::iterator JsonArray::begin()
{
    return iterator(d_entries.begin());
}

JsonArray::iterator JsonArray::end()
{
    return iterator(d_entries.end());
}

JsonArray::const_iterator JsonArray::begin() const
{
    return const_iterator(d_entries.begin());
}

JsonArray::const_iterator JsonArray::end() const
{
    return const_iterator(d_entries.end());
}

JsonArray::const_iterator JsonArray::cbegin() const
{
    return const_iterator(d_entries.cbegin());
}

JsonArray::const_iterator JsonArray::cend() const
{
    return const_iterator(d_entries.cend());
}

END_JSON_NAMESPACE

#endif // JSONARRAY_H
