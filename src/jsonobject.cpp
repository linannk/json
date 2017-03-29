#include "jsonobject.h"
#include "jsonvalue.h"
#include "io/jsonistream.h"
#include <ctype.h>
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

void JsonObject::parseJsonObject(JsonIStream &charSeq, bool parseLeadingChar)
{
    char c = 0;
    if (parseLeadingChar)
    {
        //![0] FUNC_STEP0: EXPECT char is '{'
        for (;;)
        {
            c = charSeq.getChar();
            if (c == JSON_OPENBRACE) {
                goto FUNC_STEP1;
            }
            else if (isspace(c)) {
                continue;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                throw std::runtime_error("Expected '{', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
            }
        }
    }

FUNC_STEP1:
    for (;;)
    {
        c = charSeq.getChar();
        if (c == JSON_QUOTE) {
            goto FUNC_STEP2;
        }
        else if (c == JSON_CLOSEBRACE) {
            goto FUNC_STEP6;
        }
        else if (isspace(c)) {
            continue;
        }
        else if (c == -1) {
            throw std::runtime_error("Unexpected end of JsonInputStream");
        }
        else {
            throw std::runtime_error(std::string("Expected '\"' or '}', but \"") + charSeq.json_invalid_chars(c) + "\" is encountered.");
        }
    }

FUNC_STEP2:
    {
        std::string key;
        for (;;)
        {
            c = charSeq.getChar();
            if (c == '\\')
            {
                c = charSeq.getChar();
                if (c == -1) {
                    throw std::runtime_error("Unexpected end of JsonInputStream");
                }
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
            else if (c == '\"') {
                goto FUNC_STEP3;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                key.push_back(c);
                const int char_count = charSeq.encode_char_count(c);
                for (int i = 1; i < char_count; ++i) {
                    if ((c = charSeq.getChar()) != -1) {
                        key.push_back(c);
                    }
                    else {
                        //! Todo: add more information about the error.
                        throw std::runtime_error("Unexpected end of JsonInputStream");
                    }
                }
            }
        }

FUNC_STEP3:
        for (;;) {
            c = charSeq.getChar();
            if (c == ':') {
                goto FUNC_STEP4;
            }
            else if (isspace(c)) {
                continue;
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
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
    for (;;) {
        c = charSeq.getChar();
        if (c == JSON_COMMA) {
            goto FUNC_STEP1;
        }
        else if (c == JSON_CLOSEBRACE) {
            goto FUNC_STEP6;
        }
        else if (isspace(c)) {
            continue;
        }
        else if (c == -1) {
            throw std::runtime_error("Unexpected end of JsonInputStream");
        }
        else {
            throw std::runtime_error("Expected '}' or ',', but \"" + charSeq.json_invalid_chars(c) + "\" is encountered.");
        }
    }

FUNC_STEP6:
    //! Todo: Check following char sequence.
    ;
}

bool JsonObject::parseFromInputStream(JsonIStream &charSeq)
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

bool JsonObject::serializeToOStream(std::ostream * os, int tab_size) const
{
    *os << '{' << std::endl;
    for (auto i = this->begin(); i != this->end(); ++i) {
        for (int j = 0; j < tab_size + JSON_TAB_OFFSET; ++j) {
            *os << ' ';
        }
        *os << '\"';
        for (auto ch : i->first) {
            if (ch == '\"') {
                *os << "\\\"";
            }
            else if (ch == '\\') {
                *os << "\\\\";
            }
            else {
                *os << ch;
            }
        }
        *os << "\": ";
        i->second.serializeToOStream(os, tab_size);
        
        auto tmp = i;
        ++tmp;
        if (tmp != this->end()) {
            *os << ',';
        }
        *os << std::endl;
    }
    for (int j = 0; j < tab_size; ++j) {
        *os << ' ';
    }
    *os << '}';
    return os->good();
}

END_JSON_NAMESPACE
