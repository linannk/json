#ifndef JSONARRAY_H
#define JSONARRAY_H
#include "jsondef.h"
#include <vector>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonInputStream;

class JsonArray
    : public std::vector<JsonValue>
{
public:
    void parseJsonArray(JsonInputStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonInputStream& charSeq);
};

END_JSON_NAMESPACE

#endif // JSONARRAY_H
