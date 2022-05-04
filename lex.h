
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include "result.h"

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
  private:
	CommentKind comments;
	Base int_base;

  public:
	TokenInfo() = default;
	TokenInfo(CommentKind comment) : comments(comment) {}
	TokenInfo(Base num_base) : int_base(num_base) {}
	~TokenInfo();
};
struct Token {
  private:
	TokenKind kind;
	size_t len;
	TokenInfo info;

  public:
	Token(TokenKind k, size_t len) : kind(k), len(len) {}
	Token(TokenKind k, size_t len, CommentKind comment) : kind(k), len(len), info(comment) {}
	TokenKind tkn_kind() { return this->kind; }
	size_t tkn_len() { return this->len; }
	~Token() {}
};

// The lexer/tokenizer that turns input text into a `vector<Token>`.
class Tokenizer {
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
	Result<uint32_t, std::string> test_res();

  public:
	Tokenizer(std::string_view str)  : m_str_slice{str} {}
	std::string_view content() noexcept;
	std::vector<Token> lex_input();
	~Tokenizer() {}
};
}  // namespace zade
