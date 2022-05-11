
// vim: set tabstop=4 smarttab

#pragma once

namespace zade {

enum token_kind {
	// A comment token
	t_COMMENT,
	// An integer literal
	t_INT_LIT,
	// A float literal
	t_FLOAT_LIT,
	// A char literal
	t_CHAR_LIT,
	// A string literal
	t_STR_LIT,
	// ;
	t_SEMI,
	// ,
	t_COMMA,
	// .
	t_DOT,
	// (
	t_OPEN_PAREN,
	// )
	t_CLOSE_PAREN,
	// {
	t_OPEN_BRACE,
	// }
	t_CLOSE_BRACE,
	// [
	t_OPEN_BRACKET,
	// ]
	t_CLOSE_BRACKET,
	// @
	t_AT,
	// #
	t_POUND,
	// ~
	t_TILDE,
	// ?
	t_QUESTION,
	// :
	t_COLON,
	// $
	t_DOLLAR,
	// =
	t_EQ,
	// !
	t_BANG,
	// <
	t_LESS,
	// >
	t_GREAT,
	// -
	t_MINUS,
	// &
	t_AND,
	// |
	t_PIPE,
	// +
	t_PLUS,
	// *
	t_STAR,
	// "/""
	t_SLASH,
	// ^
	t_CARET,
	// %
	t_PERCENT,
	// Any whitespace
	t_WHITESPACE,
	// Any identifier, this includes keywords
	t_IDENT,
	// End of file
	t__EOF,
	// This is incase we want error tolerance
	t_UNKNOWN,

	// ... //
	number_of_tokens,
};

#ifdef DEBUGGING
const static char* TOKEN_NAMES[number_of_tokens] = {
	"COMMENT",		"INT_LIT",		 "FLOAT_LIT",  "CHAR_LIT",	  "STR_LIT",	"SEMI",
	"COMMA",		"DOT",			 "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACE", "CLOSE_BRACE",
	"OPEN_BRACKET", "CLOSE_BRACKET", "AT",		   "POUND",		  "TILDE",		"QUESTION",
	"COLON",		"DOLLAR",		 "EQ",		   "BANG",		  "LESS",		"GREAT",
	"MINUS",		"AND",			 "PIPE",	   "PLUS",		  "STAR",		"SLASH",
	"CARET",		"PERCENT",		 "WHITESPACE", "IDENT",		  "_EOF",		"UNKNOWN",
};
#endif

}  // namespace zade
