#ifndef JSONUTF8FILECHARSEQ_H
#define JSONUTF8FILECHARSEQ_H
#include "jsonistream.h"
#include "charset/utf8_ctb.h"
#include <stdio.h>

BEGIN_JSON_NAMESPACE
class JsonUtf8FileIStream : public JsonIStream
{
public:
    JsonUtf8FileIStream(const char* filename)
        : d_fp(fopen(filename, "rb"))
    {
        //! Test BOM
        //! If the file has bom header, so it will be ignored.
        //! Utf8 file BOM header 
        //! 0xEF, 0xBB, 0xBF
        if (d_fp) {
            int c0 = fgetc(d_fp);
            if (c0 != 0xEF) {
                fseek(d_fp, -1, SEEK_CUR);
                return;
            }
            int c1 = fgetc(d_fp);
            if (c1 != 0xBB) {
                fseek(d_fp, -2, SEEK_CUR);
                return;
            }
            int c2 = fgetc(d_fp);
            if (c2 != 0xBF) {
                fseek(d_fp, -3, SEEK_CUR);
                return;
            }
        }
    }

    ~JsonUtf8FileIStream() {
        if (d_fp) {
            fclose(d_fp);
        }
    }

    int getChar() {
        return d_fp ? fgetc(d_fp) : -1;
    }

    int ungetChar() {
        if (d_fp) {
            fseek(d_fp, -1, SEEK_CUR);
        }
        return 1;
    }

    int encode_char_count(int c);

private:
    FILE* d_fp;
};

int JsonUtf8FileIStream::encode_char_count(int c)
{
    return utf8_byte_count(c);
}

END_JSON_NAMESPACE

#endif // JSONUTF8FILECHARSEQ_H
