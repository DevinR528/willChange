
// vim: set tabstop=4 expandtab smarttab

#include "fmt/format.h"
#include "lex.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

int main(int argc, char const* argv[]) {
    if (argc <= 1) {
        fmt::print(
            "usage: zadec -i path/to/file.zd [-o out/file]\n"
            "file: {}\n",
            argv[0]);
        return 0;
    }

    std::string input;
    if (strcmp(argv[1], "-i") == 0) {
        input = std::string(argv[2]);
    } else {
        fmt::print(
            "expected input file\n"
            "usage: zadec -i path/to/file.zd [-o out/file]\n"
            "found: {}\n",
            argv[1]);
        return 0;
    }

    auto tok = zade::tokenizer(input);

    fmt::print("bout to parse this '{}'\n", tok.content());
    fmt::print("bout to parse this '{}'\n", std::filesystem::current_path().string());

    return 0;
}
