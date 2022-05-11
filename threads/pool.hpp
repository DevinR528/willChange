
// vim: set tabstop=4 smarttab

#pragma once

#include <filesystem>

#include "../cmdln/flags.hpp"
#include "../prio_queue/queue.hpp"

namespace zade {

struct tpool {
	bool add_parse_phases(std::filesystem::path& file);
	bool join();
};

bool new_thread_pool(tpool*& pool, pqueue<int>*& queue, size_t jobs, options& opts);


}  // namespace zade
