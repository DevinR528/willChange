
// vim: set tabstop=4 smarttab

#include <cstring>
#include <fstream>
#include <filesystem>

// #include "print.hpp"
// #include "macros.hpp"
#include "result.hpp"
// #include "parse/tokenizer/tokenizer.hpp"

auto main(int argc, char const* argv[]) -> int {
	
	int code = 0;
	
	std::string error_msg;

    cmdln_flags* flags = NULL;
	pqueue* pqueue = NULL;
	tpool* tpool = NULL;
	
	bool error = false
		?: cmdln_process(flags, argc, argv)
		?: new_pqueue(pqueue)
		?: new_thread_pool(tpool, pqueue, flags->num_jobs, &flags->options)
		?: tpool->add_parse_phases(flags->source_file)
			// parse phase:
				// reading tokens, stream into AST
				// invoke type checker phase
				// add_type_check_task()
			
			// type checker phase:
				// ...
				// invoke reference resolution
				// add_reference resolution_task()
		
			// reference resolution
				// ...
			
			// ...

			// link phase:
				// connects references assembly labels
				// what should I invoke now?
					// maybe:
					// add_interpret_task()
					// add_print_asm_task()

			// interpret phase:

			// print asm phase:
			
		?: tpool->join()
		 ;

	tpool->~tpool();
	pqueue->~pqueue();
	flags->~cmdln_flags();

	return error ? 1 : 0;
}













	#if 0
	if (!(result<cmdln_flags*,std::string> res = cmdln_process(&flags, argc, argv)))
	{
		
	}
	else
	{

	}
	
	if (cond) {

	} else while () {}
	
	if (!res.has_value()) {
		fprintf(stderr, "!!!");
		code = 1;
	} else {
		
		result<pqueue*,std::string> res = new_pqueue();

		TODO; // setup prioity queue
	}
	
	if (res)
	{
		TODO; // setup thread pool
	}

	if (res)
	{
		TODO; // add first given file
	}

	if (res)
	{
		TODO; // join all threads
	}

	free_flags(flags);

	return code;


	if (argc <= 1) {
		zade::print("usage: ", argv[0], "-i path/to/file.zd [-o out/file]");

		return 0;
	}

	std::string filename = "";
	if (strequals(argv[1], "-i")) {
		filename = argv[2];
	} else {
	
		zade::print(
			"expected input file\n"
			"usage: zadec -i path/to/file.zd [-o out/file]\n"
			"found: ",
			argv[1]);
		
		return 0;
	}
	
	auto tok = zade::tokenizer(filename);
	// zade::print(tok.content());

#ifdef DEBUGGING
	auto res = tok.lex_input();
	if (res) {
		for (auto&& tkn : res.value()) {
			zade::print(tkn);
		}
	} else {
	TODO("rewrite");
//		size_t cnt = 0;
//		for (auto&& tkn : tok.tokens()) {
//			cnt += tkn.len();
//			zade::print(tkn);
//		}
//		zade::print("\nerror: ", res.error());
//		zade::print("at: ", tok.content().substr(cnt - 5, 20));
	}
#endif
#endif
