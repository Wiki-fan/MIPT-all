#include "unistd.h"
int fread_array(int* array, int size, int fd)
{
    char* tmp_pointer = (char*) array;
    size *= sizeof(int);
    while(size>0)
    {
        ssize_t just_read = read(fd, tmp_pointer, size);
        if (just_read < 0) return -1; /* Error. */
        if (just_read == 0)   /* EOF reached. */
        {
            return 0;
        }
        size -= just_read;
        tmp_pointer += just_read;
    }
    return 1; /* Success. Warning: not the number of characters returned. */
}