
// vim: set tabstop=4 expandtab smarttab

#include "lex.hpp"

#include "../macros.hpp"
#include "../result.hpp"

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

// As long as this is only called after the switch `tokenizer::next_tkn`
// we are guaranteed that the first char is a non numeric, it is legal for
// the following char's until whitespace to be any alpha-numeric char.
bool is_identifier(char ch) noexcept {
	switch (ch) {
		case 'a' ... 'z':
		case 'A' ... 'Z':
		case '0' ... '9':
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

std::optional<char> tokenizer::peek_nth(size_t idx = 0) noexcept {
	if (this->m_str_slice.size() <= idx) return std::optional<size_t>{};
	return this->m_str_slice[idx];
}

std::optional<size_t> tokenizer::eat_while(std::function<bool(char)> func) noexcept {
	size_t count = 0;
	while (func(*this->m_str_slice.begin())) {
		count += 1;
		auto eaten = this->eat();
		if (!eaten.has_value()) break;
	}
	return count == 0 ? std::optional<size_t>{} : count;
}

std::optional<size_t> tokenizer::eat_decimal_digits() noexcept {
	auto curr = this->peek_nth(0);
	size_t count = 0;
	while (curr.has_value()) {
		if (!(('0' <= curr && curr <= '9') || curr == '_')) { break; }
		this->eat();
		count += 1;
		curr = this->peek_nth(0);
	}
	return count == 0 ? std::optional<size_t>{} : count;
}

std::optional<size_t> tokenizer::eat_hex_digits() noexcept {
	auto curr = this->peek_nth(0);
	auto count = 0;
	while (curr.has_value()) {
		switch (curr.value()) {
			case '0' ... '9':
			case 'a' ... 'f':
			case 'A' ... 'F':
			case '_': {
				this->eat();
				count += 1;
				curr = this->peek_nth(0);
				continue;
			}
			default:
				goto break_loop;
		}
	}
break_loop:;

	// TODO: confirm
	// we want to return non here since an empty number isn't really a number
	return (count > 0) ? count : std::optional<size_t>{};
}

std::optional<size_t> tokenizer::eat_float_expo() noexcept {
	auto curr = this->peek_nth(0);
	size_t count = 0;
	if (curr.has_value()) {
		this->eat();
		count += 1;
	}
	return (OPT_TRY(this->eat_decimal_digits()) + count);
}

result<token, std::string> tokenizer::comment() noexcept {
	// eat first '/', and eat then keep the 2nd, this is why all `token`s have 2 len already
	OPT_TRY_ERR(this->eat(), std::string("malformed comment"));
	auto next = OPT_TRY_ERR(this->eat(), std::string("invalid comment"));

	if (next == '/') {
		auto tkn = token{COMMENT, 2, comment_kind::SINGLE_LINE};

		auto line = OPT_TRY_ERR(
			this->eat_while([](char ch) -> bool { return ch != '\n'; }),
			std::string("malformed comment"));

		tkn.add_len(line);
		return tkn;
	} else if (next == '*') {
		auto tkn = token{COMMENT, 2, comment_kind::MULTI_LINE};

		TODO("Multi line comments\n");
	} else {
		return std::string("invalid comment");
	}
}

result<token, std::string> tokenizer::whitespace() noexcept {
	auto tkn = token{WHITESPACE, 0};
	// We know we peeked at least one whitespace char so this will return at least 1
	auto ws =
		OPT_TRY_ERR(this->eat_while(is_whitespace), std::string("found EOF in whitespace..."));
	tkn.add_len(ws);
	return tkn;
}

result<token, std::string> tokenizer::identifier() noexcept {
	auto tkn = token{IDENT, 0};
	// We know we peeked at least one ident char so this will return at least 1
	auto ws = OPT_TRY_ERR(this->eat_while(is_identifier), std::string("found EOF in ident..."));

	tkn.add_len(ws);
	return tkn;
}

// A numeric literal, one of `1`, `1.1`, `0xff`, `1e10` and a few others maybe.
result<token, std::string> tokenizer::numeric_lit() noexcept {
	// Eat the token we just peeked we know it was a 0...9
	auto curr = this->eat();

	auto base = DEC;
	size_t eaten = 1;

	if (curr == '0') {
		auto next = OPT_TRY_ERR(this->peek_nth(0), std::string("just a zero and EOF??"));
		switch (next) {
			// Bin
			case 'b': {
				base = BIN;
				this->eat();
				eaten += 1;
				auto count = OPT_TRY_ERR(
					this->eat_decimal_digits(),
					// This is most likely an error, it would be
					// `0 b EOF` so not sure why that would ever be legit
					(token{INT_LIT, eaten, base}));
				eaten += count;
				break;
			}
			// Octal
			case 'o': {
				base = OCTAL;
				this->eat();
				eaten += 1;
				auto count = OPT_TRY_ERR(
					this->eat_decimal_digits(),
					// This is most likely an error, it would be
					// `0 o EOF` so not sure why that would ever be legit
					(token{INT_LIT, eaten, base}));
				eaten += count;
				break;
			}
			// Hex
			case 'x': {
				base = HEX;
				this->eat();
				eaten += 1;
				auto count = OPT_TRY_ERR(
					this->eat_hex_digits(),
					// This is most likely an error, it would be
					// `0 x EOF` so not sure why that would ever be legit
					(token{INT_LIT, eaten, base}));
				eaten += count;
				break;
			}
			// Leading 0
			case '0' ... '9':
			case '_':
			case '.':
			case 'e':
			case 'E': {
				this->eat();
				eaten += 1;
				auto count = OPT_TRY_ERR(
					this->eat_decimal_digits(),
					// This is most likely an error, it would be
					// `0 x EOF` so not sure why that would ever be legit
					(token{INT_LIT, eaten, base}));
				eaten += count;
				break;
			}
			// Just a 0
			default:
				return token{INT_LIT, eaten, base};
		}
	} else {
		auto count = OPT_TRY_ERR(this->eat_decimal_digits(), std::string("empty numeric lit"));
		eaten += count;
	}

	auto peek = OPT_TRY_ERR(this->peek_nth(0), (token{INT_LIT, eaten, base}));
	switch (peek) {
		case '.': {
			auto peek2 = OPT_TRY_ERR(this->peek_nth(1), std::string("decimal then EOF"));
			// TODO: do we want `12.method()` or ranges `0..10`?
			if ('0' <= peek2 || peek2 <= '9' || peek2 == '_') {
				// eat decimal point
				this->eat();
				eaten += 1;
				auto count = OPT_TRY_ERR(
					this->eat_decimal_digits(),
					std::string("empty portion after decimal point"));
				eaten += count;
				auto after_decimal = this->peek_nth(0);
				if ((after_decimal.has_value() && after_decimal.value() == 'e')
					|| after_decimal.value() == 'E') {
					this->eat();
					eaten += 1;
					auto expo_count = OPT_TRY_ERR(
						this->eat_float_expo(),
						std::string("no exponent after `e` or `E`"));
					eaten += expo_count;
				}
			}
			return token{FLOAT_LIT, eaten, base};
		}
		case 'e':
		case 'E': {
			this->eat();
			eaten += 1;
			auto count = OPT_TRY_ERR(this->eat_decimal_digits(), std::string("empty exponent lit"));
			eaten += count;
			return token{FLOAT_LIT, eaten, base};
		}
		default:
			return token{INT_LIT, eaten, base};
	}
}

// A character literal, one of 'a' or '\n'
result<token, std::string> tokenizer::char_lit() noexcept { TODO("still trucking... char_lit\n"); }

// A byte literal `b'x';`
result<token, std::string> tokenizer::byte_char() noexcept {
	TODO("still trucking... byte_char\n");
}
// A byte string literal `b"bits";` == and array of bytes so `[]uint8`
result<token, std::string> tokenizer::byte_str() noexcept { TODO("still trucking... byte_str\n"); }
// A raw string, this continues until the correct sequence is repeated.
// TODO: we could do something like `r#"this is "STILL" this string"#;`
result<token, std::string> tokenizer::raw_str() noexcept { TODO("still trucking... raw_str\n"); }

result<token, std::string> tokenizer::string_lit() noexcept {
	TODO("still trucking... string_lit\n");
}

result<token, std::string> tokenizer::next_tkn() noexcept {
	// Next or return End-Of-File
	auto peeked = OPT_TRY_ERR(this->peek_nth(0), (token{_EOF, 0}));

	switch (peeked) {
		case '/':
			return this->comment();
		case ' ':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case '\v':
			return this->whitespace();
		// Add emoji's and stuff?
		case 'a' ... 'z':
		case 'A' ... 'Z':
		case '_': {
			auto peek = this->peek_nth(0);
			auto peek2 = this->peek_nth(1);

			if (peek == 'b' && peek2 == '\'')
				return this->byte_char();
			else if (peek == 'b' && peek2 == '"')
				return this->byte_str();
			else if (peek == 'r' && peek2 == '"')
				return this->raw_str();
			else
				return this->identifier();
		}
		case '0' ... '9':
			return this->numeric_lit();
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
