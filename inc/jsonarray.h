#ifndef JSONARRAY_H
#define JSONARRAY_H
#include "jsondef.h"
#include <vector>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonIStream;

class JsonArray
    : public std::vector<JsonValue>
{
public:
    void parseJsonArray(JsonIStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonIStream& charSeq);
};

END_JSON_NAMESPACE

#endif // JSONARRAY_H
