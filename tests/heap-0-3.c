#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "stack.h"
#include "queue.h"
#include "heap.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(4);
    //INTEGER test of HEAP
    int i;
    HEAP *p = newHEAP(displayINTEGER,rcompareINTEGER,freeINTEGER);
    for (i = 0; i < 11; ++i)
        {
        int j = random() % 11;
        INTEGER *a = newINTEGER(j);
        insertHEAP(p,a);
        }
    printf("peek: %d\n",getINTEGER(peekHEAP(p)));
    if (sizeHEAP(p) < 200)
        {
        printf("heap: ");
        displayHEAP(p,stdout);
        printf("\n");
        printf("debug:\n");
        displayHEAPdebug(p,stdout);
        }
    printf("size is %d\n",sizeHEAP(p));
    freeHEAP(p);
    return 0;
    }