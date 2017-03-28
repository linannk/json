#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include "jsondef.h"
#include <map>
#include <string>

BEGIN_JSON_NAMESPACE
class JsonValue;
class JsonIStream;

class JsonObject
    : public std::map<std::string, JsonValue>
{
public:
    void parseJsonObject(JsonIStream& charSeq, bool parseLeadingChar = true);
    bool parseFromInputStream(JsonIStream& charSeq);
};

END_JSON_NAMESPACE

#endif // JSONOBJECT_H
