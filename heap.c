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
    }
    else {
        BSTNODE *temp = peekQUEUE(h->insertQ);

        if (getBSTNODEleft(temp) == NULL) {
            setBSTNODEleft(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, size+1);
        }
        else if (getBSTNODEright(temp) == NULL) {
            setBSTNODEright(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, size+1);
        }
        if (getBSTNODEleft(temp) != NULL && getBSTNODEright(temp) != NULL) {
            dequeue(h->insertQ);
        }
    }
    enqueue(h->insertQ, new);
    enqueue(h->queue, new);
    push(h->stack, new);

    h->size ++;
    return;
}



extern void
buildHEAP(HEAP *h) {
    while (sizeQUEUE(h->queue) > 0) {
        BSTNODE *temp = dequeue(h->queue);
        BSTNODE *p = getBSTNODEparent(temp);
        if (p!=NULL && h->compare(getBSTNODEvalue(p),getBSTNODEvalue(temp)) > 0) {
            heapifyUP(h, temp, p);
        }
        else {
            heapifyDOWN(h, temp);
        }
    }
    return;
}

void
heapifyDOWN(HEAP *h, BSTNODE *node) {
    if (node == NULL) {return;}

    BSTNODE *extreme = node;
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    if (l == NULL && r == NULL) {
        heapifyUP(h, node, getBSTNODEparent(node));
        return;
    }
    if (l == NULL && r != NULL) {extreme = r;}
    else if (r == NULL && l != NULL) {extreme = l;}

    else if (h->compare(getBSTNODEvalue(l),getBSTNODEvalue(r)) > 0) {
        extreme = r;
    }
    else {
        extreme = l;
    }

    if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) > 0) {
        extreme = node;
    }


    if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) != 0) {
        void *data = getBSTNODEvalue(extreme);
        setBSTNODEvalue(extreme, getBSTNODEvalue(node));
        setBSTNODEvalue(node, data);
        enqueue(h->queue, node);
        heapifyDOWN(h, extreme);
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
        enqueue(h->queue, node);
        heapifyUP(h, parent, getBSTNODEparent(parent));
    }
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

void
rebuild(HEAP *h, BSTNODE *node) {
    if (node == NULL) {return;}

    BSTNODE *extreme = node;
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);
    if (l!=NULL && h->compare(getBSTNODEvalue(l), getBSTNODEvalue(node)) < 0) {
        extreme = l;
    }
    if (r!=NULL && h->compare(getBSTNODEvalue(r), getBSTNODEvalue(node)) < 0) {
        extreme = r;
    }

    if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) != 0) {
        void *data = getBSTNODEvalue(extreme);
        setBSTNODEvalue(extreme, getBSTNODEvalue(node));
        setBSTNODEvalue(node, data);
        rebuild(h, extreme);
    }

    return;
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
    while (sizeQUEUE(h->insertQ) > 0) {dequeue(h->insertQ);}
    while (sizeQUEUE(h->queue) > 0) {dequeue(h->queue);}

    freeQUEUE(h->queue);
    freeSTACK(h->stack);
    freeQUEUE(h->insertQ);
    freeBST(h->bstree);
    free(h);
    return;
}
