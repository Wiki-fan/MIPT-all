#include <stdio.h>
#include <string.h>
#include <err.h>
#include <getopt.h>
#include <stdlib.h>
#include "../task-l1-nucleotide_pack/nucleotide_pack.h"

int main(int argc, char* argv[])
{
    char mode;
    char* in, *out;
    int optind = 0;

    int c;

    do
    {
        int curind = optind;
        static struct option long_options[] =
        {
            { "help",    no_argument,       0, 'h' },
            { "version", no_argument,       0, 'v' },
            { "pack",    required_argument, 0, 'p' },
            { "unpack",  required_argument, 0, 'u' },
            { 0, 0,                         0, 0 }
        };
        c = getopt_long(argc, argv, "hv",
                        long_options, &optind);

        switch (c)
        {
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[optind].flag != 0)
                break;
            printf ("option %s", long_options[optind].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;

        case 'h':
            puts ("Usage:\n./nucleotide_pack [pack/unpack] in_file out_file");
            exit(0);
        case 'v':
            puts ("Version 0.1");
            exit(0);
        case 'p':
            mode =  PACK_NUCLEOTIDS;
            optind++;
            if (optind < argc && *argv[optind] != '-')
            {
                in = optarg;
                out = argv[optind];
            }
            else
            {
                errx(20, "--pack option requires TWO arguments");
            }
            break;

        case 'u':
            mode =  UNPACK_NUCLEOTIDS;
            optind++;
            if (optind < argc && *argv[optind] != '-')
            {
                in = optarg;
                out = argv[optind];
            }
            else
            {
                errx(20, "--unpack option requires TWO arguments");
            }
            break;

        case '?':
            /* getopt_longs writes error by itself */
            /*if (optopt)  printf("Bad short opt '%c'\n", optopt);
            else  printf("Bad long opt \"%s\"\n", argv[curind]);*/
            errx(11, "Unknown parameter");

        case -1:
            break;

        default:
            err(3, "Unreachable code");
        }
    }
    while (c != -1);

    nucleotide_pack(mode, in, out);

    return 0;
}