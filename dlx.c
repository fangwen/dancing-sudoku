#include <stdio.h>
#include <stdlib.h>

struct node {
    struct node *left, *right, *up, *down;
    struct node *ch;
    int r;
    int c;
};

struct node *init(int nrow, int ncol)
{
    struct node *head = (struct node *) malloc(sizeof(struct node));
    head->left = head->right = head;
    head->down = head->up = head;
    struct node **p = (struct node **) malloc(ncol * sizeof(struct node *));
    struct node **q = (struct node **) malloc(nrow * sizeof(struct node *));
    int i;
    for (i = 0; i < ncol; i++)
        p[i] = (struct node *) malloc(sizeof(struct node));
    for (i = 0; i < nrow; i++)
        q[i] = (struct node *) malloc(sizeof(struct node));

    for (i = 0; i< ncol; i++) {
        p[i]->c = i;

        p[i]->right = head;
        p[i]->left = head->left;
        p[i]->left->right = p[i];
        head->left = p[i];

        p[i]->up = p[i];
        p[i]->down = p[i];
    }

    for (i = 0; i< nrow; i++) {
        q[i]->r = i;

        q[i]->down = head;
        q[i]->up = head->up;
        q[i]->up->down = q[i];
        head->up = q[i];

        q[i]->right = q[i];
        q[i]->left = q[i];
    }

    return head;
}

void insert(struct node *h, int r, int c)
{
    struct node *pc = h->right;
    while (pc->c != c)
        pc = pc->right;
    struct node *tmp = (struct node *) malloc(sizeof(struct node));
    tmp->r = r; tmp->c = c; tmp->ch = pc;
    struct node *p = pc;
    while (p->down != pc && p->down->r < r)
        p = p->down;
    tmp->up = p;
    tmp->down = p->down;
    p->down->up = tmp;
    p->down = tmp;

    struct node *pr = h->down;
    while (pr->r != r)
        pr = pr->down;
    struct node *q = pr;
    while (q->right != pr && q->right->c < c)
        q = q->right;
    tmp->left = q;
    tmp->right = q->right;
    q->right->left = tmp;
    q->right = tmp;
}

void cover(struct node *p)
{
    p->right->left = p->left;
    p->left->right = p->right;
    struct node *tmp1, *tmp2;
    for (tmp1 = p->down; tmp1 != p; tmp1 = tmp1->down) {
        for (tmp2 = tmp1->right; tmp2 != tmp1; tmp2 = tmp2->right) {
            tmp2->down->up = tmp2->up;
            tmp2->up->down = tmp2->down;
        }
    }
}

void uncover(struct node *p)
{
    struct node *tmp1, *tmp2;
    for (tmp1 = p->up; tmp1 != p; tmp1 = tmp1->up) {
        for (tmp2 = tmp1->left; tmp2 != tmp1; tmp2 = tmp2->left) {
            tmp2->down->up = tmp2;
            tmp2->up->down = tmp2;
        }
    }
    p->right->left = p;
    p->left->right = p;
}

int search(struct node *h, int i, struct node *solution[])
{
    if (h->right == h)
        return 1;

    struct node *c = h->right;
    cover(c);
    struct node *r, *tmp;
    for (r = c->down; r != c; r = r->down) {
        solution[i] = r;
        for (tmp = r->right; tmp != r; tmp = tmp->right) {
            cover(tmp->ch);
        }
        if (search(h, i+1, solution) == 1)
            return 1;
        struct node *tmp4;
        for (tmp4 = r->left; tmp4 != r; tmp4 = tmp4->left) {
            uncover(tmp4->ch);
        }
    }
    uncover(c);
    return -1;

}


void delete_rhead(struct node *h)
{
    struct node *p;
    for (p = h->down; p != h; p = p->down) {
        p->left->right = p->right;
        p->right->left = p->left;
    }
}

int main()
{
    struct node *h = init(6, 7);
    struct node **solution = (struct node **) malloc(6 * sizeof(struct node *));
    int j;
    for (j = 0; j < 6; j++) {
        solution[j] = NULL;
    }

    insert(h, 0, 0);
    insert(h, 0, 3);
    insert(h, 0, 6);
    insert(h, 1, 0);
    insert(h, 1, 3);
    insert(h, 2, 3);
    insert(h, 2, 4);
    insert(h, 2, 6);
    insert(h, 3, 2);
    insert(h, 3, 4);
    insert(h, 3, 5);
    insert(h, 4, 1);
    insert(h, 4, 2);
    insert(h, 4, 5);
    insert(h, 4, 6);
    insert(h, 5, 1);
    insert(h, 5, 6);

    delete_rhead(h);
    if(    search(h, 0, solution))
        printf("hello\n");
    int i;
    for (i = 0; solution[i] != NULL; i++)
        printf("%d\n", solution[i]->r);
    return 0;
}
