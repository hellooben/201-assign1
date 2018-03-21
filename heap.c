#include "heap.h"
#include "bst.h"
#include "sll.h"
#include "stack.h"
#include "queue.h"
#include "integer.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// void heapify(HEAP *h, BSTNODE *node);
//void heapifydown(HEAP *h, BSTNODE *node);
//BSTNODE *getSibling(HEAP *h, BSTNODE *node);
void rebuild(HEAP *h, BSTNODE *node);
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

// extern void
// insertHEAP(HEAP *h,void *value) {
//     BSTNODE *new = insertBST(h->bstree, value);
//     // printf("ROOT: ");
//     // h->display(getBSTNODEvalue(getBSTroot(h->bstree)), stdout);
//     // printf("\n");
//     enqueue(h->queue, new);
//     // printf("QQQQQQ: ");
//     // h->display(getBSTNODEvalue(peekQUEUE(h->queue)), stdout);
//     // printf("\n");
//     push(h->stack, new);
//     h->size ++;
//     return;
// }



extern void
insertHEAP(HEAP *h,void *value) {
    BSTNODE *new = newBSTNODE(value);
    BSTNODE *rt = getBSTroot(h->bstree);
    int size = sizeBST(h->bstree);
    // printf("got root\n");

    if (rt == NULL) {
        // printf("setting root\n");
        setBSTroot(h->bstree, new);
        setBSTsize(h->bstree, sizeBST(h->bstree)+1);
        //return;
    }
    else {
        BSTNODE *temp = peekQUEUE(h->insertQ);
        // printf("node ready to be dequeued: ");
        // h->display(getBSTNODEvalue(temp), stdout);
        // printf("\n");

        if (getBSTNODEleft(temp) == NULL) {
            // printf("left is null\n");
            setBSTNODEleft(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, size+1);
        }
        else if (getBSTNODEright(temp) == NULL) {
            // printf("right is null\n");
            setBSTNODEright(temp, new);
            setBSTNODEparent(new, temp);
            setBSTsize(h->bstree, size+1);
        }
        if (getBSTNODEleft(temp) != NULL && getBSTNODEright(temp) != NULL) {
            // printf("both children are present, dequeueing\n");
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
    //displaySTACK(h->stack, stdout);
    // int size = sizeQUEUE(h->queue);
    //heapify(h, peekSTACK(h->stack));
    // for (int i=0; i<size; i++) {
    while (sizeQUEUE(h->queue) > 0) {
        BSTNODE *temp = dequeue(h->queue);
        // printf("TEMP: ");
        // h->display(getBSTNODEvalue(temp), stdout);
        // printf("\n");

        BSTNODE *p = getBSTNODEparent(temp);
        if (p!=NULL && h->compare(getBSTNODEvalue(p),getBSTNODEvalue(temp)) > 0) {
            heapifyUP(h, temp, p);
            // printf("heapify up ");
            // h->display(getBSTNODEvalue(temp), stdout);
            // printf("\n");
        }
        else {
            heapifyDOWN(h, temp);
        }

        //heapify(h, temp);
    }
    return;
}

void
heapifyDOWN(HEAP *h, BSTNODE *node) {
    if (node == NULL) {return;}

    // heapify(h, getBSTNODEleft(node));
    // heapify(h, getBSTNODEright(node));

    BSTNODE *extreme = node;
    // printf("EXTREME: ");
    // h->display(getBSTNODEvalue(extreme), stdout);
    // printf("\n");
    BSTNODE *l = getBSTNODEleft(node);
    // printf("LEFT: ");
    // h->display(getBSTNODEvalue(l), stdout);
    // printf("\n");
    BSTNODE *r = getBSTNODEright(node);
    // printf("RIGHT: ");
    // h->display(getBSTNODEvalue(r), stdout);
    // printf("\n");
    //BSTNODE *childExtreme = node;
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
        //heapify(h, r);
    }


    if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) != 0) {
        void *data = getBSTNODEvalue(extreme);
        // printf("DATA: ");
        // h->display(data, stdout);
        // printf("\n");
        setBSTNODEvalue(extreme, getBSTNODEvalue(node));
        setBSTNODEvalue(node, data);
        enqueue(h->queue, node);
        heapifyDOWN(h, extreme);
    }
    return;
}

void
heapifyUP(HEAP *h, BSTNODE *node, BSTNODE *parent) {
    // printf("IN UP !!!!!!!!!\n");
    if (parent == NULL) {return;}
    if (h->compare(getBSTNODEvalue(parent),getBSTNODEvalue(node)) > 0) {
        void *data = getBSTNODEvalue(parent);
        // printf("DATA: ");
        // h->display(data, stdout);
        // printf("\n");
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
    // BSTNODE *temp = dequeue(h->queue);
    BSTNODE *temp = getBSTroot(h->bstree);
    void *returnable = getBSTNODEvalue(temp);
    // printf("TEMP: ");
    // h->display(returnable, stdout);
    // printf("\n");

    BSTNODE *leaf = pop(h->stack);
    setBSTNODEvalue(temp, getBSTNODEvalue(leaf));
    setBSTNODEvalue(leaf, returnable);
    pruneLeafBST(h->bstree, leaf);
    setBSTsize(h->bstree, sizeBST(h->bstree)-1);
    h->size --;
    heapifyDOWN(h, temp);

    return returnable;
}

// extern void *
// extractHEAP(HEAP *h) {
//     BST *t = h->bstree;
//     BSTNODE *node = getBSTroot(t);
//     BSTNODE *leaf = pop(h->stack);
//     displaySTACK(h->stack, stdout);
//     printf("BEFORE THE INITIAL SWITCH\n");
//     printf("NODE: ");
//     h->display(getBSTNODEvalue(node), stdout);
//     printf("\n");
//     printf("LEAF: ");
//     h->display(getBSTNODEvalue(leaf), stdout);
//     printf("\n");
//     void *data = getBSTNODEvalue(node);
//     setBSTNODEvalue(node, getBSTNODEvalue(leaf));
//     setBSTNODEvalue(leaf, data);
//     printf("AFTER THE SWITCH\n");
//     printf("NODE: ");
//     h->display(getBSTNODEvalue(node), stdout);
//     printf("\n");
//     printf("LEAF: ");
//     h->display(getBSTNODEvalue(leaf), stdout);
//     printf("\n");
//     printf("\n");
//     pruneLeafBST(t, leaf);
//     rebuild(h, node);
//     h->size --;
//     setBSTsize(t, sizeBST(t)-1);
//     return getBSTNODEvalue(leaf);
// }

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
        // printf("DATA: ");
        // h->display(data, stdout);
        // printf("\n");
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
    //printf("qs: %d\nss: %d\n", sizeQUEUE(h->queue), sizeSTACK(h->stack));
    //freeQUEUE(h->queue);
    //freeSTACK(h->stack);
    freeBST(h->bstree);
    free(h);
    return;
}

