#include "heap.h"
#include "bst.h"
#include "sll.h"
#include "stack.h"
#include "queue.h"
#include "integer.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void heapifyDOWN(HEAP *h, BSTNODE *node);
void heapifyUP(HEAP *h, BSTNODE *node, BSTNODE *parent);

struct heap
{
    BST *bstree;
    BSTNODE *root;
    QUEUE *queue;
    QUEUE *insertQ;
    STACK *stack;
    STACK *insertSTACK;
    int size;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);
    void (*free)(void *);
};

extern HEAP *
newHEAP(
    void (*d)(void *,FILE *),    //display
    int (*c)(void *,void *),     //compare: NULL
    void (*f)(void *)
) {
    HEAP *tree = malloc(sizeof(HEAP));
    assert(tree!=0);
    tree->bstree = newBST(d, c, NULL, f);
    tree->queue = newQUEUE(d, f);
    tree->insertQ = newQUEUE(d, f);
    tree->stack = newSTACK(d, f);
    tree->insertSTACK = newSTACK(d, f);
    tree->size = 0;
    tree->root = NULL;
    tree->display = d;
    tree->compare = c;
    tree->free = f;
    return tree;
}

extern void
insertHEAP(HEAP *h,void *value) {
    BSTNODE *new = newBSTNODE(value);
    BSTNODE *rt = getBSTroot(h->bstree);
    int size = sizeBST(h->bstree);

    if (rt == NULL) {
        setBSTroot(h->bstree, new);
        setBSTsize(h->bstree, sizeBST(h->bstree)+1);
        enqueue(h->insertQ, new);
        enqueue(h->queue, new);
        push(h->stack, new);
        push(h->insertSTACK, new);

        h->size ++;
    }
    else {
        BSTNODE *temp = peekQUEUE(h->insertQ);

        if (getBSTNODEleft(temp) == NULL) {
            setBSTNODEleft(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, size+1);
            enqueue(h->insertQ, new);
            enqueue(h->queue, new);
            push(h->stack, new);
            push(h->insertSTACK, new);

            h->size ++;
        }
        else {
            setBSTNODEright(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, size+1);
            enqueue(h->insertQ, new);
            enqueue(h->queue, new);
            push(h->stack, new);
            push(h->insertSTACK, new);

            h->size ++;
        }
        if (getBSTNODEleft(temp) != NULL && getBSTNODEright(temp) != NULL) {
            dequeue(h->insertQ);
        }
    }
    return;
}



extern void
buildHEAP(HEAP *h) {
    while (sizeSTACK(h->insertSTACK) > 0) {
        BSTNODE *temp = pop(h->insertSTACK);

        heapifyDOWN(h, temp);
    }
    return;
}

void
heapifyDOWN(HEAP *h, BSTNODE *node) {
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    if (l == NULL) {
        return;
    }
    else {
        if (l != NULL && r != NULL) {
            if (h->compare(getBSTNODEvalue(l),getBSTNODEvalue(r)) < 0) {
                if (h->compare(getBSTNODEvalue(node),getBSTNODEvalue(l)) > 0) {
                    void *data = getBSTNODEvalue(node);
                    setBSTNODEvalue(node, getBSTNODEvalue(l));
                    setBSTNODEvalue(l, data);
                    heapifyDOWN(h, l);
                }
                else {
                    return;
                }
            }
            else {
                if (h->compare(getBSTNODEvalue(node),getBSTNODEvalue(r)) > 0) {
                    void *data = getBSTNODEvalue(node);
                    setBSTNODEvalue(node, getBSTNODEvalue(r));
                    setBSTNODEvalue(r, data);
                    heapifyDOWN(h, r);
                }
            }
            return;
        }
        else if (h->compare(getBSTNODEvalue(node),getBSTNODEvalue(l)) > 0) {
            // printf("left is the one, but there is no right\n");
            void *data = getBSTNODEvalue(node);
            setBSTNODEvalue(node, getBSTNODEvalue(l));
            setBSTNODEvalue(l, data);
            // enqueue(h->queue, node);
            heapifyDOWN(h, l);
        }
    }
    return;
}

void
heapifyUP(HEAP *h, BSTNODE *node, BSTNODE *parent) {
    if (parent == NULL) {return;}
    if (h->compare(getBSTNODEvalue(parent),getBSTNODEvalue(node)) > 0) {
        void *data = getBSTNODEvalue(parent);
        setBSTNODEvalue(parent, getBSTNODEvalue(node));
        setBSTNODEvalue(node, data);
        heapifyUP(h, parent, getBSTNODEparent(parent));
    }
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
    BSTNODE *temp = getBSTroot(h->bstree);
    void *returnable = getBSTNODEvalue(temp);

    BSTNODE *leaf = pop(h->stack);
    setBSTNODEvalue(temp, getBSTNODEvalue(leaf));
    setBSTNODEvalue(leaf, returnable);
    pruneLeafBST(h->bstree, leaf);
    h->free(leaf);
    setBSTsize(h->bstree, sizeBST(h->bstree)-1);
    h->size --;
    heapifyDOWN(h, temp);

    return returnable;
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
    while (sizeSTACK(h->stack) > 0) {pop(h->stack);}
    while (sizeSTACK(h->insertSTACK) > 0) {pop(h->insertSTACK);}
    while (sizeQUEUE(h->insertQ) > 0) {dequeue(h->insertQ);}
    while (sizeQUEUE(h->queue) > 0) {dequeue(h->queue);}

    freeQUEUE(h->queue);
    freeSTACK(h->stack);
    freeSTACK(h->insertSTACK);
    freeQUEUE(h->insertQ);
    freeBST(h->bstree);
    free(h);
    return;
}
