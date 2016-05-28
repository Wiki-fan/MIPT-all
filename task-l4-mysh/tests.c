#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../common/utils.h"

#include "proc_stuff.h"
#include "parser_stuff.h"

TOKEN gettoken (char* buf);

void test_fork_execve()
{
    char* argv[3];
    proc st = { 0, 1, 2, 0, 0, 0, 0 };
    argv[0] = "/bin/ls";
    argv[1] = "-la";
    argv[3] = NULL;
    st.argv = argv;
    run( &st, -1 );
}

TOKEN test_gettoken()
{
    TOKEN t;
    char* buf;

    t = gettoken( buf );
    switch( t )
    {
    case T_BAR:
        printf( "T_BAR\n" );
        break;
    case T_GT:
        printf( "T_GT\n" );
        break;
    case T_GTGT:
        printf( "T_GTGT\n" );
        break;
    case T_LT:
        printf( "T_LT\n" );
        break;
    case T_AMP:
        printf( "T_AMP\n" );
        break;
    case T_WORD:
        printf( "T_WORD %s\n", buf );
        break;
    case T_NL:
        printf( "T_NL\n" );
        break;
    case T_END:
        printf( "END\n" );
        break;
    default:
        errx( 30, "Missed token" );
    }
    free( buf );
    return t;
}

void test_prompt()
{
    TOKEN token = T_NL;
    while( 1 )
    {
        if( token == T_NL )
        {
            printf( PROMPT );
        }
        token = test_gettoken();
        if( token == T_END )
        {
            break;
        }
    }
}

void testdup2()
{
    int fd, c=0, num;
    CHN1( fd = open( "main.c", O_RDONLY, 0 ), 29, "Error opening file" );

    /*CHN1( dup2( fd, STDIN_FILENO ), 30, "Dup2 failed" );*/
    /* close unneeded descriptor */
    /*CHN1( close( fd ), 31, "Can't close file" );*/
    while( num = read(fd/*STDIN_FILENO*/, &c, 1))
    {
        if (num == 0) break;
        if (num == -1) err(21, "error");
        putchar( c );
    }
    close( STDIN_FILENO );
}
int main()
{
    /*test_fork_execve();*/
    /*test_prompt();*/
    testdup2();
    return 0;
}