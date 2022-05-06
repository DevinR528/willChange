
// vim: set tabstop=4 smarttab

//#include <filesystem>
#include <fstream>
//#include <string>
//#include <string_view>
#include <string.h>

#include <parse/lex.hpp>

#include "print.hpp"

#define strequals(a, b) (!(strcmp(a, b)))

int main(int argc, char const* argv[])
{
	if (argc <= 1)
	{
		print("usage: ", argv[0], "-i path/to/file.zd [-o out/file]");
		
		return 0;
	}
	
	std::string input;
	if (strequals(argv[1], "-i"))
	{
		std::ifstream file(argv[2]);
		
		if (file.is_open())
		{
			while (file.peek() != EOF)
				input.push_back(file.get());
			file.close();
		}
	}
	else
	{
		print(
			"expected input file\n"
			"usage: zadec -i path/to/file.zd [-o out/file]\n"
			"found: ", argv[1]);
		
		return 0;
	}
	
	auto tok = zade::Tokenizer(input);
	
	print(tok.content());
	
	#ifdef DEBUGGING
	auto res = tok.lex_input();
	if (res) {
		for (auto&& tkn : res.value()) {
//			assert(!"TODO");
			print(tkn);
			// fmt::print("{}\n", tkn);
		}
	} else {
		assert(!"TODO");
		// fmt::print("error: {}\n", res.error());
	}
	#endif
	
//	// fmt::print("bout to parse this '{}'\n", std::filesystem::current_path().string());

	return 0;
}









