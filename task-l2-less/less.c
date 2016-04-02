#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <getopt.h>
#include <slcurses.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include "../common/manipulations.h"

void print_strs( char** strs, int* lengths, int top, int left, int w, int h, int lines_number )
{
    int i, j;
    if ( h>lines_number)
    {
        h = lines_number;

    }
    for( i = 0; i < h; ++i )
    {
        if (top + i <lengths[i])
        {
            printf( "%s", strs[top + i] + left );
        }
        else
        {
            putchar('\n');
        }
    }
}

void less( FILE* f, int line_print_flag )
{

    struct winsize winsz;
    int lines_number, columns_number; /* lines in file */
    ioctl( STDIN_FILENO, TIOCGWINSZ, &winsz );
    printf( "col %d row %d\n", winsz.ws_col, winsz.ws_row );

    char** strs;
    int* lengths;
    int symbol;
    bool fl = true;

    read_file( f, &strs, &lengths, &lines_number, &columns_number );
    printf( "%d %d\n", lines_number, columns_number );

    int top = 0, left = 0;
    print_strs( strs,lengths, top, left, winsz.ws_col, winsz.ws_row,/*50,50,*/ lines_number );

    while( fl )
    {
        /*printf( "Был введён символ: код=%d начертание '%c'\n", symbol, (char) symbol );*/
        print_strs( strs,lengths, top, left, winsz.ws_col, winsz.ws_row, lines_number );
        symbol = getchar();
        switch( symbol )
        {
        case 27:
            symbol = getchar();
            switch (symbol)
            {
            case 91:
                symbol = getchar();
                switch( symbol )
                {
                case EOF:
                    break;
                case 'A': /*up*/
                    top -= top == 0 ? 0 : 1;
                    break;
                case 'B': /*down*/
                    top += top == lines_number - winsz.ws_row ? 0 : 1;
                    break;
                case 'C': /*right*/
                    left += left - winsz.ws_col == columns_number ? 0 : 1;
                    break;
                case 'D': /*left*/
                    left -= left == 0 ? 0 : 1;
                    break;
                default:
                    ;
                }
                break;
            case EOF:
                fl = false;
                break;
            default:
                ;
            }
            break;
        case EOF:
            fl = false;
            break;
        default:
            ;
        }
        printf( "%d %d \n", top, left );
        if( symbol == '\004' )
        {
            break;
        }
    }
}


int main( int argc, char* argv[] )
{
    FILE* f;
    int c;

    struct termios old_attributes, new_attributes;
    bool line_print_flag = false;

    if( !isatty( STDIN_FILENO ))
    {
        errx( 20, "Input is redirected not from terminal. Error" );
    }

    tcgetattr( STDIN_FILENO, &old_attributes );
    memcpy( &new_attributes, &old_attributes, sizeof( struct termios ));

    /* Disable echoing. */
    new_attributes.c_lflag &= ~ECHO;
    /* Enable non-canonical mode. */
    new_attributes.c_lflag &= ~ICANON;
    /* Minimum number of characters for non-canonical read */
    new_attributes.c_cc[VMIN] = 1;

    /* Включает жёсткое чтение по одному символу.
     * Терминал будет работать не как сокет.
     * Поскольку как-то оно слишком агрессивно, по моему
     * мнению я это выключу нафик.
     */
    /*
    new_attributes.c_cc[VTIME] = 0;
    */
    tcsetattr( STDIN_FILENO, TCSANOW, &new_attributes );

    printf( "Пожалуйста вводите текст, настройки терминала уже изменены:\n" );

    while(( c = getopt( argc, argv, "n" )) != -1 )
    {
        switch( c )
        {
        case 'n':
            line_print_flag = true;
            break;
        case '?':
            errx( 11, "Unknown parameter" );
        default:
            errx( 3, "Unreachable code" );
        }
    }

    if( argc == NULL)
    {
        f = stdin;
    }
    else
    {
        if( optind >= argc )
        {
            errx( 12, "Expected argument after options\n" );
        }
        f = fopen_s( argv[optind], "r" );
    }
    less( f, line_print_flag );

    tcsetattr( STDIN_FILENO, TCSANOW, &old_attributes );

    printf( "\nНастройки терминала изменены обратно\n" );

    return 0;
}