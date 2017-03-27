#ifndef JSONFILECHARSEQ_H
#define JSONFILECHARSEQ_H
#include "jsoncharseq.h"
#include <stdio.h>

BEGIN_JSON_NAMESPACE
class JsonFileCharSeq : public JsonCharSeq
{
public:
    JsonFileCharSeq(const char* filename)
        : d_fp(fopen(filename, "rb"))
    { }

    ~JsonFileCharSeq() {
        if (d_fp) {
            fclose(d_fp);
        }
    }

    int getChar() {
        return d_fp ? fgetc(d_fp) : -1;
    }

private:
    FILE* d_fp;
};
END_JSON_NAMESPACE

#endif // JSONFILECHARSEQ_H
