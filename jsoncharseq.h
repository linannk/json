#ifndef JSONCHARSEQ_H
#define JSONCHARSEQ_H
#include "jsondef.h"

BEGIN_JSON_NAMESPACE
class JsonCharSeq {
public:
    virtual ~JsonCharSeq() {}
    virtual char getChar() = 0;
};
END_JSON_NAMESPACE

#endif // JSONCHARSEQ_H
