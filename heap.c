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
BSTNODE * findExtreme(HEAP *h, BSTNODE *one, BSTNODE *two, BSTNODE *three);

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
        enqueue(h->insertQ, new);
        enqueue(h->queue, new);
        push(h->stack, new);

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

            h->size ++;
        }
        // else if  (getBSTNODEright(temp) == NULL) {
        else {
            setBSTNODEright(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, size+1);
            enqueue(h->insertQ, new);
            enqueue(h->queue, new);
            push(h->stack, new);

            h->size ++;
        }
        if (getBSTNODEleft(temp) != NULL && getBSTNODEright(temp) != NULL) {
            dequeue(h->insertQ);
        }
    }
    // enqueue(h->insertQ, new);
    // enqueue(h->queue, new);
    // push(h->stack, new);
    //
    // h->size ++;
    return;
}



extern void
buildHEAP(HEAP *h) {
    BSTNODE *temp = dequeue(h->queue);
    while (sizeQUEUE(h->queue) > 0) {
        // printf("DEBUGGing: ");
        // h->display(getBSTNODEvalue(temp), stdout);
        // printf("\n");
        // if (getBSTNODEleft(temp) != NULL) {
        //     printf("LEFT: ");
        //     h->display(getBSTNODEvalue(getBSTNODEleft(temp)), stdout);
        //     printf("\n");
        // }
        // if (getBSTNODEright(temp) != NULL) {
        //     printf("RIGHT: ");
        //     h->display(getBSTNODEvalue(getBSTNODEright(temp)), stdout);
        //     printf("\n");
        // }
        // displayHEAPdebug(h, stdout);
        // printf("\n\n");

        BSTNODE *p = getBSTNODEparent(temp);
        if (p!=NULL && h->compare(getBSTNODEvalue(p),getBSTNODEvalue(temp)) > 0) {
            heapifyUP(h, temp, p);
        }
        else {
            heapifyDOWN(h, temp);
        }
        temp = dequeue(h->queue);
        // displayHEAPdebug(h, stdout);
        // printf("\n\n");
    }
    return;
}

