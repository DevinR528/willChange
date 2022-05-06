
// vim: set tabstop=4 smarttab

#include <parse/lex.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

enum test {
    test_a,
    test_b,
    test_c,
};

std::string lookup[] = {
    [test_a] = "a",
    [test_b] = "b",
    [test_c] = "c",
};

template<> struct fmt::formatter<enum test> {
	constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
	    return ctx.begin();
	}
	
	template<typename FormatContext>
	auto format(const enum test& x, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), "test::{}", lookup[x]);
	}
};

int main(int argc, char const* argv[])
{
	if (argc <= 1) {
		fmt::print(
			"usage: zadec -i path/to/file.zd [-o out/file]\n"
			"file: {}\n",
			argv[0]);
		return 0;
	}
    
//    struct enum_value x = enum_value("three", 3);
    
    fmt::print("x: {}\n", test_b);
    
//	std::string input;
//	if (strcmp(argv[1], "-i") == 0) {
//		std::ifstream file(argv[2]);
//		if (file.is_open()) {
//			while (file.peek() != EOF)
//				input.push_back(file.get());
//			file.close();
//		}
//	} else {
//		fmt::print(
//			"expected input file\n"
//			"usage: zadec -i path/to/file.zd [-o out/file]\n"
//			"found: {}\n",
//			argv[1]);
//		return 0;
//	}

//	auto tok = zade::Tokenizer(input);

//	fmt::print("`{}`", tok.content());

//	auto res = tok.lex_input();
//	if (res) {
//		for (auto&& tkn : res.value()) {
//			fmt::print("{}\n", tkn);
//		}
//	} else {
//		fmt::print("error: {}\n", res.error());
//	}

	// fmt::print("bout to parse this '{}'\n", std::filesystem::current_path().string());

	return 0;
}
