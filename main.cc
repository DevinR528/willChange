// To use the fmt library as a header only library...
#define FMT_HEADER_ONLY

#include "./include/fmt-8.1.1/include/fmt/format.h"
#include "parse.h"

#include <iostream>
#include <string>
#include <string_view>

int main(int argc, char const* argv[]) {
    if (argc <= 1) {
        fmt::print(
            "usage: zadec -i path/to/file.zd [-o out/file]\nfile: {}\n",
            argv[0]);
        return 0;
    }

    std::string input;
    if (strcmp(argv[1], "-i") == 0) {
        input = std::string(argv[2]);
    } else {
        fmt::print(
            "expected input file\nusage: zadec -i path/to/file.zd [-o out/file]\nfound: {}\n",
            argv[1]);
        return 0;
    }

    auto tok = zade::tokenizer(input);

    fmt::print("bout to parse this '{}'\n", tok.content());

    return 0;
}
