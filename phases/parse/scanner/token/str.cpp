
#include "token.h"

#ifdef DEBUGGING
std::string str() const& {
	std::string info;
	switch (this->kind()) {
		case INT_LIT:
		case FLOAT_LIT:
			// Safe to "unwrap" value of std::optional since we know all int/floats have
			// `numeric_base_kind` for the `token_info` union
			info = std::string(", ") + BASE_INFO_NAMES[num_base_info().value()];
			break;
		
		case COMMENT:
			// Safety: same as above but for comments
			info = std::string(", ") + COMMENT_INFO_NAMES[comment_info().value()];
			break;
		
		default:
			info = "";
			break;
	}
	return std::string("token(") + TOKEN_NAMES[this->kind()] + ", "
		+ std::to_string(this->len()) + info + ")";
}
#endif
