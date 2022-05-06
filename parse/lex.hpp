
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <result.hpp>

#include <fmt/format.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace zade {

// NOTE: if you add a variant to this you need to add it to debug.hpp also
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
	"COMMENT",	  "INT_LIT",	 "FLOAT_LIT",	 "CHAR_LIT",	  "STR_LIT",
	"SEMI",		  "COMMA",		 "DOT",			 "OPEN_PAREN",	  "CLOSE_PAREN",
	"OPEN_BRACE", "CLOSE_BRACE", "OPEN_BRACKET", "CLOSE_BRACKET", "AT",
	"POUND",	  "TILDE",		 "QUESTION",	 "COLON",		  "DOLLAR",
	"EQ",		  "BANG",		 "LESS",		 "GREAT",		  "MINUS",
	"AND",		  "PIPE",		 "PLUS",		 "STAR",		  "CARET",
	"PERCENT",	  "WHITESPACE",	 "IDENT",		 "_EOF",		  "UNKNOWN",
};
#endif

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
	~TokenInfo() {}
};
struct Token {
  private:
	TokenKind kind;
	size_t len;
	TokenInfo info;

  public:
	Token() : kind(UNKNOWN), len(0) {}
	Token(TokenKind k, size_t len) : kind{k}, len{len} {}
	Token(TokenKind k, size_t len, CommentKind comment) :
		kind{k},
		len{len},
		info{comment} {}
	TokenKind tkn_kind() const& { return this->kind; }
	size_t tkn_len() const& { return this->len; }
	void add_len(size_t add) { this->len += add; }
	~Token() {}
};

// The lexer/tokenizer that turns input text into a `vector<Token>`.
class Tokenizer {
	using none = std::optional<Token>;

  private:
	std::string_view m_str_slice;
	std::string_view m_str_slice_orig;
	std::vector<Token> m_tkns{};

	Result<Token, std::string> next_tkn() noexcept;

	std::optional<char> eat() noexcept;
	std::optional<size_t> eat_while(std::function<bool(char)> func) noexcept;

	Result<Token, std::string> comment() noexcept;
	Result<Token, std::string> whitespace() noexcept;
	Result<Token, std::string> identifier() noexcept;
	Result<Token, std::string> numeric_lit() noexcept;
	Result<Token, std::string> char_lit() noexcept;
	Result<Token, std::string> string_lit() noexcept;

  public:
	Tokenizer(std::string_view str) : m_str_slice{str}, m_str_slice_orig{str} {}
	std::string_view content() noexcept { return this->m_str_slice_orig; };
	Result<std::vector<Token>, std::string> lex_input() noexcept;

	~Tokenizer() {}
};
}  // namespace zade

template<>
struct fmt::formatter<zade::Token> {
	// Presentation format: 'f' - fixed, 'e' - exponential.
	// char presentation = 'f';
	constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(); // , end = ctx.end();
		return it;
	}

	template<typename FormatContext>
	auto format(const zade::Token& tkn, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(
			ctx.out(),
			"Token({}, {})",
			zade::TOKEN_NAMES[tkn.tkn_kind()],
			tkn.tkn_len());
	}
};
