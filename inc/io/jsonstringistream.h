#ifndef JSONSTRINGINPUTSTREAM_H
#define JSONSTRINGINPUTSTREAM_H
#include "jsonistream.h"
#include <string>
#include <stdexcept>
BEGIN_JSON_NAMESPACE
class JsonStringIStream : public JsonIStream {
public:
    explicit JsonStringIStream(const std::string& str)
        : d_idx(0)
        , d_str(str)
    {}

    explicit JsonStringIStream(const char* str)
        : d_idx(0)
        , d_str(str)
    {}

    int getChar() {
        return d_idx < d_str.size() ? d_str[d_idx++] : -1;
    }

    int ungetChar() {
        --d_idx;
        return 1;
    }

private:
    std::size_t d_idx;
    std::string d_str;
};

class JsonStringWrapperIStream : public JsonIStream {
public:
    explicit JsonStringWrapperIStream(const std::string& str)
        : d_str(str)
        , d_idx(0)
    { }

    int getChar() {
        return d_idx < d_str.size() ? d_str[d_idx++] : -1;
    }

    int ungetChar() {
        --d_idx;
        return 1;
    }
private:
    const std::string& d_str;
    std::size_t d_idx;
};

class JsonCStrWrapperIStream : public JsonIStream {
public:
    explicit JsonCStrWrapperIStream(const char* str, std::size_t size)
        : d_str(str)
        , d_idx(0)
        , d_size(size)
    {}

    int getChar() {
        return d_idx < d_size ? d_str[d_idx++] : -1;
    }

    int ungetChar() {
        --d_idx;
        return 1;
    }

private:
    const char* d_str;
    std::size_t d_idx;
    std::size_t d_size;
};

END_JSON_NAMESPACE

#endif // JSONSTRINGINPUTSTREAM_H
