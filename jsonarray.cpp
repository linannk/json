#include "jsonarray.h"
#include "jsonvalue.h"
#include "jsoncharseq.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

void JsonArray::parseJsonArray(JsonCharSeq &charSeq, bool parseLeadingChar)
{
    char c = 0;
    if (parseLeadingChar) {
    //![0] FUNC_STEP0: EXPECT char is '['
        while ((c = charSeq.getChar())) {
            if (isspace(c)) {
                continue;
            }
            else if (c == '[') {
                goto FUNC_STEP1;
            }
            else {
                throw std::logic_error(std::string("Unexpected char: '") + c + "' encountered.");
            }
        }
    }

FUNC_STEP1:
    {
        JsonValue value;
        value.parseJsonValue(charSeq);
        this->push_back(std::move(value));
        while ((c = charSeq.getChar())) {
            if (c == ']') {
                goto FUNC_STEP2;
            }
            else if (c == ',') {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else {
                throw std::logic_error(std::string("Unexpected char: '") + c + "' encountered.");
            }
        }
    }

FUNC_STEP2:;
}

bool JsonArray::parseFromCharSeq(JsonCharSeq &charSeq)
{
    try {
        JsonArray tmpArray;
        tmpArray.parseJsonArray(charSeq);
        *this = std::move(tmpArray);
        return true;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

END_JSON_NAMESPACE
