#include <iostream>
#include "jsonvalue.h"
#include "jsonobject.h"
#include "jsonarray.h"
#include "jsonstringcharseq.h"
#include "jsonfilecharseq.h"

int main(int argc, char *argv[])
{
    json::JsonObject json_object;
    json::JsonFileCharSeq json_file_seq("C:\\Users\\linan\\Desktop\\1.json");
    json_object.parseFromCharSeq(json_file_seq);
    for (auto i : json_object) {
        std::cout << i.first << std::endl;
        for (auto j : i.second.toObject()) {
            std::cout << '\t' << j.first << "=>";
            for (auto k : j.second.toArray()) {
                std::cout << k.toString() << ", ";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
