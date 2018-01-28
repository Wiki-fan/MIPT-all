#include <stdio.h>
int printsmth( int a, int b, int c, int d, int e, int f)
{
    printf("%d %d %d %d %d %d\n", a, b);
    return 13;
}
int main()
{
    int a = 10, b = 11, c=12,d=13,e=14,f=15;
    int t = printsmth(a,b,c,d,e,f);
    return 0;
}