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
    json::JsonObject j = R"({"name":"linane","age":123})"_json;
    j.serializeToOStream(&std::cout, 0);
    std::cout << std::endl;

    json::JsonArray json_array = R"(["linan", "lihong", "jhon"])"_json;
    json_array.serializeToOStream(&std::cout, 0);
    std::cout << std::endl;

    json::JsonValue json_value = R"("linannk")"_json;
    std::cout << json_value.serializeToOStream(&std::cout, 0);
    std::cout << std::endl;

    json_value["name"] = "linan";
    json_value["age"] = 13.0;

    json_value.serializeToOStream(&std::cout, 0);

    json::JsonObject h = std::move(json_value);
    json_value.serializeToOStream(&std::cout, 0);
    std::cout << std::endl;
    h.serializeToOStream(&std::cout, 0);
    std::cout << std::endl;
    return 0;
}
