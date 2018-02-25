#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"
#include "integer.h"
#include "bst.h"
#include "heap.h"
#include "scanner.h"
#include "real.h"
#include "string.h"

int main (int argc, char **argv) {
    for (int i=0; i<argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            printf("Ben Bailey\n");
            return 0;
        }
        else if (strcmp(argv[i], "-i") == 0) {
            HEAP *heap = newHEAP(displayINTEGER, compareINTEGER, freeINTEGER);
            FILE *fp;
            fp = fopen(argv[argc-1], "r");

            int temp = readInt(fp);
            insertHEAP(heap, temp);
            temp = readInt(fp);

            while (temp != NULL) {
                insertHEAP(heap, temp);
                temp = readInt(fp);
            }

            buildHEAP(heap);
            for (int i=0; i<sizeheap(heap); i++) {
                void *curr = extracheap(heap);
                heap->display(curr->data);
                
            }


            printf("-integers\n");
        }
        else if (strcmp(argv[i], "-r") == 0) {
            printf("-real numbers\n");
        }
        else if (strcmp(argv[i], "-s") == 0) {
            printf("-strings\n");
        }
        else if (strcmp(argv[i], "-I") == 0) {
            printf("-min heap\n");
        }
        else if (strcmp(argv[i], "-D") == 0) {
            printf("-max heap\n");
        }
    }







    return 0;
}
