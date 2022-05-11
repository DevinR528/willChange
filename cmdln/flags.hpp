
// vim: set tabstop=4 smarttab

#pragma once

#include <filesystem>

namespace zade {
enum run_kind { INTERPRET, COMPILE };

struct optimization_level {
	bool ol_constant_folding;
	bool ol_float_constant_folding;
	bool ol_cross_function_constant_folding;
	bool ol_cross_function_loop_depth;
};

struct options {
	optimization_level opt_level;
	run_kind action;
};

struct cmdln_flags {
	std::filesystem::path source_file;
	size_t num_jobs;
	options opts;
};

}  // namespace zade
