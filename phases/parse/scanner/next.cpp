
// vim: set tabstop=4 smarttab

#include "next.hpp"
#include "token_kind.hpp"

namespace zade {

int next_token() {
	return STATES[0][0];
}

}  //  namespace zade
