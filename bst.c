#include "bst.h"
#include "sll.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

BSTNODE *binaryInsert(BST *t, BSTNODE *node, BSTNODE *prev, void *value);
BSTNODE *binaryFind(BST *t, BSTNODE *node, void *value);
void displayRecursive(BST *t, BSTNODE *root, FILE *fp);
void freeRecursive(BSTNODE *node, BST *t);
void levelOrder(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq);
int BSTmaximum(BSTNODE *root);
int BSTminimum(BSTNODE *root);

struct bstnode
{
    void *data;
    struct bstnode *parent;
    struct bstnode *left;
    struct bstnode *right;
};

extern BSTNODE *
newBSTNODE(void *v) {
    BSTNODE *new = malloc(sizeof(BSTNODE));
    new->data = v;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

extern void *
getBSTNODEvalue(BSTNODE *n) {
    return n->data;
}

extern void
setBSTNODEvalue(BSTNODE *n,void *value) {
    n->data = value;
    return;
}

extern BSTNODE *
getBSTNODEleft(BSTNODE *n) {
    if (n->left != NULL) {
        return n->left;
    }
    else {
        return NULL;
    }
}

extern void
setBSTNODEleft(BSTNODE *n,BSTNODE *replacement) {
    BSTNODE *curr = getBSTNODEleft(n);
    if (curr != NULL) {
        n->left = replacement;
        //replacement->parent = n;
        //replacement->right = getBSTNODEright(curr);
        //replacement->left = getBSTNODEleft(curr);
        return;
    }
    else {return;}
}

extern BSTNODE *
getBSTNODEright(BSTNODE *n) {
    if (n->right != NULL) {
        return n->right;
    }
    else {
        return NULL;
    }
}

extern void
setBSTNODEright(BSTNODE *n,BSTNODE *replacement) {
    BSTNODE *curr = getBSTNODEright(n);
    if (curr != NULL) {
        n->right = replacement;
        //replacement->parent = n;
        //replacement->right = getBSTNODEright(curr);
        //replacement->left = getBSTNODEleft(curr);
        return;
    }
    else {return;}
}

extern BSTNODE *
getBSTNODEparent(BSTNODE *n) {
    if (n->parent != NULL) {
        return n->parent;
    }
    else {
        return NULL;
    }
}

extern void
setBSTNODEparent(BSTNODE *n,BSTNODE *replacement) {
    BSTNODE *curr = getBSTNODEparent(n);
    if (curr != NULL) {
        n->parent = replacement;
        //replacement->parent = getBSTNODEparent(curr);
        //replacement->right = getBSTNODEright(curr);
        //replacement->left = getBSTNODEleft(curr);
        return;
    }
    else {return;}
}

extern void
freeBSTNODE(BSTNODE *n,void (*f)(void *)) {
    if (f != NULL) {
        f(n->data);
        free(n);
        return;
    }
    else {return;}
}


/*----------------------------------------------------------------------*/


struct bst
{
    //QUEUE *list;
    int size;
    BSTNODE *root;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*swap)(BSTNODE *,BSTNODE *);     //swapper
    void (*free)(void *);                  //free
};

extern BST *
newBST(void (*d)(void *,FILE *),
int (*c)(void *,void *),
void (*s)(BSTNODE *,BSTNODE *),
void (*f)(void *)) {
    BST *tree = malloc(sizeof(BST));
    assert(tree!=0);
    //tree->list = newQUEUE(d, f);
    tree->size = 0;
    tree->root = NULL;
    tree->display = d;
    tree->compare = c;
    tree->swap = s;
    tree->free = f;
    return tree;
}

extern BSTNODE *
getBSTroot(BST *t) {
    if (t->root != NULL) {
        return t->root;
    }
    else {
        return NULL;
    }
}

extern void
setBSTroot(BST *t,BSTNODE *replacement) {
    BSTNODE *curr = getBSTroot(t);
    if (curr != NULL) {
        t->root = replacement;
        //replacement->right = curr->right;
        //replacement->left = curr->left;
        //replacement->parent = NULL;
        return;
    }
    else {return;}
}

extern void
setBSTsize(BST *t,int s) {
    t->size = s;
    return;
}

extern BSTNODE *
insertBST(BST *t,void *value) {
    if (t->root == NULL) {
        BSTNODE *new = newBSTNODE(value);
        assert(new!=0);
        t->root = new;
        t->size ++;
        //enqueue(t->list, new);
        return new;
    }
    else {
        //BSTNODE *curr = t->root;

        BSTNODE *returnable = binaryInsert(t, t->root, t->root, value);
        assert(returnable!=0);
        t->size ++;
        //enqueue(t->list, returnable);
        return returnable;
    }
}

