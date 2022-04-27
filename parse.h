
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <string>
#include <string_view>

namespace zade {
    class tokenizer {
    
    private:
        std::string_view m_str_slice;
    public:
        tokenizer(std::string_view str);
        std::string_view content();
        ~tokenizer();
    };
}
