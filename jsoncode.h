#ifndef JSONCODE_H
#define JSONCODE_H
#include "jsondef.h"
#include <string>

BEGIN_JSON_NAMESPACE

class JsonInputStream;

int json_char_count(int c);
std::string get_invalid_char(int c, JsonInputStream &charSeq);

END_JSON_NAMESPACE

#endif // JSONCODE_H