extern BSTNODE *
findBST(BST *t,void *value) {
    if (t->root == NULL) {
        return NULL;
    }
    else {
        BSTNODE *curr = t->root;
        BSTNODE *returnable = binaryFind(t, curr, value);
        return returnable;
    }
}

BSTNODE *
binaryFind(BST *t, BSTNODE *node, void *value) {
    if (node == NULL) {
        return node;
    }
    if (t->compare(node->data, value) == 0) {
        return node;
    }
    else if (t->compare(node->data, value) > 0) {
        //printf("looking at: ");
        //t->display(node->data, stdout);
        //printf(". went left\n");
        return binaryFind(t, node->left, value);
    }
    else if (t->compare(node->data, value) < 0) {
        //printf("looking at: ");
        //t->display(node->data, stdout);
        //printf(". went right\n");
        return binaryFind(t, node->right, value);
    }
    else {
        return NULL;
    }
}

extern BSTNODE *
deleteBST(BST *t,void *value) {
    BSTNODE *current = findBST(t, value);
    //printf("found\n");
    //printf("OLD BST: \n");
    //displayBSTdebug(t, stdout);
    current = swapToLeafBST(t, current);
    //printf("after swapping ");
    //t->display(current->data, stdout);
    //printf(" to a leaf\n");
    //printf("NEW BST: \n");
    //displayBSTdebug(t, stdout);

    //printf("swapped\n");

    pruneLeafBST(t, current);
    t->size --;
    //printf("BST after ");
    //t->display(current->data, stdout);
    //printf(" was pruned: \n");
    //displayBSTdebug(t, stdout);

    return current;
}

extern BSTNODE *
swapToLeafBST(BST *t,BSTNODE *node) {
    //printf("in swap\n");
    if (t->size == 1) {
        return node;
    }
    else if (node->left == NULL && node->right == NULL) {
        //t->display(node->data, stdout);
        //printf(" was swapped to a leaf\n");
        return node;
    }
    else {
        BSTNODE *temp = node;
        if (node->right != NULL) {
            //void *data = getBSTNODEvalue(node);
            //setBSTNODEvalue(node, getBSTNODEvalue(node->right));
            //setBSTNODEvalue(node->right, data);
            node = node->right;
            while (node->left != NULL) {
                //void *data = getBSTNODEvalue(node);
                //setBSTNODEvalue(node, getBSTNODEvalue(node->left));
                //setBSTNODEvalue(node->left, data);
                node = node->left;
            }
        }
        else {
            //void *data = getBSTNODEvalue(node);
            //setBSTNODEvalue(node, getBSTNODEvalue(node->left));
            //setBSTNODEvalue(node->left, data);
            node = node->left;
            while (node->right != NULL) {
                //void *data = getBSTNODEvalue(node);
                //setBSTNODEvalue(node, getBSTNODEvalue(node->right));
                //setBSTNODEvalue(node->right, data);
                node = node->right;
            }
        }
        void *data = getBSTNODEvalue(temp);
        setBSTNODEvalue(temp, getBSTNODEvalue(node));
        setBSTNODEvalue(node, data);
        return swapToLeafBST(t, node);
    }
}

extern void
pruneLeafBST(BST *t,BSTNODE *leaf) {
    //printf("in prune\n");
    if (leaf->left != NULL || leaf->right != NULL) {
        //printf("this isnt a leaf. you fucked up\n");
        return;
    }
    else {
        //printf("it's a leaf\n");
        if (leaf->parent == NULL) {
            //printf("NULLED OUT ROOT\n");
            t->root = NULL;
            return;
        }
        //t->display(leaf->data, stdout);
        //printf("'s parent isnt null\n");
        BSTNODE *par = getBSTNODEparent(leaf);
        //printf("got parent: ");
        //t->display(par->data, stdout);
        //printf("\n");
        if (par->left != NULL) {
            //printf("left isnt null\n");
            if (t->compare(getBSTNODEvalue(par->left), getBSTNODEvalue(leaf)) == 0) {
                //printf("its a left leaf\n");
                par->left = NULL;
                leaf->parent = NULL;
                return;
            }
        }
        if (par->right != NULL) {
            //printf("it's a right leaf\n");
            par->right = NULL;
            leaf->parent = NULL;
            return;
        }
        //leaf->parent = NULL;
        return;
    }
}

extern int
sizeBST(BST *t) {
    return t->size;
}

