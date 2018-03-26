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
#include <string.h>

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
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        printf("Author: Ben Bailey\nHeap runs in O(nlogn) time because:\nInsert runs in O(1) time\nBuild runs in linear O(n) time\nEach extract call takes O(logn) time\nAs a result, since each extract call takes O(logn) time, n extract calls will be made to get the extreme value of the heap, therefore 'sorting' the heap.\n");
        return 0;
    }
    else {
        for (int i=0; i<argc-1; i++) {
            // printf("right before the switch\n");
            switch (argv[i][1]) {
                // printf("in switch\n");
                case 'i': //integer
                    // printf("found an integer arg\n");
                    data = 0;
                    break;
                case 'r': //real
                    // printf("found an real arg\n");
                    data = 1;
                    break;
                case 's': //string
                    // printf("found an string arg: \n");
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
    // printf("DATA: %d\nTYPE: %d\n", data, type);

    if (data == 1) {
        // printf("after first if\n");
        display = displayREAL;
        if (type == 1) {
            // printf("MAKING A REAL MAX HEAP\n");
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
        display = displaySTRING;
        if (type == 1) {
            // printf("MAKING A NEW MAX HEAP\n");
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

    // displayHEAPdebug(h, stdout);
    // printf("\n\n");
    // printf("about to build\n");
    buildHEAP(h);
    // displayHEAPdebug(h, stdout);
    // printf("\n\n\n");
    while (sizeHEAP(h) > 0) {
        // if (sizeHEAP(h) < 5) {
        //     printf("\n");
        //     displayHEAPdebug(h, stdout);
        //     printf("\n\n");
        // }
        void *thing = extractHEAP(h);
        // printf("EXTRACTED\n");
        display(thing, stdout);
        if (sizeHEAP(h) != 0) {printf(" ");}
        // if (sizeHEAP(h) < 5) {
        //     printf("\n");
        //     displayHEAPdebug(h, stdout);
        //     printf("\n\n");
        // }
    }
    printf("\n");
    // freeHEAP(h);
    fclose(fp);

    return 0;
}
