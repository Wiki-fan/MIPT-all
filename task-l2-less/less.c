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

FILE* fileopen(const char* filename, const char* mode)
{
    FILE* f = fopen(filename, mode);
    if (f == NULL)
    {
        err(2, "Error opening file %s", filename);
    }
    return f;
}

void less()
{

}

int main(int argc, char* argv[])
{
    FILE* f;
    int opt;
    struct termios old_attributes,new_attributes;
    bool line_print_flag = false;
    if(!isatty(0))
    {
        errx(3, "Input is redirected not from terminal. Error");
    }

    while ((opt = getopt(argc, argv, "n")))
    {
        switch(opt)
        {
        case 'n':
            line_print_flag = true;
            break;
        case 'h':
            printf("Help here");
            break;
        case 'v':
            printf("Version 0.1");
            break;
        default:
            less();
        }
    }
    f = fileopen(argv[1], "r");
    tcgetattr(0,&old_attributes);

    memcpy(&new_attributes,&old_attributes,sizeof(struct termios));

    /* Disable echoing. */
    new_attributes.c_lflag &= ~ECHO;

    /*
     * По идее теперь не нужно будет нажимать
     * <enter> для ввода символа. Однако,
     *  в результате ctrl+d тоже перестанет восприниматься
     *  как конец файла.
     */
    new_attributes.c_lflag &= ~ICANON;

    /*
     * Выставляем, чтобы терминал считывал
     * с клавиатуры по одному символу,
     * включая стрелочки.
     */
    new_attributes.c_cc[VMIN] = 1;

    /* Включает жёсткое чтение по одному символу.
     * Терминал будет работать не как сокет.
     * Поскольку как-то оно слишком агрессивно, по моему
     * мнению я это выключу нафик.
     */
    /*
    new_attributes.c_cc[VTIME] = 0;
    */

    tcsetattr(0,TCSANOW,&new_attributes);

    printf("Пожалуйста вводите текст, настройки терминала уже изменены:\n");

    while((symbol=getchar())!=EOF)
    {
        printf("Был введён символ: код=%d начертание '%c'\n",symbol,(char)symbol);

        /*
         * О ужас, теперь, чтобы всё-таки сохранить иллюзию
         * старого поведения на ctrl+d мы вынуждены делать такой хак.
         */
        if (symbol == '\004')
            break;
    }

    tcsetattr(0,TCSANOW,&old_attributes);

    printf("\nНастройки терминала изменены обратно\n");


    return 0;
}