
// vim: set tabstop=4 smarttab

#include "parse/lex.hpp"
#include "print.hpp"

#include <cstring>
#include <fstream>

#define strequals(a, b) (!(strcmp(a, b)))

auto main(int argc, char const* argv[]) -> int {
	if (argc <= 1) {
		zade::print("usage: ", argv[0], "-i path/to/file.zd [-o out/file]");

		return 0;
	}

	std::string input;
	if (strequals(argv[1], "-i")) {
		std::ifstream file(argv[2]);

		if (file.is_open()) {
			while (file.peek() != EOF)
				input.push_back(file.get());
			file.close();
		}
	} else {
		zade::print(
			"expected input file\n"
			"usage: zadec -i path/to/file.zd [-o out/file]\n"
			"found: ",
			argv[1]);

		return 0;
	}

	auto tok = zade::tokenizer(input);
	zade::print(tok.content());

#ifdef DEBUGGING
	auto res = tok.lex_input();
	if (res) {
		for (auto&& tkn : res.value()) {
			zade::print(tkn);
		}
	} else {
		for (auto&& tkn : tok.tokens()) {
			zade::print(tkn);
		}
		zade::print("error: ", res.error());
	}
#endif

	return 0;
}