// void
// heapify(HEAP *h, BSTNODE *node) {
//     if (node == NULL) {return;}
//
//     heapify(h, getBSTNODEleft(node));
//     heapify(h, getBSTNODEright(node));
//
//     BSTNODE *extreme = node;
//     // printf("EXTREME: ");
//     // h->display(getBSTNODEvalue(extreme), stdout);
//     // printf("\n");
//     BSTNODE *l = getBSTNODEleft(node);
//     // printf("LEFT: ");
//     // h->display(getBSTNODEvalue(l), stdout);
//     // printf("\n");
//     BSTNODE *r = getBSTNODEright(node);
//     // printf("RIGHT: ");
//     // h->display(getBSTNODEvalue(r), stdout);
//     // printf("\n");
//     if (l!=NULL && h->compare(getBSTNODEvalue(l), getBSTNODEvalue(node)) < 0) {
//         extreme = l;
//         //heapify(h, l);
//     }
//     if (r!=NULL && h->compare(getBSTNODEvalue(r), getBSTNODEvalue(node)) < 0) {
//         extreme = r;
//         //heapify(h, r);
//     }
//
//
//     if (h->compare(getBSTNODEvalue(extreme), getBSTNODEvalue(node)) != 0) {
//         void *data = getBSTNODEvalue(extreme);
//         // printf("DATA: ");
//         // h->display(data, stdout);
//         // printf("\n");
//         setBSTNODEvalue(extreme, getBSTNODEvalue(node));
//         setBSTNODEvalue(node, data);
//         // heapify(h, getBSTNODEleft(extreme));
//         // heapify(h, getBSTNODEright(extreme));
//     }
//
//     // heapify(h, getBSTNODEleft(node));
//     // heapify(h, getBSTNODEright(node));
//     return;
// }


/*
void
heapify(HEAP *h, BSTNODE *node) {
    if (getBSTNODEparent(node) == NULL) {
        //heapifydown(h, node);
        return;
    }
    else {
        BSTNODE *par = getBSTNODEparent(node);
        printf("Has a parent\nNode: ");
        h->display(getBSTNODEvalue(node), stdout);
        printf("\n");
        printf("Parent: ");
        h->display(getBSTNODEvalue(par), stdout);
        printf("\n");

        if (h->compare(getBSTNODEvalue(node), getBSTNODEvalue(par)) < 0) {
            printf("parent is greater\n");
            void *data = getBSTNODEvalue(par);
            setBSTNODEvalue(par, getBSTNODEvalue(node));
            setBSTNODEvalue(node, data);

            printf("AFTER SWITCHING: Node: ");
            h->display(getBSTNODEvalue(node), stdout);
            printf("\n");
            printf("Parent: ");
            h->display(getBSTNODEvalue(par), stdout);
            printf("\n");

            node = par; // this is the problem
            h->display(getBSTNODEvalue(node), stdout);
            if (getBSTNODEparent(node) != NULL) {
                par = getBSTNODEparent(node);
                h->display(getBSTNODEvalue(par), stdout);
            }
            printf("hi\n");
            while (par != NULL && h->compare(getBSTNODEvalue(node), getBSTNODEvalue(par)) < 0) {
                printf("in the while\n");
                //par = getBSTNODEparent(node);
                void *data = getBSTNODEvalue(par);
                setBSTNODEvalue(par, getBSTNODEvalue(node));
                setBSTNODEvalue(node, data);

                printf("AFTER SWITCHING: Node: ");
                h->display(getBSTNODEvalue(node), stdout);
                printf("\n");
                printf("Parent: ");
                h->display(getBSTNODEvalue(par), stdout);
                printf("\n");

                node = par;
                par = getBSTNODEparent(node);

            }
            printf("out of the while\n");
        }
        printf("DONE HEAPIFYING\n\n");
        return;
    }
}
*/


/*
void
heapifydown(HEAP *h, BSTNODE *node) {
    if (getBSTNODEleft(node) == NULL && getBSTNODEright(node) == NULL) {
        return;
    }
    else {
        BSTNODE *left = NULL;
        BSTNODE *right = NULL;
        BSTNODE *extreme = NULL;
        if (getBSTNODEleft(node) != NULL) {left = getBSTNODEleft(node);}
        if (getBSTNODEright(node) != NULL) {right = getBSTNODEright(node);}

        if (h->compare(getBSTNODEvalue(left), getBSTNODEvalue(right)) < 0) {
            extreme = left;
        }
        else if (h->compare(getBSTNODEvalue(left), getBSTNODEvalue(right)) > 0) {
            extreme = right;
        }
    }
}*/

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
