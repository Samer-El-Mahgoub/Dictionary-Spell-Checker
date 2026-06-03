#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define MAX 100

struct avlnode {
    char data[MAX];
    int h;
    struct avlnode *left, *right;
};
typedef struct avlnode AVLNODE;

AVLNODE *avl_root = NULL;

int mymax(int a, int b) { return (a > b) ? a : b; }

int geth(AVLNODE *n) {
    if(n == NULL) return 0;
    return n->h;
}

AVLNODE* new_avl_node(char w[]) {
    AVLNODE *p = (AVLNODE*)malloc(sizeof(AVLNODE));
    strcpy(p->data, w);
    p->left = p->right = NULL;
    p->h = 1;
    return p;
}

AVLNODE* rot_right(AVLNODE *y) {
    AVLNODE *x  = y->left;
    AVLNODE *t2 = x->right;
    x->right = y;
    y->left  = t2;
    y->h = mymax(geth(y->left), geth(y->right)) + 1;
    x->h = mymax(geth(x->left), geth(x->right)) + 1;
    return x;
}

AVLNODE* rot_left(AVLNODE *x) {
    AVLNODE *y  = x->right;
    AVLNODE *t2 = y->left;
    y->left  = x;
    x->right = t2;
    x->h = mymax(geth(x->left), geth(x->right)) + 1;
    y->h = mymax(geth(y->left), geth(y->right)) + 1;
    return y;
}

int get_balance(AVLNODE *n) {
    if(n == NULL) return 0;
    return geth(n->left) - geth(n->right);
}

AVLNODE* insert_avl(AVLNODE *node, char w[]) {
    if(node == NULL) return new_avl_node(w);

    if(strcasecmp(w, node->data) < 0)
        node->left = insert_avl(node->left, w);
    else if(strcasecmp(w, node->data) > 0)
        node->right = insert_avl(node->right, w);
    else
        return node;

    node->h = 1 + mymax(geth(node->left), geth(node->right));

    int bal = get_balance(node);

    if(bal > 1 && strcasecmp(w, node->left->data) < 0)
        return rot_right(node);
    if(bal < -1 && strcasecmp(w, node->right->data) > 0)
        return rot_left(node);
    if(bal > 1 && strcasecmp(w, node->left->data) > 0) {
        node->left = rot_left(node->left);
        return rot_right(node);
    }
    if(bal < -1 && strcasecmp(w, node->right->data) < 0) {
        node->right = rot_right(node->right);
        return rot_left(node);
    }

    return node;
}

AVLNODE* search_avl(AVLNODE *root, char w[], AVLNODE **last) {
    if(root == NULL) return NULL;

    int c = strcasecmp(w, root->data);
    if(c == 0) return root;

    *last = root;

    if(c < 0) return search_avl(root->left, w, last);
    else       return search_avl(root->right, w, last);
}

AVLNODE* pred_avl(AVLNODE *root, char w[]) {
    AVLNODE *res = NULL;
    AVLNODE *curr = root;
    while(curr != NULL) {
        if(strcasecmp(curr->data, w) < 0) {
            res = curr;
            curr = curr->right;
        } else {
            curr = curr->left;
        }
    }
    return res;
}

AVLNODE* succ_avl(AVLNODE *root, char w[]) {
    AVLNODE *res = NULL;
    AVLNODE *curr = root;
    while(curr != NULL) {
        if(strcasecmp(curr->data, w) > 0) {
            res = curr;
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return res;
}

int height_avl(AVLNODE *root) {
    if(root == NULL) return -1;
    return root->h - 1;
}

int count_avl(AVLNODE *root) {
    if(root == NULL) return 0;
    return 1 + count_avl(root->left) + count_avl(root->right);
}

void make_lower(char str[]) {
    int i = 0;
    while(str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }
}

void clean_word(char src[], char dest[]) {
    int i = 0, j = 0;
    while(src[i] != '\0') {
        if(isalpha(src[i])) {
            dest[j] = tolower(src[i]);
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

void check_sentence(char line[])
{
    char orig[MAX];
    char cleaned[MAX];

    char *token = strtok(line, " \t\r\n");
    while(token != NULL)
    {
        strcpy(orig, token);
        clean_word(token, cleaned);

        if(strlen(cleaned) > 0)
        {
            AVLNODE *last = NULL;
            AVLNODE *found = search_avl(avl_root, cleaned, &last);

            if(found != NULL)
            {
                printf("%s - CORRECT\n", orig);
            }
            else
            {
                printf("%s - Incorrect, Suggestions : ", orig);
                if(last != NULL)
                {
                    printf("%s", last->data);

                    AVLNODE *pr = pred_avl(avl_root, last->data);
                    if(pr != NULL) printf(" %s", pr->data);

                    AVLNODE *sc = succ_avl(avl_root, last->data);
                    if(sc != NULL) printf(" %s", sc->data);
                }
                printf("\n");
            }
        }
        token = strtok(NULL, " \t\r\n");
    }
}

int main()
{
    FILE *fp;
    char word[MAX];
    char line[1000];

    fp = fopen("Dictionary.txt", "r");
    if(fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    printf("Dictionary Loaded Successfully...!\n");
    printf("................................\n");

    while(fscanf(fp, "%s", word) == 1)
    {
        make_lower(word);
        avl_root = insert_avl(avl_root, word);
    }
    fclose(fp);

    printf("Size = %d\n", count_avl(avl_root));
    printf("................................\n");
    printf("Height = %d\n", height_avl(avl_root));
    printf("................................\n");

    printf("Enter a sentence :\n");
    fgets(line, 1000, stdin);

    check_sentence(line);

    return 0;
}
