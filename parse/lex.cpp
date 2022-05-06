
// vim: set tabstop=4 expandtab smarttab

#include "lex.hpp"

#include "macros.hpp"
#include "result.hpp"

#include <cctype>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace zade {

bool is_whitespace(char ch) noexcept {
	switch (ch) {
		case ' ':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case '\v':
			return true;
		default:
			return false;
	}
}

bool is_identifier(char ch) noexcept {
	switch (ch) {
		case 'a' ... 'z':
		case 'A' ... 'Z':
		case '_':  // Add emoji's and stuff?
			return true;
		default:
			return false;
	}
}

// The tokenizer
std::optional<char> tokenizer::eat() noexcept {
	if (this->m_str_slice.empty()) return std::optional<size_t>{};

	auto ch = *this->m_str_slice.begin();
	this->m_str_slice = this->m_str_slice.substr(1);
	return ch;
}

std::optional<size_t> tokenizer::eat_while(std::function<bool(char)> func) noexcept {
	size_t res = 0;
	while (func(*this->m_str_slice.begin())) {
		res += 1;
		auto eaten = this->eat();
		if (!eaten.has_value()) break;
	}
	return res == 0 ? std::optional<size_t>{} : res;
}

result<token, std::string> tokenizer::comment() noexcept {
	// eat first '/', this is why all `token`s have 1 len already
	auto is_ok = this->eat();
	if (!is_ok) return std::string("malformed comment");

	if (*this->m_str_slice.begin() == '/') {
		auto tkn = token{COMMENT, 1, comment_kind::SINGLE_LINE};

		auto line = this->eat_while([](char ch) -> bool { return ch != '\n'; });
		if (!line.has_value()) return std::string("malformed comment");

		tkn.add_len(line.value());
		return tkn;
	} else if (*this->m_str_slice.begin() == '*') {
		auto tkn = token{COMMENT, 1, comment_kind::MULTI_LINE};

		TODO("Multi line comments\n");
	} else {
		return std::string("invalid comment");
	}
}

result<token, std::string> tokenizer::whitespace() noexcept {
	auto tkn = token{WHITESPACE, 0};

	auto ws = this->eat_while(is_whitespace);
	if (!ws.has_value()) return std::string("found EOF in whitespace...");

	tkn.add_len(ws.value());
	return tkn;
}

result<token, std::string> tokenizer::identifier() noexcept {
	auto tkn = token{IDENT, 0};

	auto ws = this->eat_while(is_identifier);
	if (!ws) return std::string("found EOF in ident...");

	tkn.add_len(ws.value());
	return tkn;
}

result<token, std::string> tokenizer::numeric_lit() noexcept {
	TODO("still trucking... numeric_lit\n");
}

result<token, std::string> tokenizer::char_lit() noexcept { TODO("still trucking... char_lit\n"); }

result<token, std::string> tokenizer::string_lit() noexcept {
	TODO("still trucking... string_lit\n");
}

result<token, std::string> tokenizer::next_tkn() noexcept {
	if (this->m_str_slice.empty()) return token{_EOF, 0};

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
		case ';': {
			if (!this->eat().has_value()) return std::string("expected semicolon");
			return token{SEMI, 1};
		}
		case ',': {
			if (!this->eat().has_value()) return std::string("expected comma");
			return token{COMMA, 1};
		}
		case '.': {
			if (!this->eat().has_value()) return std::string("expected dot");
			return token{DOT, 1};
		}
		case '(': {
			if (!this->eat().has_value()) return std::string("expected open parenthesis");
			return token{OPEN_PAREN, 1};
		}
		case ')': {
			if (!this->eat().has_value()) return std::string("expected close parenthesis");
			return token{CLOSE_PAREN, 1};
		}
		case '{': {
			if (!this->eat().has_value()) return std::string("expected open brace");
			return token{OPEN_BRACE, 1};
		}
		case '}': {
			if (!this->eat().has_value()) return std::string("expected close brace");
			return token{CLOSE_BRACE, 1};
		}
		case '[': {
			if (!this->eat().has_value()) return std::string("expected open bracket");
			return token{OPEN_BRACKET, 1};
		}
		case ']': {
			if (!this->eat().has_value()) return std::string("expected close bracket");
			return token{CLOSE_BRACKET, 1};
		}
		case '@': {
			if (!this->eat().has_value()) return std::string("expected at");
			return token{AT, 1};
		}
		case '#': {
			if (!this->eat().has_value()) return std::string("expected hash");
			return token{POUND, 1};
		}
		case '~': {
			if (!this->eat().has_value()) return std::string("expected tilde");
			return token{TILDE, 1};
		}
		case '?': {
			if (!this->eat().has_value()) return std::string("expected question mark");
			return token{QUESTION, 1};
		}
		case ':': {
			if (!this->eat().has_value()) return std::string("expected colon");
			return token{COLON, 1};
		}
		case '$': {
			if (!this->eat().has_value()) return std::string("expected dollar");
			return token{DOLLAR, 1};
		}
		case '=': {
			if (!this->eat().has_value()) return std::string("expected equal");
			return token{EQ, 1};
		}
		case '!': {
			if (!this->eat().has_value()) return std::string("expected exclamation point");
			return token{BANG, 1};
		}
		case '<': {
			if (!this->eat().has_value()) return std::string("expected open angle bracket");
			return token{LESS, 1};
		}
		case '>': {
			if (!this->eat().has_value()) return std::string("expected close angle bracket");
			return token{GREAT, 1};
		}
		case '-': {
			if (!this->eat().has_value()) return std::string("expected minus");
			return token{MINUS, 1};
		}
		case '&': {
			if (!this->eat().has_value()) return std::string("expected ampresand");
			return token{AND, 1};
		}
		case '|': {
			if (!this->eat().has_value()) return std::string("expected pipe");
			return token{PIPE, 1};
		}
		case '+': {
			if (!this->eat().has_value()) return std::string("expected plus");
			return token{PLUS, 1};
		}
		case '*': {
			if (!this->eat().has_value()) return std::string("expected star");
			return token{STAR, 1};
		}
		case '^': {
			if (!this->eat().has_value()) return std::string("expected caret");
			return token{CARET, 1};
		}
		case '%': {
			if (!this->eat().has_value()) return std::string("expected percent");
			return token{PERCENT, 1};
		}
		default:
			return token{UNKNOWN, 0};
	}
}

result<std::vector<token>, std::string> tokenizer::lex_input() noexcept {
	// This uses an overload `explicit operator bool()` for std::optional
	while (true) {
		auto curr_tkn = this->next_tkn();
		if (curr_tkn.has_value()) {
			if (curr_tkn.value().tkn_kind() == UNKNOWN) return std::string("found unkown token");

			if (curr_tkn.value().tkn_kind() == _EOF) break;

			this->m_tkns.push_back(curr_tkn.value());
		} else
			return curr_tkn.error();
	}

	// TODO: c++ questions...
	// Is this moved out (move semantics)? does it get R[eturn]V[alue]O[ptimized]?
	return this->m_tkns;
}

}  // namespace zade
