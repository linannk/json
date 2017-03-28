#include "jsonvalue.h"
#include "jsonobject.h"
#include "jsonarray.h"
#include "jsonstringcharseq.h"
#include "jsonfilecharseq.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

int main(int argc, char *argv[])
{
    json::JsonObject json_object;
    const char* platform = "linux_x86";
    const char* workdir = ".";
    const char* json_file = "/home/linan/Desktop/1.json";

    json::JsonFileCharSeq json_file_seq(json_file);

    auto beg = std::chrono::high_resolution_clock::now();
    json_object.parseFromInputStream(json_file_seq);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count() << std::endl;

    return 0;
    auto p_iter = json_object.find(platform);
    if (p_iter == json_object.end()) {
        std::cerr << platform << "is not found in json file." << std::endl;
        return 1;
    }

    std::cout << json_object["version"].toString() << std::endl;
    std::cout << p_iter->first << std::endl;
    for (const auto& p : p_iter->second.toObject()) {
        std::cout << "  " << p.first << std::endl;
        for (const auto& l : p.second.toObject()) {
            std::cout << "    " << l.first << std::endl;
            for (const auto& i : l.second.toObject()) {
                std::cout << "      " << i.first << " : " << i.second.toString() << std::endl;
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
