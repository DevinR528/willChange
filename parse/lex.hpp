
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include "../result.hpp"

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
#ifdef DEBUGGING
const static char* COMMENT_INFO_NAMES[] = {
	"MULTI_LINE",
	"SINGLE_LINE",
};
#endif

enum numeric_base_kind {
	// 0b
	BIN,
	// 0o
	OCTAL,
	// 0x
	HEX,
	// No prefix
	DEC
};
#ifdef DEBUGGING
const static char* BASE_INFO_NAMES[] = {
	"BIN",
	"OCTAL",
	"HEX",
	"DEC",
};
#endif

union token_info {
  private:
	comment_kind m_comments;
	numeric_base_kind m_base;

  public:
	token_info() = default;
	token_info(comment_kind comment) : m_comments(comment) {}
	token_info(numeric_base_kind num_base) : m_base(num_base) {}

	auto comment() const& -> comment_kind { return m_comments; }
	auto base() const& -> numeric_base_kind { return m_base; }

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
	token(token_kind k, size_t len, token_info info) : kind{k}, len{len}, info{info} {}
	~token() {}

	// Get
	token_kind tkn_kind() const& { return this->kind; }
	size_t tkn_len() const& { return this->len; }
	// Set
	void add_len(size_t add) { this->len += add; }

	std::optional<comment_kind> comment_info() const& { return this->info.comment(); }
	std::optional<numeric_base_kind> num_base_info() const& { return this->info.base(); }


	friend bool operator==(const token& lhs, const token& rhs) { return lhs.len == rhs.len && lhs.kind == rhs.kind; }
	friend bool operator!=(const token& lhs, const token& rhs) { return !(lhs == rhs); }

#ifdef DEBUGGING
	std::string str() const& {
		std::string info;
		switch (tkn_kind()) {
			case INT_LIT:
			case FLOAT_LIT:
				// Safe to "unwrap" value of std::optional since we know all int/floats have
				// `numeric_base_kind` for the `token_info` union
				info = std::string(", ") + BASE_INFO_NAMES[num_base_info().value()];
				break;
			case COMMENT:
				// Safety: same as above but for comments
				info = std::string(", ") + COMMENT_INFO_NAMES[comment_info().value()];
				break;
			default:
				info = "";
				break;
		}
		return std::string("token(") + TOKEN_NAMES[tkn_kind()] + ", " + std::to_string(tkn_len())
			+ info + ")";
	}
#endif
};

// The lexer/tokenizer that turns input text into a `vector<token>`.
class tokenizer {
  private:
	std::string_view m_str_slice;
	const std::string_view m_str_slice_orig;
	std::vector<token> m_tkns{};

	std::optional<char> peek_nth(size_t) noexcept;
	std::optional<char> eat() noexcept;
	std::optional<size_t> eat_while(std::function<bool(char)>) noexcept;
	std::optional<size_t> eat_decimal_digits() noexcept;
	std::optional<size_t> eat_hex_digits() noexcept;
	std::optional<size_t> eat_float_expo() noexcept;

	result<token, std::string> comment() noexcept;
	result<token, std::string> whitespace() noexcept;
	result<token, std::string> identifier() noexcept;
	result<token, std::string> numeric_lit() noexcept;
	result<token, std::string> byte_char() noexcept;
	result<token, std::string> byte_str() noexcept;
	result<token, std::string> raw_str() noexcept;
	result<token, std::string> char_lit() noexcept;
	result<token, std::string> string_lit() noexcept;

	result<token, std::string> next_tkn() noexcept;

  public:
	tokenizer(std::string_view str) : m_str_slice{str}, m_str_slice_orig{str} {}
	~tokenizer() {}

	const std::string_view content() noexcept { return this->m_str_slice_orig; };
	result<std::vector<token>, std::string> lex_input() noexcept;

#ifdef DEBUGGING
	const std::vector<token>& tokens() const& { return this->m_tkns; }
#endif
};

}  // namespace zade
