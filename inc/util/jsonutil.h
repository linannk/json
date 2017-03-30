#ifndef JSONUTIL_H
#define JSONUTIL_H
#include "../jsondef.h"
#include <string>

BEGIN_JSON_NAMESPACE
class JsonIStream;

void utilSkipComment(JsonIStream& charSeq);

std::string utilGetString(JsonIStream& charSeq);

END_JSON_NAMESPACE

#endif // JSONUTIL_H