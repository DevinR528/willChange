
// vim: set tabstop=4 smarttab

#include "parse/lex.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

typedef ptrdiff_t Size;
template<typename Type, Size n> Size count_of(Type (&)[n]) { return n; }

bool lex_in(std::string input, std::vector<zade::token> expected) {
	auto tok = zade::tokenizer(input);
	auto res = tok.lex_input();
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
};

const test tests[] = {
	{"extern", std::vector{zade::token{zade::IDENT, 6}}},
	{"#include", std::vector{zade::token{zade::POUND, 1}, zade::token{zade::IDENT, 7}}},
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
