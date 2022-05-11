
// vim: set tabstop=4 smarttab

#include <filesystem>

#include "pool.hpp"

#include "../cmdln/flags.hpp"
#include "../prio_queue/queue.hpp"
#include "../macros.hpp"

namespace zade {

bool tpool::add_parse_phases(std::filesystem::path& file) { TODO("tpool::add_parse_phase"); }
bool tpool::join() { TODO("tpool::join"); }

bool new_thread_pool(tpool*& pool, pqueue<int>*& queue, size_t jobs, options& opts) {
	TODO("new_thread_pool");
}

}  // namespace zade
