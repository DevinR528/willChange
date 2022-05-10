#pragma once

#include <optional>

#include "../token_kind.hpp"
#include "../../token_kind.hpp"

namespace zade {

enum whitespace_comment { wc_WHITESPACE, };

struct whitespace_comment {
	space_comment kind;
	union {
		whitespace_info whitespace;
		comment_info comment;
	};

	whitespace_comment() = delete;
	whitespace_comment(comment_kind comment) : m_comments(comment) {}
	whitespace_comment(numeric_base_kind num_base) : m_base(num_base) {}
	~token_info() {}
};

struct token {
	token_kind m_kind;
	std::optional<std::string> tkn_str;
	token_info info;

	token() : m_kind(UNKNOWN), m_len(0) {}
	token(token_kind k, size_t len) : m_kind{k}, m_len{len} {}
	token(token_kind k, size_t len, token_info info) : m_kind{k}, m_len{len}, info{info} {}
	~token() {}

	std::optional<comment_kind> comment_info() const& { return this->info.comment(); }
	std::optional<numeric_base_kind> num_base_info() const& { return this->info.base(); }

	friend bool operator==(const token& lhs, const token& rhs) {
		return lhs.m_len == rhs.m_len && lhs.m_kind == rhs.m_kind;
	}
	
	friend bool operator!=(const token& lhs, const token& rhs) {
		return !(lhs == rhs);
	}

	#ifdef DEBUGGING
	std::string str() const&;
	#endif
};

}