#include "../include/bst.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- Helpers --- */
static TreeNode *create_tnode(const char *w, const char *syn, const char *ant) {
    TreeNode *n = malloc(sizeof *n);
    n->word = strdup(w);
    n->syn = strdup(syn);
    n->ant = strdup(ant);
    n->num_chars = strlen(w);
    n->num_vowels = count_vowels(w);
    n->left = n->right = NULL;
    return n;
}

static TTree *init_tree(void) {
    TTree *tr = malloc(sizeof *tr);
    tr->root = NULL;
    tr->size = 0;
    return tr;
}

/* --- Core Functions --- */
TTree *AddWordBST(TTree *tr, const char *w, const char *syn, const char *ant) {
    TreeNode **cur = &tr->root;
    while (*cur) {
        int cmp = strcmp(w, (*cur)->word);
        if (cmp == 0) {
            free((*cur)->syn);
            free((*cur)->ant);
            (*cur)->syn = strdup(syn);
            (*cur)->ant = strdup(ant);
            return tr;
        }
        cur = (cmp < 0) ? &(*cur)->left : &(*cur)->right;
    }
    *cur = create_tnode(w, syn, ant);
    tr->size++;
    return tr;
}

TreeNode *getInfWordTree(TTree *tr, const char *w) {
    TreeNode *cur = tr->root;
    while (cur) {
        int cmp = strcmp(w, cur->word);
        if (cmp == 0) return cur;
        cur = (cmp < 0) ? cur->left : cur->right;
    }
    return NULL;
}

static TreeNode *min_node(TreeNode *n) {
    while (n && n->left) n = n->left;
    return n;
}

static TreeNode *delete_node(TreeNode *n, const char *w, int *deleted) {
    if (!n) return NULL;
    int cmp = strcmp(w, n->word);
    if (cmp < 0) n->left = delete_node(n->left, w, deleted);
    else if (cmp > 0) n->right = delete_node(n->right, w, deleted);
    else {
        *deleted = 1;
        if (!n->left || !n->right) {
            TreeNode *tmp = n->left ? n->left : n->right;
            free(n->word); free(n->syn); free(n->ant); free(n);
            return tmp;
        } else {
            TreeNode *succ = min_node(n->right);
            free(n->word); free(n->syn); free(n->ant);
            n->word = strdup(succ->word);
            n->syn = strdup(succ->syn);
            n->ant = strdup(succ->ant);
            n->right = delete_node(n->right, succ->word, deleted);
        }
    }
    return n;
}

TTree *deleteWordBST(TTree *tr, const char *w) {
    int deleted = 0;
    tr->root = delete_node(tr->root, w, &deleted);
    if (deleted) tr->size--;
    return tr;
}

TTree *UpdateWordBST(TTree *tr, const char *w, const char *s, const char *a) {
    TreeNode *p = getInfWordTree(tr, w);
    if (p) {
        free(p->syn); free(p->ant);
        p->syn = strdup(s);
        p->ant = strdup(a);
        p->num_vowels = count_vowels(p->word);
    }
    return tr;
}

/* --- Traversals --- */
void TraversalBSTinOrder(TTree *tr, void (*visit)(TreeNode*)) {
    void dfs(TreeNode *n) {
        if (!n) return;
        dfs(n->left);
        visit(n);
        dfs(n->right);
    }
    dfs(tr->root);
}

void TraversalBSTpreOrder(TTree *tr, void (*visit)(TreeNode*)) {
    void dfs(TreeNode *n) {
        if (!n) return;
        visit(n);
        dfs(n->left);
        dfs(n->right);
    }
    dfs(tr->root);
}

void TraversalBSTpostOrder(TTree *tr, void (*visit)(TreeNode*)) {
    void dfs(TreeNode *n) {
        if (!n) return;
        dfs(n->left);
        dfs(n->right);
        visit(n);
    }
    dfs(tr->root);
}

/* --- Utilities --- */
void HighSizeBST(TTree *tr) {
    int height(TreeNode *n) {
        if (!n) return 0;
        int lh = height(n->left), rh = height(n->right);
        return (lh > rh ? lh : rh) + 1;
    }
    printf("Size: %d | Height: %d\n", tr->size, height(tr->root));
}

