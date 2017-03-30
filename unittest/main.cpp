#include "jsonvalue.h"
#include "jsonobject.h"
#include "jsonarray.h"
#include "io/jsonstringistream.h"
#include "io/jsonutf8fileistream.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

#define BEGIN_CHRONO { auto beg_chrono_time = std::chrono::high_resolution_clock::now();

#define END_CHRONO \
    auto end_chrono_time = std::chrono::high_resolution_clock::now(); \
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_chrono_time - beg_chrono_time).count() << std::endl; }

int main(int argc, char *argv[])
{
    auto j = "{\"name\":\"linannk\"}"_json;

    std::cout << j["name"].toString() << std::endl;

    json::JsonObject2 j2;
    j2.emplace(std::string("123"), std::string("123"));

    return 0;
}
