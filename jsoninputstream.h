#ifndef JSONINPUTSTREAM_H
#define JSONINPUTSTREAM_H
#include "jsondef.h"
#include <string>

BEGIN_JSON_NAMESPACE
class JsonInputStream {
public:
    /*!
     * \brief ~JsonCharSeq
     * Virtual destructor.
     */
    virtual ~JsonInputStream() {}

    /*!
     * \brief getChar get current char and move char-pointer to next
     * \return current char, if no more bytes available it should return -1
     */
    virtual int getChar() = 0;

    /*!
     * \brief ugetChar backup a char
     * \return The return value is ignored.
     */
    virtual int ungetChar() = 0;

//    /*!
//     * \brief next
//     * \param data
//     * \return
//     */
//    virtual size_t next(void** data) = 0;

//    /*!
//     * \brief backUp
//     * \param count
//     */
//    virtual void backUp(size_t count) = 0;

    virtual int json_char_count(int c);
    std::string json_invalid_chars(int c);
};

END_JSON_NAMESPACE

#endif // JSONINPUTSTREAM_H
