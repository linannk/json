#ifndef JSONSTRINGCHARSEQ_H
#define JSONSTRINGCHARSEQ_H
#include "jsoncharseq.h"
#include <string>
BEGIN_JSON_NAMESPACE
class JsonStringCharSeq : public JsonCharSeq {
public:
    explicit JsonStringCharSeq(const std::string& str)
        : d_idx(0)
        , d_str(str)
    {}

    explicit JsonStringCharSeq(const char* str)
        : d_idx(0)
        , d_str(str)
    {}

    char getChar() {
        if (d_idx < d_str.size()) {
            char ret = d_str[d_idx];
            ++d_idx;
            return ret;
        }
        return 0;
    }

private:
    std::size_t d_idx;
    std::string d_str;
};
END_JSON_NAMESPACE

#endif // JSONSTRINGCHARSEQ_H