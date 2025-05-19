#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdbool.h>
#include "stack.h"  // Needed for TStack reference in toTree()

// ------------------------------
// Tree Node Structure
// ------------------------------
typedef struct TreeNode {
    char *word;
    char *syn;
    char *ant;
    int num_chars;
    int num_vowels;
    struct TreeNode *left, *right;
} TreeNode;

// ------------------------------
// Tree Structure
// ------------------------------
typedef struct {
    TreeNode *root;
    int size;
} TTree;

// ------------------------------
// Public BST Functions
// ------------------------------

// Create and fill tree from file or stack
TTree *fillTree(FILE *f);
TTree *toTree(TStack *stk);

// Core operations
TTree *AddWordBST(TTree *tr, const char *word, const char *syne, const char *anton);
TTree *deleteWordBST(TTree *tr, const char *word);
TTree *UpdateWordBST(TTree *tr, const char *word, const char *syne, const char *anton);

// Lookup
TreeNode *getInfWordTree(TTree *tr, const char *word);

// Traversals
void TraversalBSTinOrder(TTree *tr, void (*visit)(TreeNode *));
void TraversalBSTpreOrder(TTree *tr, void (*visit)(TreeNode *));
void TraversalBSTpostOrder(TTree *tr, void (*visit)(TreeNode *));

// Structural analysis
void HighSizeBST(TTree *tr);
bool isBalancedBST(TTree *tr);
TreeNode *LowestCommonAncestor(TTree *tr, const char *w1, const char *w2);
int CountNodesRanges(TTree *tr, int low, int high);
TreeNode *inOrderSuccessor(TTree *tr, const char *word);
TTree *BSTMirror(TTree *tr);

// Merge two BSTs into a balanced one
TTree *BSTMerge(TTree *tr1, TTree *tr2);

// Testing
void test_BST(void);

#endif

