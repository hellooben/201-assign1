#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "stack.h"
#include "queue.h"
#include "heap.h"
#include "scanner.h"


extern void readIntegers(HEAP *h, FILE *fp);
extern void readReals(HEAP *h, FILE *fp);
extern void readStringsAndTokens(HEAP *h, FILE *fp);


int
main(int argc, char **argv) {
  // printf("beginning of main\n");
  // static void (*reader)(FILE *,HEAP *);

  static int (*compare)(void *,void *);
  static void (*display)(void *,FILE *);
  static void (*release)(void *);

  HEAP *h;
  FILE *fp = fopen(argv[argc-1], "r");
  int sort = 0;
  int var  = 0;

  for (int i=0; i<argc; i++) {
    switch (argv[i][1]) {
      case 'v':
        printf("Ben Bailey\n");
        exit(-1);
        break;

      case 'i':
        var = 0;
        break;

      case 'r':
        var = 1;
        break;

      case 's':
        var = 2;
        break;

      case 'I':
        sort = 0;
        break;

      case 'D':
        sort = 1;
        break;
    }
  }

    if (var == 1) {
    display = displayREAL;
    release = freeREAL;
    if (sort == 1) compare = compareREALdecr;
    else compare = compareREAL;
    h = newHEAP(display, compare, release);
    readReals(h, fp);
}
  else if (var == 2) {
    display = displaySTRING;
    release = freeSTRING;
    if (sort == 1) compare = compareSTRINGdecr;
    else compare = compareSTRING;
    h = newHEAP(display, compare, release);
    readStringsAndTokens(h, fp);
  }
  else {
    display = displayINTEGER;
    release = freeINTEGER;
    if (sort == 1) compare = compareINTEGERdecr;
    else compare = compareINTEGER;
    h = newHEAP(display, compare, release);
    readIntegers(h, fp);
  }

  buildHEAP(h);
  while (sizeHEAP(h) > 0) {
    void *temp = extractHEAP(h);
    if (sizeHEAP(h) == 0) {
      display(temp, stdout);
    }
    else {
      display(temp, stdout);
      printf(" ");
    }
  }
  printf("\n");

  fclose(fp);

  return 0;
}

extern void
readIntegers(HEAP *h, FILE *fp) {
  int temp = readInt(fp);
  while (!feof(fp)) {
    insertHEAP(h, newINTEGER(temp));
    temp = readInt(fp);
  }
}

extern void
readReals(HEAP *h, FILE *fp) {
  double temp = readReal(fp);
  while (!feof(fp)) {
    insertHEAP(h, newREAL(temp));
    temp = readReal(fp);
  }
}

extern void
readStringsAndTokens(HEAP *h, FILE *fp) {
  int sot = stringPending(fp);
  char *temp;
  if (sot == 0) temp = readToken(fp);
  else temp = readString(fp);
  while (!feof(fp)) {
    STRING *temp2 = newSTRING(temp);
    insertHEAP(h, temp2);
    sot = stringPending(fp);
    if (sot == 0) temp = readToken(fp);
    else temp = readString(fp);
  }
}
