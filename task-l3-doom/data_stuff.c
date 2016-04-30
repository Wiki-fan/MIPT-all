#include "data_stuff.h"
#include "../common/utils.h"

/* appends character to buffer */
void String_push( String* s, char c )
{
	s->buf[s->cur_pos] = c;
	if( ++( s->cur_pos ) == s->buf_size ) {
		s->buf_size *= 2;
		s->buf = realloc_s( s->buf, s->buf_size * sizeof( char ));
	}
}

void String_init(String* s)
{
	s->buf = malloc_s(200*sizeof(char));
	s->buf_size = 200;
	s->cur_pos = 0;
}