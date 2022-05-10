
#include "token_kind.h"

static enum {
    s_error,

    s_whitespace,

    s_start,

    s_reading_whitespace,

    number_of_states,
} lookup[number_of_states][127] = {
    [s_start][' ' ] = s_reading_whitespace,
    [s_start]['\t'] = s_reading_whitespace,
    [s_start]['\n'] = s_reading_whitespace,
        [s_reading_whitespace][ALL] = s_whitespace,
        [s_reading_whitespace][' ' ] = s_reading_whitespace,
        [s_reading_whitespace]['\t'] = s_reading_whitespace,
        [s_reading_whitespace]['\n'] = s_reading_whitespace,
};

int next_token()
{
    
}
