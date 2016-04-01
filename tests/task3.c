/* Variant 1. */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* assuming matrix to compare goes to input first (all its 9 digits), then until EOF other matrices go as well */
typedef int Matrix[3][3];
typedef struct Matrix* pMatrix;

struct Node
{
    struct Node* next;
    Matrix matrix;
};
typedef struct Node* pNode;

int determ(Matrix* a)
{
    return   (*a)[0][0]*((*a)[1][1]*(*a)[2][2] - (*a)[1][2]*(*a)[2][1])
             -(*a)[1][0]*((*a)[0][0]*(*a)[2][2] - (*a)[0][2]*(*a)[2][0])
             +(*a)[2][0]*((*a)[0][0]*(*a)[1][1] - (*a)[1][0]*(*a)[0][1]);
}

int main()
{
    int i;
    Matrix matrix; /* will compare with */
    struct Node begin; /* list beginning */
    pNode cur = &begin;
    for (i = 0; i<9; ++i)
    {
        scanf("%d", &matrix[0][i]);
    }
    while ( stdin )
    {
        pNode temp = (pNode)malloc(sizeof(struct Node));
        temp->next = 0;
        cur->next = temp;
        cur = temp;
        for (i = 0; i<9; ++i)
        {
            scanf("%d", &(temp->matrix[0][i]));
        }
    }
    cur = &begin;
    while(cur->next != 0)
    {
        if (determ(&cur->next->matrix) == determ(&matrix))
        {
            pNode temp = cur->next;
            cur->next = cur->next->next;
            free(temp);
        }
    }

    return 0;
}