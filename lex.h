
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace zade {
    struct Token {
    private:
    public:
    };

    class tokenizer {

    private:
        std::string_view m_str_slice;
        std::vector<Token> m_tkns {};

        std::optional<Token> next_tkn() noexcept;
        std::optional<Token> comment();
        std::optional<Token> whitespace();
        std::optional<Token> identifier();
        std::optional<Token> numeric_lit();

    public:
        tokenizer(std::string_view str);
        std::string_view content() noexcept;
        std::vector<Token> lex_input();
        ~tokenizer();
    };
}
