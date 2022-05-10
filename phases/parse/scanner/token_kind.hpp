
enum token_kind {
	// A comment token
	COMMENT,
	// An integer literal
	INT_LIT,
	// A float literal
	FLOAT_LIT,
	// A char literal
	CHAR_LIT,
	// A string literal
	STR_LIT,
	// ;
	SEMI,
	// ,
	COMMA,
	// .
	DOT,
	// (
	OPEN_PAREN,
	// )
	CLOSE_PAREN,
	// {
	OPEN_BRACE,
	// }
	CLOSE_BRACE,
	// [
	OPEN_BRACKET,
	// ]
	CLOSE_BRACKET,
	// @
	AT,
	// #
	POUND,
	// ~
	TILDE,
	// ?
	QUESTION,
	// :
	COLON,
	// $
	DOLLAR,
	// =
	EQ,
	// !
	BANG,
	// <
	LESS,
	// >
	GREAT,
	// -
	MINUS,
	// &
	AND,
	// |
	PIPE,
	// +
	PLUS,
	// *
	STAR,
	// "/""
	SLASH,
	// ^
	CARET,
	// %
	PERCENT,
	// Any whitespace
	WHITESPACE,
	// Any identifier, this includes keywords
	IDENT,
	// End of file
	_EOF,
	// This is incase we want error tolerance
	UNKNOWN,
};

#ifdef DEBUGGING
const static char* TOKEN_NAMES[] = {
	"COMMENT",		"INT_LIT",		 "FLOAT_LIT",  "CHAR_LIT",	  "STR_LIT",	"SEMI",
	"COMMA",		"DOT",			 "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACE", "CLOSE_BRACE",
	"OPEN_BRACKET", "CLOSE_BRACKET", "AT",		   "POUND",		  "TILDE",		"QUESTION",
	"COLON",		"DOLLAR",		 "EQ",		   "BANG",		  "LESS",		"GREAT",
	"MINUS",		"AND",			 "PIPE",	   "PLUS",		  "STAR",		"SLASH",
	"CARET",		"PERCENT",		 "WHITESPACE", "IDENT",		  "_EOF",		"UNKNOWN",
};
#endif