bool isBalancedBST(TTree *tr) {
    int dfs(TreeNode *n, int *h) {
        if (!n) return 1;
        int lh = 0, rh = 0;
        if (!dfs(n->left, &lh) || !dfs(n->right, &rh)) return 0;
        *h = (lh > rh ? lh : rh) + 1;
        return abs(lh - rh) <= 1;
    }
    int h;
    return dfs(tr->root, &h);
}

TreeNode *LowestCommonAncestor(TTree *tr, const char *a, const char *b) {
    TreeNode *cur = tr->root;
    while (cur) {
        if (strcmp(a, cur->word) < 0 && strcmp(b, cur->word) < 0)
            cur = cur->left;
        else if (strcmp(a, cur->word) > 0 && strcmp(b, cur->word) > 0)
            cur = cur->right;
        else return cur;
    }
    return NULL;
}

int CountNodesRanges(TTree *tr, int low, int high) {
    int count(TreeNode *n) {
        if (!n) return 0;
        int inRange = (n->num_chars >= low && n->num_chars <= high) ? 1 : 0;
        return inRange + count(n->left) + count(n->right);
    }
    return count(tr->root);
}

TreeNode *inOrderSuccessor(TTree *tr, const char *word) {
    TreeNode *cur = tr->root, *succ = NULL;
    while (cur) {
        int cmp = strcmp(word, cur->word);
        if (cmp < 0) {
            succ = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return succ;
}

TTree *BSTMirror(TTree *tr) {
    TreeNode *mirror(TreeNode *n) {
        if (!n) return NULL;
        TreeNode *tmp = n->left;
        n->left = mirror(n->right);
        n->right = mirror(tmp);
        return n;
    }
    tr->root = mirror(tr->root);
    return tr;
}

/* --- Merge --- */
static void inorder_collect(TreeNode *node, TreeNode **arr, int *index) {
    if (!node) return;
    inorder_collect(node->left, arr, index);
    arr[(*index)++] = node;
    inorder_collect(node->right, arr, index);
}

static TreeNode *build_balanced(TreeNode **arr, int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    TreeNode *root = arr[mid];
    root->left = build_balanced(arr, start, mid - 1);
    root->right = build_balanced(arr, mid + 1, end);
    return root;
}

TTree *BSTMerge(TTree *tr1, TTree *tr2) {
    int n1 = tr1->size, n2 = tr2->size;
    TreeNode **arr1 = malloc(n1 * sizeof(TreeNode *));
    TreeNode **arr2 = malloc(n2 * sizeof(TreeNode *));
    int i1 = 0, i2 = 0;

    inorder_collect(tr1->root, arr1, &i1);
    inorder_collect(tr2->root, arr2, &i2);

    TreeNode **merged = malloc((n1 + n2) * sizeof(TreeNode *));
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2)
        merged[k++] = strcmp(arr1[i]->word, arr2[j]->word) < 0 ? arr1[i++] : arr2[j++];
    while (i < n1) merged[k++] = arr1[i++];
    while (j < n2) merged[k++] = arr2[j++];

    TTree *newTree = init_tree();
    newTree->root = build_balanced(merged, 0, k - 1);
    newTree->size = k;

    free(arr1); free(arr2); free(merged);
    return newTree;
}

/* --- Fill Tree from File --- */
TTree *fillTree(FILE *f) {
    if (!f) return NULL;
    TTree *tr = init_tree();
    char w[100], s[100], a[100];
    while (fscanf(f, "%s %s %s", w, s, a) == 3) {
        AddWordBST(tr, w, s, a);
    }
    rewind(f);
    return tr;
}

/* --- Debug Printer --- */
void print_node(TreeNode *n) {
    if (!n) return;
    printf("Word: %-12s Syn: %-12s Ant: %-12s | Chars: %2d Vowels: %d\n",
           n->word, n->syn, n->ant, n->num_chars, n->num_vowels);
}

void test_BST() {
    FILE *f = fopen("data/words.txt", "r");
    if (!f) {
        printf("❌ Could not open data/words.txt\n");
        return;
    }

    TTree *tree = fillTree(f);
    fclose(f);

    int choice;
    char word[100], syn[100], ant[100], word2[100];

    do {
        printf("\n====== BST MODULE MENU ======\n");
        printf("1. Add Word\n");
        printf("2. Delete Word\n");
        printf("3. Update Word\n");
        printf("4. Display Word Info\n");
        printf("5. InOrder Traversal\n");
        printf("6. PreOrder Traversal\n");
        printf("7. PostOrder Traversal\n");
        printf("8. Tree Size & Height\n");
        printf("9. Check Balance\n");
        printf("10. Lowest Common Ancestor\n");
        printf("11. Count Nodes in Length Range\n");
        printf("12. InOrder Successor\n");
        printf("13. Mirror Tree\n");
        printf("14. Merge with Another Tree\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice); getchar();

        switch (choice) {
            case 1:
                printf("Enter word, synonym, antonym: ");
                scanf("%s %s %s", word, syn, ant);
                tree = AddWordBST(tree, word, syn, ant);
                printf("✅ Word added.\n");
                break;

            case 2:
                printf("Enter word to delete: ");
                scanf("%s", word);
                tree = deleteWordBST(tree, word);
                printf("✅ Word deleted (if existed).\n");
                break;

            case 3:
                printf("Enter word to update, new synonym and antonym: ");
                scanf("%s %s %s", word, syn, ant);
                tree = UpdateWordBST(tree, word, syn, ant);
                printf("✅ Word updated.\n");
                break;

            case 4:
                printf("Enter word to display: ");
                scanf("%s", word);
                {
                    TreeNode *n = getInfWordTree(tree, word);
                    if (n) print_node(n);
                    else printf("⚠️ Word not found.\n");
                }
                break;

            case 5:
                printf("🌳 InOrder Traversal:\n");
                TraversalBSTinOrder(tree, print_node);
                break;

            case 6:
                printf("🌳 PreOrder Traversal:\n");
                TraversalBSTpreOrder(tree, print_node);
                break;

            case 7:
                printf("🌳 PostOrder Traversal:\n");
                TraversalBSTpostOrder(tree, print_node);
                break;

            case 8:
                HighSizeBST(tree);
                break;

            case 9:
                printf("Tree is %sbalanced.\n", isBalancedBST(tree) ? "" : "not ");
                break;

            case 10:
                printf("Enter two words: ");
                scanf("%s %s", word, word2);
                {
                    TreeNode *ancestor = LowestCommonAncestor(tree, word, word2);
                    if (ancestor) {
                        printf("Lowest Common Ancestor: %s\n", ancestor->word);
                    } else {
                        printf("⚠️ No common ancestor found.\n");
                    }
                }
                break;

            case 11:
                {
                    int low, high;
                    printf("Enter low and high range of word length: ");
                    scanf("%d %d", &low, &high);
                    int count = CountNodesRanges(tree, low, high);
                    printf("✅ Nodes in range [%d-%d]: %d\n", low, high, count);
                }
                break;

            case 12:
                printf("Enter word to find inOrder successor: ");
                scanf("%s", word);
                {
                    TreeNode *succ = inOrderSuccessor(tree, word);
                    if (succ) print_node(succ);
                    else printf("⚠️ No successor found.\n");
                }
                break;

            case 13:
                tree = BSTMirror(tree);
                printf("✅ Tree mirrored.\n");
                break;

            case 14: {
                FILE *f2 = fopen("data/words.txt", "r");
                if (!f2) {
                    printf("⚠️ Failed to open file again for second tree.\n");
                    break;
                }
                TTree *tree2 = fillTree(f2);
                fclose(f2);
                tree = BSTMerge(tree, tree2);
                printf("✅ Trees merged into balanced tree.\n");
                break;
            }

            case 0:
                printf("🚪 Exiting BST test.\n");
                break;

            default:
                printf("❌ Invalid choice.\n");
        }
    } while (choice != 0);
}

