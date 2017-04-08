#include "util\jsonutil.h"
#include "io/jsonistream.h"
#include <stdexcept>
#include <iostream>

BEGIN_JSON_NAMESPACE

void utilSkipComment(JsonIStream & charSeq) {
    int c = charSeq.getChar();
    if (c == '/') {
        for (;;) {
            c = charSeq.getChar();
            if (c == '\r' || c == '\n') {
                goto FUNC_NORMAL_RET;
            }
            else if (c == -1) {
                goto FUNC_ERROR_END_STREAM;
            }
            else {
                int char_count = charSeq.encode_char_count(c);
                for (int i = 1; i < char_count; ++i) {
                    c = charSeq.getChar();
                    if (c == -1) {
                        goto FUNC_ERROR_END_STREAM;
                    }
                }
            }
        }
    }
    else if (c == '*') {
        for (;;) {
            c = charSeq.getChar();
            if (c == '*') {
                c = charSeq.getChar();
                if (c == '/') {
                    goto FUNC_NORMAL_RET;
                }
                else if (c == '-1') {
                    goto FUNC_ERROR_END_STREAM;
                }
                else {
                    throw std::runtime_error("Expect \"/\", but " + charSeq.json_invalid_chars(c) + " is encountered.");
                }
            }
            else if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            else {
                int char_count = charSeq.encode_char_count(c);
                for (int i = 1; i < char_count; ++i) {
                    c = charSeq.getChar();
                    if (c == -1) {
                        throw std::runtime_error("Unexpected end of JsonInputStream");
                    }
                }
            }
        }
    }
    else if (c == -1) {
        goto FUNC_ERROR_END_STREAM;
    }
    else {
        throw std::runtime_error("Expect \"*\" or \"/\", but " + charSeq.json_invalid_chars(c) + " is encountered");
    }

FUNC_ERROR_END_STREAM:
    throw std::runtime_error("Unexpected end of JsonInputStream");
FUNC_NORMAL_RET:;
}

std::string utilGetString(JsonIStream & charSeq)
{
    std::string str;
    str.reserve(128);
    int c = 0;
    for (;;) {
        c = charSeq.getChar();
        if (c == -1) {
            throw std::runtime_error("Unexpected end of JsonInputStream");
        }
        else if (c == '\\') 
        {
            c = charSeq.getChar();
            if (c == -1) {
                throw std::runtime_error("Unexpected end of JsonInputStream");
            }
            switch (c) {
            case '\"':
                str.push_back('\"');
                break;
            case '\\':
                str.push_back('\\');
                break;
            case '/':
                //! \/ ??
                str.push_back('/');
                break;
            case 'b':
                str.push_back('\b');
                break;
            case 'f':
                str.push_back('\f');
                break;
            case 'n':
                str.push_back('\n');
                break;
            case 'r':
                str.push_back('\r');
                break;
            case 't':
                str.push_back('\t');
                break;
            case 'u':
                std::cerr << "\\u IS NOT IMPLEMENTED NOW" << std::endl;
                break;
            default:
                std::cerr << "Unknown char \\" << c << std::endl;
                break;
            }
        }
        else if (c == '\"') {
            goto FUNC_NORMAL_RET;
        }
        else {
            str.push_back(c);
            const int char_count = charSeq.encode_char_count(c);
            for (int i = 1; i < char_count; ++i) {
                c = charSeq.getChar();
                if (c == -1) {
                    throw std::runtime_error("Unexpected end of JsonInputStream");
                }
                else {
                    str.push_back(c);
                }
            }
        }
    }

FUNC_NORMAL_RET:
    return str;
}

END_JSON_NAMESPACE