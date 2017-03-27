#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include "jsondef.h"
#include <map>
#include <string>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonCharSeq;

class JsonObject {
public:
    /*!
     * \brief map_type type alias
     */
    typedef std::map<std::string, JsonValue> map_type;

    /*!
     * \brief The iterator class
     */
    class iterator {
    public:
        iterator();
        iterator(const iterator& other) : d_iter(other.d_iter) {}
        iterator(map_type::iterator iter) : d_iter(iter) {}
        iterator& operator =(const iterator& other);
        bool operator ==(const iterator& other) const;
        bool operator !=(const iterator& other) const;
        iterator& operator++();
        iterator& operator--();
        map_type::pointer operator->();
        map_type::reference operator*();
    private:
        map_type::iterator d_iter;
    };

    /*!
     * \brief The const_iterator class
     */
    class const_iterator {
    public:
        const_iterator();
        const_iterator(const const_iterator& other) : d_iter(other.d_iter) {}
        const_iterator(map_type::const_iterator iter) : d_iter(iter) {}
        const_iterator& operator =(const const_iterator& other);
        bool operator ==(const const_iterator& other) const;
        bool operator !=(const const_iterator& other) const;
        const_iterator& operator++();
        const_iterator& operator--();
        map_type::const_pointer operator->();
        map_type::const_reference operator*();
    private:
        map_type::const_iterator d_iter;
    };

    /*!
     * \brief JsonObject
     */
    JsonObject();

    /*!
     * \brief JsonObject
     * \param other
     */
    JsonObject(const JsonObject& other);

    /*!
     * \brief JsonObject
     * \param other
     */
    JsonObject(JsonObject&& other);

    ~JsonObject();

    JsonObject& operator =(const JsonObject &other);
    JsonObject& operator =(JsonObject &&other);

    bool isEmpty() const { return d_entries.empty(); }
    bool operator ==(const JsonObject& other) const;
    bool operator !=(const JsonObject& other) const;
    bool hasKey(const std::string& key) const;

    const map_type& map() const { return d_entries; }
    map_type& map() { return d_entries; }

    JsonValue& operator[](const std::string &key);
    iterator find(const std::string &key);
    const_iterator find(const std::string &key) const;

    inline iterator begin();
    inline iterator end();
    inline const_iterator begin() const;
    inline const_iterator end() const;
    inline const_iterator cbegin() const;
    inline const_iterator cend() const;

    void parseJsonObject(JsonCharSeq& charSeq, bool parseLeadingChar = true);
    bool parseFromCharSeq(JsonCharSeq& charSeq);
private:
    map_type d_entries;
};

JsonObject::iterator JsonObject::begin()
{
    return iterator(d_entries.begin());
}

JsonObject::iterator JsonObject::end()
{
    return iterator(d_entries.end());
}

JsonObject::const_iterator JsonObject::begin() const
{
    return const_iterator(d_entries.begin());
}

JsonObject::const_iterator JsonObject::end() const
{
    return const_iterator(d_entries.end());
}

JsonObject::const_iterator JsonObject::cbegin() const
{
    return const_iterator(d_entries.end());
}

END_JSON_NAMESPACE

#endif // JSONOBJECT_H
