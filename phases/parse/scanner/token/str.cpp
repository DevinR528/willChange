

#include <vector>

#include "token.hpp"

namespace zade {

#ifdef DEBUGGING
std::string token::str() const& {
	return std::string("token(") + TOKEN_NAMES[this->kind]
		+ ", " + std::to_string(this->tkn_str.value_or("0").size()) + ")";
}
#endif

}  // namespace zade
