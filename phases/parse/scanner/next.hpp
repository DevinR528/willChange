
// vim: set tabstop=4 smarttab

#pragma once

#include "token_kind.hpp"

namespace zade {

enum lookup_states {
	s_ERROR,
	s_WHITESPACE,
	s_START,
	s_READING_WHITESPACE,

	number_of_states,
};

const int ALL = 0;

const lookup_states STATES[number_of_states][127] = {
	// [s_START][' '] = s_READING_WHITESPACE,
	// [s_START]['\t'] = s_READING_WHITESPACE,
	// [s_START]['\n'] = s_READING_WHITESPACE,
	// [s_READING_WHITESPACE][ALL] = s_WHITESPACE,
	// [s_READING_WHITESPACE][' '] = s_READING_WHITESPACE,
	// [s_READING_WHITESPACE]['\t'] = s_READING_WHITESPACE,
	// [s_READING_WHITESPACE]['\n'] = s_READING_WHITESPACE,
};

int next_token();

}  //  namespace zade
