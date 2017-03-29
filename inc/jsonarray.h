#ifndef JSONARRAY_H
#define JSONARRAY_H
#include "jsondef.h"
#include <vector>
#include <ostream>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonIStream;

class JsonArray
    : public std::vector<JsonValue>
{
public:
    JsonArray();
    JsonArray(const JsonValue& other);
    JsonArray(JsonValue&& other);
    JsonArray(size_t n, const JsonValue& value);

    template<typename InputIterator>
    JsonArray(InputIterator beg, InputIterator end)
        : std::vector<JsonValue>(beg, end)
    {}

    JsonArray& operator = (const JsonValue& value);
    JsonArray& operator = (JsonValue&& value);

    void parseJsonArray(JsonIStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonIStream& charSeq);

    bool serializeToOStream(std::ostream* os, int tab_size) const;
};

END_JSON_NAMESPACE

std::ostream& operator << (std::ostream& os, const JSON_NAMESPACE::JsonArray& array);

#endif // JSONARRAY_H
