#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include "jsondef.h"
#include <map>
#include <string>
#include <initializer_list>
#include <ostream>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonIStream;

class JsonObject {
public:
    typedef std::map<std::string, JsonValue> container_type;

    JsonObject();
    JsonObject(const JsonObject& other);
    JsonObject(JsonObject &&other);

    JsonObject& operator = (const JsonObject& other);
    JsonObject& operator = (JsonObject&& other);

    JsonObject(const JsonValue& value);
    JsonObject(JsonValue&& vlaue);

    JsonObject& operator = (const JsonValue& value);
    JsonObject& operator = (JsonValue&& value);

    bool operator == (const JsonObject& other) const;
public:
    typedef container_type::iterator iterator;
    typedef container_type::reverse_iterator reverse_iterator;
    typedef container_type::const_iterator const_iterator;
    typedef container_type::const_reverse_iterator const_reverse_iterator;

    iterator begin() { return d_map.begin(); }
    iterator end() { return d_map.end(); }
    reverse_iterator rbegin() { return d_map.rbegin(); }
    reverse_iterator rend() { return d_map.rend(); }

    const_reverse_iterator rbegin() const { return d_map.rbegin(); }
    const_reverse_iterator rend() const { return d_map.rend(); }
    const_reverse_iterator crbegin() const { return d_map.crbegin(); }
    const_reverse_iterator crend() const { return d_map.crend(); }

    const_iterator begin() const { return d_map.begin(); }
    const_iterator end() const { return d_map.end(); }
    const_iterator cbegin() const { return d_map.cbegin(); }
    const_iterator cend() const { return d_map.cend(); }

    iterator insert(const std::string& key, const JsonValue& value);

    iterator find(const std::string& key);
    const_iterator find(const std::string& key) const;

#ifdef JSON_DEBUG
    bool contains_recurse(const JsonValue* value) const;
    bool contains_recurse(const JsonObject* value) const;
#endif // JSON_DEBUG

public:
    JsonValue& operator[](const std::string& key);
    bool hasKey(const std::string& key) const;
    bool contains(const std::string& key) const;
public:
    void parseJsonObject(JsonIStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonIStream& charSeq);
    bool serializeToOStream(std::ostream* os, int tab_size) const;

private:
    container_type d_map;
};

END_JSON_NAMESPACE

#endif // JSONOBJECT_H
