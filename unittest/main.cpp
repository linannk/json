#include "jsonvalue.h"
#include "jsonobject.h"
#include "jsonarray.h"
#include "io/jsonstringistream.h"
#include "io/jsonutf8fileistream.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

json::JsonValue operator"" _json(const char* str, std::size_t i)
{
    std::cout << str << std::endl;
    return json::JsonValue();
}

int main(int argc, char *argv[])
{
    auto j = R"({"name": "linan"})"_json;

    json::JsonObject json_object;
    const char* platform = "linux_x86";
    const char* workdir = ".";
    const char* json_file = "C:\\Users\\linan\\Desktop\\1.json";

    json::JsonUtf8FileIStream json_file_seq(json_file);

    auto beg = std::chrono::high_resolution_clock::now();
    json_object.parseFromInputStream(json_file_seq);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count() << std::endl;

    json_object.serializeToOStream(&std::cout, 0);

    std::cout << std::endl;
    return 0;
}
