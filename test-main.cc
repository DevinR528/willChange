
// vim: set tabstop=4 smarttab

#include "macros.hpp"
#include "phases/parse/scanner/token/token.hpp"
#include "phases/parse/scanner/token_kind.hpp"
#include "result.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

typedef ptrdiff_t Size;
template<typename Type, Size n> Size count_of(Type (&)[n]) { return n; }

zade::result<std::vector<zade::token>, std::string> tokenizer(std::string in) {
	TODO("make zade::tokenizer");
}

bool lex_in(std::string input, std::vector<zade::token> expected) {
	auto res = tokenizer(input);
	if (res.has_value() && std::equal(expected.begin(), expected.end(), res.value().begin())) {
		return 0;
	} else {
#ifdef DEBUGGING
		for (auto&& tkn : tok.tokens()) {
			zade::print(tkn);
		}
		zade::print("error: ", res.error());
#endif
		return 1;
	}
}

struct test {
	std::string input;
	bool should_fail = false;
	std::vector<zade::token> expected;

	test(std::string input, std::vector<zade::token> expected) : input(input), expected(expected) {}
	test(std::string input, std::vector<zade::token> expected, bool should_fail) :
		input(input),
		expected(expected),
		should_fail(should_fail) {}
};

const test tests[] = {
	{"extern", std::vector{zade::token{zade::t_IDENT}}},
	{"#include", std::vector{zade::token{zade::t_POUND}, zade::token{zade::t_IDENT}}},
};

int main() {
	int correct = 0;
	int n = 1;
	for (int i = 0, n = count_of(tests); i < n; i++) {
		int actual = lex_in(tests[i].input, tests[i].expected);

		if (actual == 0) {
			correct++;
		} else {
			fprintf(stderr, "I'm mad!\n");
		}
	}

	printf("%i out of %i (%g%%)\n", correct, n, (float)correct / n * 100);

	return 0;
}
