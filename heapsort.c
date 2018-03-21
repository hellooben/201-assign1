#include "heap.h"
#include "bst.h"
#include "sll.h"
#include "dll.h"
#include "stack.h"
#include "queue.h"
#include "integer.h"
#include "real.h"
#include "string.h"
//#include "options.c"
#include "scanner.h"
#include <stdlib.h>
#include <stdio.h>

static void (*display)(void *,FILE *);

int main(int argc, char **argv) {
    // printf("%d\n", argc);
    // for (int i=0; i<argc; i++) {
    //     printf("%s\n", argv[i]);
    // }
    HEAP *h;
    FILE *fp = fopen(argv[argc-1], "r");
    int data = 0;
    int type = 0;
    // printf("just outside of the loop\n");
    if (argc != 2) {
        for (int i=0; i<argc; i++) {
            // printf("right before the switch\n");
            switch (argv[i][1]) {
                // printf("in switch\n");
                case 'v':
                    printf("Author: Ben Bailey\nWhy heapsort runs in nlogn time");
                    exit(-1);
                    break;

                case 'i': //integer
                    data = 0;
                    break;
                case 'r': //real
                    data = 1;
                    break;
                case 's': //string
                    data = 2;
                    break;
                case 'I': //increasing (min heap)
                    type = 0;
                    break;
                case 'D':  //deacreasing (max heap)
                    type = 1;
                    break;
            }
        }
    }
    // printf("sup\n");

    if (data == 1) {
        // printf("after first if\n");
        display = displayREAL;
        if (type == 1) {
            h = newHEAP(displayREAL, rcompareREAL, freeREAL);
        }
        else {
            h = newHEAP(displayREAL, compareREAL, freeREAL);
        }

        double dub = readReal(fp);
        while (!feof(fp)) {
            insertHEAP(h, newREAL(dub));
            dub = readReal(fp);
        }
    }


    else if (data == 2) {
        // printf("after second if\n");
        display = displayREAL;
        if (type == 1) {
            h = newHEAP(displaySTRING, rcompareSTRING, freeSTRING);
        }
        else {
            h = newHEAP(displaySTRING, compareSTRING, freeSTRING);
        }

        int str = stringPending(fp);
        char *temp;
        if (str == 0) {
            temp = readToken(fp);
        }
        else {
            temp = readString(fp);
        }
        while (!feof(fp)) {
            STRING *new = newSTRING(temp);
            insertHEAP(h, new);
            str = stringPending(fp);
            if (str == 0) {
                temp = readToken(fp);
            }
            else {
                temp = readString(fp);
            }
        }
    }


    else {
        // printf("after third if\n");
        display = displayINTEGER;
        if (type == 1) {
            h = newHEAP(displayINTEGER, rcompareINTEGER, freeINTEGER);
        }
        else {
            h = newHEAP(displayINTEGER, compareINTEGER, freeINTEGER);
        }

        int num = readInt(fp);
        while (!feof(fp)) {
            insertHEAP(h, newINTEGER(num));
            num = readInt(fp);
        }
    }


    // printf("about to build\n");
    buildHEAP(h);
    while (sizeHEAP(h) > 0) {
        void *thing = extractHEAP(h);
        display(thing, stdout);
        if (sizeHEAP(h) != 0) {printf(" ");}
    }
    printf("\n");
    fclose(fp);

    return 0;
}
