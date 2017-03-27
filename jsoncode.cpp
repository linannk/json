#include "jsoncode.h"
#include "jsoncharseq.h"

BEGIN_JSON_NAMESPACE
int json_char_count(int c)
{
    return 1;
}

std::string get_invalid_char(int c, JsonCharSeq &charSeq)
{
    std::string invalid_char;
    invalid_char.reserve(128);
    invalid_char.push_back(c);
    for (int i = 1; i < json_char_count(c); ++i) {
        invalid_char.push_back(charSeq.getChar());
    }
    return invalid_char;
}

END_JSON_NAMESPACE
