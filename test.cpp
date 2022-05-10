#include <optional>
#include <fstream>

#include "print.hpp"
#include "macros.hpp"
#include "parse/tokenizer/tokenizer.hpp"


auto main(int argc, char const* argv[]) -> int {
	
	int code = 0;

	std::optional<std::string> flags;

	if (!(res = cmdln_process(&flags, argc, argv)))
	{
		
	}
}