extern void
statisticsBST(BST *t,FILE *fp) {
    BSTNODE *current = t->root;
    if (current == NULL) {
        fprintf(fp, "Nodes: %d\nMinimum depth: %d\nMaximum depth: %d\n", 0, -1, -1);
        return;
    }
    else {
        int min = BSTminimum(t->root);
        int max = BSTmaximum(t->root);
        fprintf(fp, "Nodes: %d\nMinimum depth: %d\nMaximum depth: %d\n", sizeBST(t), min, max);

        return;
    }
}

int
BSTminimum(BSTNODE *root) {
    if (root == NULL) {
        return -1;
    }
    else {
        int leeft = BSTminimum(root->left);
        int reet = BSTminimum(root->right);

        if (leeft < reet) {
            return leeft + 1;
        }
        else {
            return reet + 1;
        }
    }
}

int
BSTmaximum(BSTNODE *root) {
    if (root == NULL) {
        return -1;
    }
    else {
        int leeft = BSTmaximum(root->left);
        int reet = BSTmaximum(root->right);

        if (leeft > reet) {
            return leeft + 1;
        }
        else {
            return reet + 1;
        }
    }
}

extern void
displayBST(BST *t,FILE *fp) {
    if (t->root == NULL) {
        printf("[]");
        return;
    }
    else {
        displayRecursive(t, t->root, fp);
        return;
    }
}

extern void
displayBSTdebug(BST *t,FILE *fp) {
    if (t->root == NULL) {
        return;
    }
    else {
        QUEUE *newq = newQUEUE(t->display, t->free);
        QUEUE *oldq = newQUEUE(t->display, t->free);
        levelOrder(t, t->root, fp, newq, oldq);
        freeQUEUE(newq);
        freeQUEUE(oldq);
        return;
    }
}

void
levelOrder(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq) {
    int j = 0;
    t->display(node->data, fp);
    printf("\n");
    j ++;
    enqueue(oldq, node);

    while (j<sizeBST(t)) {
        int oldsize = sizeQUEUE(oldq);
        for (int i=0; i<oldsize; i++) {
            node = dequeue(oldq);
            if (node->left != NULL) {
                enqueue(newq, node->left);
            }
            if (node->right != NULL) {
                enqueue(newq, node->right);
            }
        }
        int newsize = sizeQUEUE(newq);
        for (int x=0; x<newsize; x++) {
            node = dequeue(newq);
            t->display(node->data, fp);
            enqueue(oldq, node);
            if (x < newsize-1) {printf(" ");}
            j ++;
        }
        printf("\n");
    }
    //printf("SIZE OF THE QUEUES:\nold: %d\nnew: %d\n", sizeQUEUE(oldq), sizeQUEUE(newq));
    for (int i=0; i<=sizeQUEUE(oldq); i++) {
        dequeue(oldq);
    }
    //printf("SIZE OF THE QUEUES:\nold: %d\nnew: %d\n", sizeQUEUE(oldq), sizeQUEUE(newq));
    return;
}

extern void freeBST(BST *t) {
    if (t->root == NULL) {
        free(t);
        return;
    }
    else {
        //printf("in free, queue size is %d, tree size is %d\n", sizeQUEUE(t->list), sizeBST(t));
        //displayQUEUE(t->list, stdout);
        //printf("\n");
        BSTNODE *curr = t->root;
        freeRecursive(curr, t);
        //freeQUEUE(t->list);
        //printf("freed queue\n");
        free(t);
        return;
    }
}

void
freeRecursive(BSTNODE *node, BST *t) {
    if (node == NULL) {
        return;
    }

    BSTNODE *temp = node;
    freeBSTNODE(node, t->free);

    if (temp->right != NULL) {
        freeRecursive(temp->right, t);
    }
    if (temp->left != NULL) {
        freeRecursive(temp->left, t);
    }

}

BSTNODE *
binaryInsert(BST *t, BSTNODE *node, BSTNODE *prev, void *value) {
    if (node == NULL) {
        BSTNODE *new = newBSTNODE(value);
        new->parent = prev;
        return new;
    }
    else if (t->compare(node->data, value) < 0) {
        node->right = binaryInsert(t, node->right, node, value);
    }
    else if (t->compare(node->data, value) > 0) {
        node->left = binaryInsert(t, node->left, node, value);
    }
    return node;
}

void
displayRecursive(BST *t, BSTNODE *root, FILE *fp) {
    if (root == NULL) {
        printf("[]");
        return;
    }

    printf("[");
    t->display(root->data, fp);

    if (root->left != NULL) {
        printf(" ");
        displayRecursive(t, root->left, fp);
    }
    if (root->right != NULL) {
        printf(" ");
        displayRecursive(t, root->right, fp);

    }
    printf("]");
}
