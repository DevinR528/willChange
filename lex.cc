
// vim: set tabstop=4 expandtab smarttab

#include "lex.h"
#include "result.h"
#include "macros.h"

#include <cctype>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace zade {

// The tokenizer
std::string_view Tokenizer::content() noexcept { return this->m_str_slice; }

// Comments go here...
Result<uint32_t, std::string> Tokenizer::test_res() {
	void* ptr = std::malloc(8);
	intptr_t num = reinterpret_cast<intptr_t>(ptr);
	if (num % 2 == 0) {
		return Result{3};
	} else {
		return Result{std::string("failure...")};
	}
}


std::optional<Token> Tokenizer::comment() {
	// eat first '/'
	this->m_str_slice = this->m_str_slice.substr(1);
	if (this->m_str_slice.empty()) return none{};

	if (*this->m_str_slice.begin() == '/') {
		auto token = Token{COMMENT, 2, CommentKind::SINGLE_LINE};

		fmt::print("{} {}", token.tkn_kind(), token.tkn_len());

		TODO("still working on this");
	} else if (*this->m_str_slice.begin() == '*') {
		auto token = Token{COMMENT, 2, CommentKind::MULTI_LINE};

		TODO("Multi line comments");
	} else {
		return none{};
	}
}

std::optional<Token> Tokenizer::next_tkn() noexcept {
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

std::vector<Token> Tokenizer::lex_input() {
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
