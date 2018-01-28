#ifndef TASK_L3__DATA_STUFF
#define TASK_L3__DATA_STUFF

typedef struct
{
    char *buf;
    int buf_size;
    int cur_pos;
} String;

/* appends character to buffer */
void String_push( String* s, char c );

void String_init(String* s);

#endif /* L3__DATA_STUFF */


