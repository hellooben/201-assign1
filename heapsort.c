#include "heap.h"
#include "bst.h"
#include "sll.h"
#include "dll.h"
#include "stack.h"
#include "queue.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "scanner.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void (*display)(void *,FILE *);

int main(int argc, char **argv) {
    HEAP *h;
    FILE *fp = fopen(argv[argc-1], "r");
    int data = 0;
    int type = 0;
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        printf("Author: Ben Bailey\nHeap runs in O(nlogn) time because:\nInsert runs in O(1) time\nBuild runs in linear O(n) time\nEach extract call takes O(logn) time\nAs a result, since each extract call takes O(logn) time, n extract calls will be made to get the extreme value of the heap, therefore 'sorting' the heap.\n");
        return 0;
    }
    else {
        for (int i=0; i<argc-1; i++) {
            switch (argv[i][1]) {
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


    if (data == 1) {
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
        display = displaySTRING;
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
