#include "jsonarray.h"
#include "jsonvalue.h"
#include "jsoninputstream.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

void JsonArray::parseJsonArray(JsonInputStream &charSeq, bool parseLeadingChar)
{
    int c = 0;
    if (parseLeadingChar)
    {
        //![0] FUNC_STEP0: EXPECT char is '['
        for (;;)
        {
            c = charSeq.getChar();
            if (c == JSON_OPENBRACKET) {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::runtime_error(std::string("Unexpected char: '") + charSeq.json_invalid_chars(c) + "' encountered.");
            }
        }
    }

FUNC_STEP1:
    {
        JsonValue value;
        value.parseJsonValue(charSeq);
        this->push_back(std::move(value));
        for (;;) {
            c = charSeq.getChar();
            if (c == JSON_COMMA) {
                goto FUNC_STEP1;
            }
            else if (c == JSON_CLOSEBRACKET) {
                goto FUNC_STEP2;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::logic_error("Unexpected char: '" + charSeq.json_invalid_chars(c) + "' encountered.");
            }
        }
    }

FUNC_STEP2:;
}

bool JsonArray::parseFromInputStream(JsonInputStream &charSeq)
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
