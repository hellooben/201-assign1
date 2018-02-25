#include "heap.h"
#include "bst.h"
#include "sll.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void heapify(HEAP *h, BSTNODE *node);

struct heap
{
    BST *bstree;
    BSTNODE *root;
    //QUEUE *queue;
    STACK *stack;
    int size;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);
    void (*free)(void *);
};

extern HEAP *
newHEAP(
    void (*d)(void *,FILE *),    //display
    int (*c)(void *,void *),     //compare: NULL
    void (*f)(void *)) {
        HEAP *tree = malloc(sizeof(HEAP));
        assert(tree!=0);
        tree->bstree = newBST(d, c, NULL, f);
        //tree->queue = newQUEUE(d, f);
        tree->stack = newSTACK(d, f);
        tree->size = 0;
        tree->root = NULL;
        tree->display = d;
        tree->compare = c;
        tree->free = f;
        return tree;
}

extern void
insertHEAP(HEAP *h,void *value) {
    //BSTNODE *new = newBSTNODE(value);
    //insertBST(h->bstree, value);
    push(h->stack, new);
    h->size ++;
    return;
}

extern void
buildHEAP(HEAP *h) {
    for (int i=sizeSTACK(H->stack)/2; i>0; i--) {
        heapify(h, h->root);
    }
    h->root = getBSTroot(h->bstree);
    return;
}

extern void *
peekHEAP(HEAP *h) {
    if (sizeBST(h->bstree) == 0) {return NULL;}
    else {
        return getBSTNODEvalue(getBSTroot(h->bstree));
    }
}

extern void *
extractHEAP(HEAP *h) {
    BSTNODE *returnable = deleteBST(h->bstree, getBSTNODEvalue(getBSTroot(h->bstree)));
    h->size --;
    return getBSTNODEvalue(returnable);
}

extern int
sizeHEAP(HEAP *h) {
    return h->size;
}

extern void
displayHEAP(HEAP *h,FILE *fp) {
    displayBST(h->bstree, fp);
    return;
}

extern void
displayHEAPdebug(HEAP *h, FILE *fp) {
    printf("heap size: %d\nbst size: %d\n", sizeHEAP(h), sizeBST(h->bstree));
    displayBSTdebug(h->bstree, fp);
    return;
}

extern void
freeHEAP(HEAP *h) {
    freeBST(h->bstree);
    free(h);
    return;
}

void
heapify(HEAP *h, BSTNODE *node) {
    if (node == NULL) {return;}
    BSTNODE *left = getBSTNODEleft(node);
    BSTNODE *right = getBSTNODEright(node);
    BSTNODE *key = node;
    if (h->compare(getBSTNODEvalue(left), getBSTNODEvalue(node)) > 0) {
        key = left;
    }
    if (h->compare(getBSTNODEvalue(right), getBSTNODEvalue(key)) > 0) {
        key = right;
    }
    if (h->compare(getBSTNODEvalue(key), getBSTNODEvalue(node)) != 0) {
        void *data = getBSTNODEvalue(node);
        setBSTNODEvalue(node, getBSTNODEvalue(key));
        setBSTNODEvalue(key, data);
        heapify(h, key);
    }
    //else {return;}
}
