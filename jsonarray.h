#ifndef JSONARRAY_H
#define JSONARRAY_H
#include "jsondef.h"
#include <vector>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonCharSeq;

class JsonArray
    : public std::vector<JsonValue>
{
public:
    void parseJsonArray(JsonCharSeq& charSeq, bool parseLeadingChar = true);
    bool parseFromCharSeq(JsonCharSeq& charSeq);
};

END_JSON_NAMESPACE

#endif // JSONARRAY_H
