#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include "jsondef.h"
#include <map>
#include <string>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonCharSeq;

class JsonObject
    : public std::map<std::string, JsonValue>
{
public:
    void parseJsonObject(JsonCharSeq& charSeq, bool parseLeadingChar = true);
    bool parseFromCharSeq(JsonCharSeq& charSeq);
};

END_JSON_NAMESPACE

#endif // JSONOBJECT_H
