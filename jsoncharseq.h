#ifndef JSONCHARSEQ_H
#define JSONCHARSEQ_H
#include "jsondef.h"
#include <string>
BEGIN_JSON_NAMESPACE
class JsonCharSeq {
public:
    virtual ~JsonCharSeq() {}
    virtual int getChar() = 0;
    virtual int json_char_count(int c);

    std::string json_invalid_chars(int c);
};

END_JSON_NAMESPACE

#endif // JSONCHARSEQ_H
