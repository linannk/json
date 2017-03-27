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

    char getChar() {
        if (!d_fp) {
            return 0;
        }
        int c = fgetc(d_fp);
        if (c == -1) {
            return 0;
        }
        else {
            return static_cast<char>(c);
        }
    }

private:
    FILE* d_fp;
};
END_JSON_NAMESPACE

#endif // JSONFILECHARSEQ_H
