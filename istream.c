#include <stdlib.h>

#include "istream.h"

struct istream_t {
    char* input;
    int pos;
};

istream *istream_new(char* input)
{
    istream *is = malloc(sizeof(istream));
    if (is) {
	is->input = input;
	is->pos = 0;
    }
    return is;
}

char istream_peek(istream* is)
{
    return is->input[is->pos];
}

char istream_next(istream *is)
{
    return is->input[is->pos++];
}

int istream_eof(istream* is)
{
    return istream_peek(is) == '\0';
}

void istream_delete(istream *is)
{
    if (is) {
	if (is->input) free(is->input);
	free(is);
    }
}
