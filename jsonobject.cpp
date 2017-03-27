#include "jsonobject.h"
#include "jsonvalue.h"
#include "jsoncharseq.h"
#include "jsoncode.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

void JsonObject::parseJsonObject(JsonCharSeq &charSeq, bool parseLeadingChar)
{
    char c = 0;
    if (parseLeadingChar)
    {
        //![0] FUNC_STEP0: EXPECT char is '{'
        while((c = charSeq.getChar()) != -1)
        {
            if (c == '{') {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else {
                throw std::runtime_error("Expected '{', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
            }
        }
    }

FUNC_STEP1:
    while((c = charSeq.getChar()) != -1) {
        if (c == '\"') {
            goto FUNC_STEP2;
        }
        else if (isspace(c)) {
            continue;
        }
        else {
            throw std::runtime_error(std::string("Expected '\"', but \"") + charSeq.json_invalid_chars(c) + "\" is encountered.");
        }
    }

FUNC_STEP2:
    {
        std::string key;
        while((c = charSeq.getChar()) != -1) {
            if (c == '\\') {
                if ((c = charSeq.getChar()) != -1) {
                    switch(c) {
                    case '\"':
                        key.push_back('\"');
                        break;
                    case '\\':
                        key.push_back('\\');
                        break;
                    case '/':
                        //! \/ ??
                        key.push_back('/');
                        break;
                    case 'b':
                        key.push_back('\b');
                        break;
                    case 'f':
                        key.push_back('\f');
                        break;
                    case 'n':
                        key.push_back('\n');
                        break;
                    case 'r':
                        key.push_back('\r');
                        break;
                    case 't':
                        key.push_back('\t');
                        break;
                    case 'u':
                        std::cerr << "\\u IS NOT IMPLEMENTED YET" << std::endl;
                        key.push_back('\\');
                        key.push_back('u');
                        break;
                    default:
                        //! Todo: add more details about the error.
                        throw std::runtime_error("Unexpected char encountered: " + charSeq.json_invalid_chars(c));
                        break;
                    }
                }
                else {
                    throw std::runtime_error("Unexpected end of char sequence.");
                }
            }
            else if (c == '\"') {
                goto FUNC_STEP3;
            }
            else {
                key.push_back(c);
                for (int i = 1; i < charSeq.json_char_count(c); ++i) {
                    if ((c = charSeq.getChar()) != -1) {
                        key.push_back(c);
                    }
                    else {
                        //! Todo: add more information about the error.
                        throw std::runtime_error("Unexpected end of char sequence.");
                    }
                }
            }
        }

FUNC_STEP3:
        while((c = charSeq.getChar())) {
            if (c == ':') {
                goto FUNC_STEP4;
            }
            else if (isspace(c)) {
                continue;
            }
            else {
                throw std::runtime_error("Expected ':', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
            }
        }

FUNC_STEP4:
        JsonValue value;
        value.parseJsonValue(charSeq);
        this->emplace(std::move(key), std::move(value));
    }

    //! [5] FUNC_STEP5: EXPECT chars are ',' and '}'
    while((c = charSeq.getChar()) != -1) {
        if (c == ',') {
            goto FUNC_STEP1;
        }
        else if (c == '}') {
            goto FUNC_STEP6;
        }
        else if (isspace(c)) {
            continue;
        }
        else {
            throw std::runtime_error("Expected '}' or ',', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
        }
    }

FUNC_STEP6:
    //! Todo: Check following char sequence.
    ;
}

bool JsonObject::parseFromCharSeq(JsonCharSeq &charSeq)
{
    try {
        JsonObject tmpObject;
        tmpObject.parseJsonObject(charSeq);
        *this = std::move(tmpObject);
        return true;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

END_JSON_NAMESPACE
