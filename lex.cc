
// vim: set tabstop=4 expandtab smarttab

#include "macros.h"

#include <cctype>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace zade {
enum TokenKind {
    // A comment token
    COMMENT,
    // An integer literal
    INT_LIT,
    // A float literal
    FLOAT_LIT,
    // A char literal
    CHAR_LIT,
    // A string literal
    STR_LIT,
    // ;
    SEMI,
    // ,
    COMMA,
    // .
    DOT,
    // (
    OPEN_PAREN,
    // )
    CLOSE_PAREN,
    // {
    OPEN_BRACE,
    // }
    CLOSE_BRACE,
    // [
    OPEN_BRACKET,
    // ]
    CLOSE_BRACKET,
    // @
    AT,
    // #
    POUND,
    // ~
    TILDE,
    // ?
    QUESTION,
    // :
    COLON,
    // $
    DOLLAR,
    // =
    EQ,
    // !
    BANG,
    // <
    LESS,
    // >
    GREAT,
    // -
    MINUS,
    // &
    AND,
    // |
    OR,
    // +
    PLUS,
    // *
    STAR,
    // ^
    CARET,
    // %
    PERCENT,
    // This is incase we want error tolerance
    UNKNOWN
};

// A parsed token.
//
// This is only a length and kind, which tells what this range of text represents.
struct Token {
    enum CommentKind { MULTI_LINE, SINGLE_LINE };
    enum Base {
        // 0b
        BIN,
        // 0o
        OCTAL,
        // 0x
        HEX,
        // No prefix
        DEC
    };

    union TokenInfo {
        CommentKind comments;
        Base int_base;
    };

  private:
    TokenKind kind;
    size_t len;
    TokenInfo info;

  public:
    Token(TokenKind k, size_t len);
    ~Token();
};

Token::Token(TokenKind k, size_t l) : kind{k}, len{l} {}
Token::~Token() {}

// The lexer/tokenizer that turns input text into a `vector<Token>`.
class tokenizer {
    using none = std::optional<Token>;

  private:
    std::string_view m_str_slice;
    std::vector<Token> m_tkns{};

    std::optional<Token> next_tkn() noexcept;
    std::optional<Token> eat_tkn();
    std::optional<Token> comment();
    std::optional<Token> whitespace();
    std::optional<Token> identifier();
    std::optional<Token> numeric_lit();
    std::optional<Token> char_lit();
    std::optional<Token> string_lit();

  public:
    tokenizer(std::string_view str);
    std::string_view content() noexcept;
    std::vector<Token> lex_input();
    ~tokenizer();
};

tokenizer::tokenizer(std::string_view str) : m_str_slice{str} {}
tokenizer::~tokenizer() {}
std::string_view tokenizer::content() noexcept { return this->m_str_slice; }

std::optional<Token> tokenizer::comment() {
    auto token = Token { COMMENT, 1 };
    // eat first '/'
    this->m_str_slice = this->m_str_slice.substr(1);

    if (this->m_str_slice.empty()) return none{};

    if (*this->m_str_slice.begin() == '/') {
        TODO("still working on this");
    } else if (*this->m_str_slice.begin() == '*') {
        TODO("Multi line comments");
    } else {
        return none{};
    }
}

std::optional<Token> tokenizer::next_tkn() noexcept {
    if (this->m_str_slice.empty()) return none{};

    switch (*this->m_str_slice.begin()) {
        case '/':
            return this->comment();

        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
            return this->whitespace();

        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':  // Add emoji's and stuff?
            return this->identifier();

        case '0' ... '9':
            return this->numeric_lit();
        // TODO:
        // check for byte literals?
        // other special characters maybe?
        case '\'':
            return this->char_lit();
        case '"':
            return this->string_lit();

        // One-symbol tokens.
        case ';':
            return Token{SEMI, 1};
        case ',':
            return Token{COMMA, 1};
        case '.':
            return Token{DOT, 1};
        case '(':
            return Token{OPEN_PAREN, 1};
        case ')':
            return Token{CLOSE_PAREN, 1};
        case '{':
            return Token{OPEN_BRACE, 1};
        case '}':
            return Token{CLOSE_BRACE, 1};
        case '[':
            return Token{OPEN_BRACKET, 1};
        case ']':
            return Token{CLOSE_BRACKET, 1};
        case '@':
            return Token{AT, 1};
        case '#':
            return Token{POUND, 1};
        case '~':
            return Token{TILDE, 1};
        case '?':
            return Token{QUESTION, 1};
        case ':':
            return Token{COLON, 1};
        case '$':
            return Token{DOLLAR, 1};
        case '=':
            return Token{EQ, 1};
        case '!':
            return Token{BANG, 1};
        case '<':
            return Token{LESS, 1};
        case '>':
            return Token{GREAT, 1};
        case '-':
            return Token{MINUS, 1};
        case '&':
            return Token{AND, 1};
        case '|':
            return Token{OR, 1};
        case '+':
            return Token{PLUS, 1};
        case '*':
            return Token{STAR, 1};
        case '^':
            return Token{CARET, 1};
        case '%':
            return Token{PERCENT, 1};
        default:
            return Token{UNKNOWN, 0};
    }
}

std::vector<Token> tokenizer::lex_input() {
    std::optional<Token> curr_tkn;
    // This uses an overload `explicit operator bool()` for std::optional
    while ((curr_tkn = this->next_tkn())) {
        this->m_tkns.push_back(curr_tkn.value());
    }

    // TODO: c++ questions...
    // Is this moved out (move semantics)? does it get R[eturn]V[alue]O[ptimized]?
    return this->m_tkns;
}

}  // namespace zade
