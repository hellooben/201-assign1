#include "heap.h"
#include "bst.h"
#include "sll.h"
#include "stack.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void heapify(HEAP *h, BSTNODE *node);
BSTNODE *getSibling(HEAP *h, BSTNODE *node);

struct heap
{
    BST *bstree;
    BSTNODE *root;
    QUEUE *insertQ;
    QUEUE *buildQ;
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
        tree->insertQ = newQUEUE(d, f);
        tree->buildQ = newQUEUE(d, f);
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
    printf("got root\n");

    if (rt == NULL) {
        printf("setting root\n");
        setBSTroot(h->bstree, new);
        setBSTsize(h->bstree, sizeBST(h->bstree)+1);
        //return;
    }
    else {
        BSTNODE *temp = peekQUEUE(h->insertQ);
        printf("node ready to be dequeued: ");
        h->display(getBSTNODEvalue(temp), stdout);
        printf("\n");

        if (getBSTNODEleft(temp) == NULL) {
            printf("left is null\n");
            setBSTNODEleft(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, sizeBST(h->bstree)+1);
        }
        else if (getBSTNODEright(temp) == NULL) {
            printf("right is null\n");
            setBSTNODEright(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, sizeBST(h->bstree)+1);
        }
        if (getBSTNODEleft(temp) != NULL && getBSTNODEright(temp) != NULL) {
            printf("both children are present, dequeueing\n");
            dequeue(h->insertQ);
        }
    }
    enqueue(h->insertQ, new);
    enqueue(h->buildQ, new);
    push(h->stack, new);

    h->size ++;
    return;
}

extern void
buildHEAP(HEAP *h) {
    //displaySTACK(h->stack, stdout);
    for (int i=sizeQUEUE(h->buildQ)/2; i>0; i--) {
        BSTNODE *temp = dequeue(h->buildQ);
        heapify(h, temp);
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
    if (sizeHEAP(h) == 0) {
        h->root = node;
        return;
    }
    else {

    }
}

/*
BSTNODE *
getSibling(HEAP *h, BSTNODE *node) {
    if (node->parent != NULL) {
        BSTNODE *par = node->parent;
        if (par->left != NULL) {
            BSTNODE *l = par->left;
        }
        if (par->right != NULL) {
            BSTNODE *r = par->right;
        }
        if (h->compare(node->data, l->data) == 0) {
            return r;
        }
        else {
            return l;
        }
    }
    else {return node;}
}
*/
