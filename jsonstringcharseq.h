#ifndef JSONSTRINGCHARSEQ_H
#define JSONSTRINGCHARSEQ_H
#include "jsoninputstream.h"
#include <string>
#include <stdexcept>
BEGIN_JSON_NAMESPACE
class JsonStringCharSeq : public JsonInputStream {
public:
    explicit JsonStringCharSeq(const std::string& str)
        : d_idx(0)
        , d_str(str)
    {}

    explicit JsonStringCharSeq(const char* str)
        : d_idx(0)
        , d_str(str)
    {}

    int getChar() {
        if (d_idx < d_str.size()) {
            char ret = d_str[d_idx];
            ++d_idx;
            return ret;
        }
        else {
            throw std::runtime_error("Unexpected end of file");
        }
    }

    int ungetChar() {
        --d_idx;
        return 1;
    }

private:
    std::size_t d_idx;
    std::string d_str;
};
END_JSON_NAMESPACE

#endif // JSONSTRINGCHARSEQ_H
