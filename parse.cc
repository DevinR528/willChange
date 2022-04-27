
// vim: set tabstop=4 expandtab smarttab

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

tokenizer::tokenizer(std::string_view str) : m_str_slice {str} {}

tokenizer::~tokenizer() {}

std::string_view tokenizer::content() {
    return this->m_str_slice;
}

}  // namespace zade
