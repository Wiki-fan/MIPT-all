#include <string.h>
#include <err.h>
#include "nucleotide_pack.h"

int main(int argc, char* argv[])
{
    char mode;
    if (argc != 4)
    {
        errx(10, "Wrong number of parameters");
    }
    if (!strcmp(argv[1], "pack"))
    {
        mode = PACK_NUCLEOTIDS;
    }
    else if (!strcmp(argv[1], "unpack"))
    {
        mode = UNPACK_NUCLEOTIDS;
    }
    else
    {
        errx(11, "Unknown mode %s", argv[1]);
    }
    nucleotide_pack(mode, argv[2], argv[3]);

    return 0;
}