void
heapifyDOWN(HEAP *h, BSTNODE *node) {
    // printf("DEBUGGing DOWN: ");
    // h->display(getBSTNODEvalue(node), stdout);
    // printf("\n");
    // if (getBSTNODEleft(node) != NULL) {
    //     printf("LEFT: ");
    //     h->display(getBSTNODEvalue(getBSTNODEleft(node)), stdout);
    //     printf("\n");
    // }
    // if (getBSTNODEright(node) != NULL) {
    //     printf("RIGHT: ");
    //     h->display(getBSTNODEvalue(getBSTNODEright(node)), stdout);
    //     printf("\n");
    // }
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    if (l == NULL) {return;}
    else {
        if (l != NULL && r != NULL) {
            if (h->compare(getBSTNODEvalue(l),getBSTNODEvalue(r)) < 0) {
                if (h->compare(getBSTNODEvalue(node),getBSTNODEvalue(l)) > 0) {
                    // printf("left is the one\n");
                    void *data = getBSTNODEvalue(node);
                    setBSTNODEvalue(node, getBSTNODEvalue(l));
                    setBSTNODEvalue(l, data);
                    enqueue(h->queue, node);
                    heapifyDOWN(h, l);
                }
                else {
                    // printf("apparently nothing\n");
                    return;
                }
            }
            else {
                if (h->compare(getBSTNODEvalue(node),getBSTNODEvalue(r)) > 0) {
                    // printf("right is the one\n");
                    void *data = getBSTNODEvalue(node);
                    setBSTNODEvalue(node, getBSTNODEvalue(r));
                    setBSTNODEvalue(r, data);
                    enqueue(h->queue, node);
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
            enqueue(h->queue, node);
            heapifyDOWN(h, l);
        }
    }
    return;
}

// void
// heapifyDOWN(HEAP *h, BSTNODE *node) {
    // if (node == NULL) {return;}
    //
    // BSTNODE *extreme = node;
    // BSTNODE *l = getBSTNODEleft(node);
    // BSTNODE *r = getBSTNODEright(node);
    //
    // if (l == NULL) {return;}
    //
    // else if (l == NULL && r == NULL) {
    //     // printf("no children, heapifying up\n");
    //     heapifyUP(h, node, getBSTNODEparent(node));
    //     return;
    // }
    // // if (l != NULL && r != NULL) {
    // //     if (h->compare(getBSTNODEvalue(l),getBSTNODEvalue(node))>0 && h->compare(getBSTNODEvalue(r),getBSTNODEvalue(node))>0) {
    // //         heapifyUP(h, node, getBSTNODEparent(node));
    // //         return;
    // //     }
    // // }
    // if (r == NULL) {extreme = l;}
    // // else {printf("finding extreme\n");extreme = findExtreme(h, node, l, r);}
    // //
    // // if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) != 0) {
    // //     void *data = getBSTNODEvalue(extreme);
    // //     setBSTNODEvalue(extreme, getBSTNODEvalue(node));
    // //     setBSTNODEvalue(node, data);
    // //     enqueue(h->queue, node);
    // //     heapifyDOWN(h, extreme);
    // // }
    // // else {
    // //     heapifyUP(h, node, getBSTNODEparent(node));
    // // }
    // if (r == NULL && l != NULL) {extreme = l;}

//     else if (h->compare(getBSTNODEvalue(l),getBSTNODEvalue(r)) > 0) {
//         extreme = r;
//     }
//     else {
//         extreme = l;
//     }
//
//     if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) > 0) {
//         extreme = node;
//     }
//
//     if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) != 0) {
//         void *data = getBSTNODEvalue(extreme);
//         setBSTNODEvalue(extreme, getBSTNODEvalue(node));
//         setBSTNODEvalue(node, data);
//         enqueue(h->queue, node);
//         heapifyDOWN(h, extreme);
//     }
//
//     return;
// }

BSTNODE *
findExtreme(HEAP *h, BSTNODE *one, BSTNODE *two, BSTNODE *three) {
    //BSTNODE *extreme = one;
    if (h->compare(getBSTNODEvalue(one), getBSTNODEvalue(two)) > 0 && h->compare(getBSTNODEvalue(three), getBSTNODEvalue(two)) > 0) {
        return two;
    }
    else if (h->compare(getBSTNODEvalue(one), getBSTNODEvalue(three)) > 0 && h->compare(getBSTNODEvalue(two), getBSTNODEvalue(three)) > 0) {
        return three;
    }
    else {
        return one;
    }
}

void
heapifyUP(HEAP *h, BSTNODE *node, BSTNODE *parent) {
    // printf("DEBUGGing UP: ");
    // h->display(getBSTNODEvalue(node), stdout);
    // printf("\n");
    // if (getBSTNODEleft(node) != NULL) {
    //     printf("LEFT: ");
    //     h->display(getBSTNODEvalue(getBSTNODEleft(node)), stdout);
    //     printf("\n");
    // }
    // if (getBSTNODEright(node) != NULL) {
    //     printf("RIGHT: ");
    //     h->display(getBSTNODEvalue(getBSTNODEright(node)), stdout);
    //     printf("\n");
    // }
    if (parent == NULL) {return;}
    // if (h->compare(getBSTNODEvalue(node),getBSTNODEvalue(parent)) > 0) {return;}
    if (h->compare(getBSTNODEvalue(parent),getBSTNODEvalue(node)) > 0) {
        void *data = getBSTNODEvalue(parent);
        setBSTNODEvalue(parent, getBSTNODEvalue(node));
        setBSTNODEvalue(node, data);
        enqueue(h->queue, node);
        heapifyUP(h, parent, getBSTNODEparent(parent));
    }
    // else {
    //     heapifyDOWN(h, node);
    // }
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
