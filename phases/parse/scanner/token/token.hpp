
// vim: set tabstop=4 smarttab

#pragma once

#include <optional>
#include <string>

#include "../token_kind.hpp"

namespace zade {

struct whitespace_info {};
struct comment_info {};

enum tag_whitespace_comment { wc_WHITESPACE, wc_COMMENT };

struct whitespace_comment {
	tag_whitespace_comment kind;
	union {
		whitespace_info whitespace;
		comment_info comment;
	};

	whitespace_comment() = delete;
	whitespace_comment(whitespace_info ws) : kind(wc_WHITESPACE), whitespace(ws) {}
	whitespace_comment(comment_info cmt) : kind(wc_COMMENT), comment(cmt) {}
	~whitespace_comment() {}
};

struct token {
	token_kind kind;
	std::optional<std::string> tkn_str;
	// token_info info;

	token() : kind(t_UNKNOWN) {}
	token(token_kind kind) : kind(kind) {}
	token(token_kind kind, std::string tkn_str) : kind(kind), tkn_str(tkn_str) {}
	~token() {}

	// friend bool operator==(const token& lhs, const token& rhs) {
	// 	return lhs.m_len == rhs.m_len && lhs.m_kind == rhs.m_kind;
	// }

	// friend bool operator!=(const token& lhs, const token& rhs) {
	// 	return !(lhs == rhs);
	// }

	#ifdef DEBUGGING
	std::string str() const&;
	#endif
};

}
