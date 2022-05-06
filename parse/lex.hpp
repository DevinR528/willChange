
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include "result.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace zade {

enum token_kind {
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
	PIPE,
	// +
	PLUS,
	// *
	STAR,
	// ^
	CARET,
	// %
	PERCENT,
	// Any whitespace
	WHITESPACE,
	// Any identifier, this includes keywords
	IDENT,
	// End of file
	_EOF,
	// This is incase we want error tolerance
	UNKNOWN,
};

#ifdef DEBUGGING
const static char* TOKEN_NAMES[] = {
	"COMMENT",		"INT_LIT",		 "FLOAT_LIT",  "CHAR_LIT",	  "STR_LIT",	"SEMI",
	"COMMA",		"DOT",			 "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACE", "CLOSE_BRACE",
	"OPEN_BRACKET", "CLOSE_BRACKET", "AT",		   "POUND",		  "TILDE",		"QUESTION",
	"COLON",		"DOLLAR",		 "EQ",		   "BANG",		  "LESS",		"GREAT",
	"MINUS",		"AND",			 "PIPE",	   "PLUS",		  "STAR",		"CARET",
	"PERCENT",		"WHITESPACE",	 "IDENT",	   "_EOF",		  "UNKNOWN",
};
#endif

enum comment_kind { MULTI_LINE, SINGLE_LINE };
enum base {
	// 0b
	BIN,
	// 0o
	OCTAL,
	// 0x
	HEX,
	// No prefix
	DEC
};

union token_info {
  private:
	comment_kind comments;
	base int_base;

  public:
	token_info() = default;
	token_info(comment_kind comment) : comments(comment) {}
	token_info(base num_base) : int_base(num_base) {}
	~token_info() {}
};
struct token {
  private:
	token_kind kind;
	size_t len;
	token_info info;

  public:
	token() : kind(UNKNOWN), len(0) {}
	token(token_kind k, size_t len) : kind{k}, len{len} {}
	token(token_kind k, size_t len, comment_kind comment) : kind{k}, len{len}, info{comment} {}

	token_kind tkn_kind() const& { return this->kind; }

	size_t tkn_len() const& { return this->len; }

	void add_len(size_t add) { this->len += add; }

#ifdef DEBUGGING
	std::string str() {
		return std::string("token(") + zade::TOKEN_NAMES[tkn_kind()] + ", "
			+ std::to_string(tkn_len()) + ")";
	}
#endif

	~token() {}
};

// The lexer/tokenizer that turns input text into a `vector<token>`.
class tokenizer {
	using none = std::optional<token>;

  private:
	std::string_view m_str_slice;
	std::string_view m_str_slice_orig;
	std::vector<token> m_tkns{};

	std::optional<char> eat() noexcept;
	std::optional<size_t> eat_while(std::function<bool(char)> func) noexcept;

	result<token, std::string> comment() noexcept;
	result<token, std::string> whitespace() noexcept;
	result<token, std::string> identifier() noexcept;
	result<token, std::string> numeric_lit() noexcept;
	result<token, std::string> char_lit() noexcept;
	result<token, std::string> string_lit() noexcept;

	result<token, std::string> next_tkn() noexcept;

  public:
	tokenizer(std::string_view str) : m_str_slice{str}, m_str_slice_orig{str} {}
	std::string_view content() noexcept { return this->m_str_slice_orig; };
	result<std::vector<token>, std::string> lex_input() noexcept;

	~tokenizer() {}
};
}  // namespace zade
