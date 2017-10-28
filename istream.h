#ifndef istream_h
#define istream_h

typedef struct istream_t istream;

istream *istream_new(char*);

char istream_peek(istream*);
char istream_next(istream*);
int istream_eof(istream*);

void istream_delete(istream*);

#endif
