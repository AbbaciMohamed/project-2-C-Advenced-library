#ifndef QUEUELIST_H
#define QUEUELIST_H

#include <stdio.h>
#include <stdbool.h>

// ======== NODE AND STRUCT DEFINITIONS ========

typedef struct TNode {
    char *word;
    char *syn_or_ant;
    int num_chars;
    int num_vowels;
    struct TNode *prev, *next;
} TNode;

typedef struct {
    TNode *head, *tail;
    int size;
} TList;

typedef struct QNode {
    char *word;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front, *rear;
    int size;
} TQueue;

// ======== LIST OPERATIONS ========

// Load words from file
TList *getSynWords(FILE *f);
TList *getAntoWords(FILE *f);

// Info retrieval
void getInfWord(TList *syn, TList *ant, const char *word);
void getInfWord2(TList *syn, TList *ant, const char *inf);

// Sorting
TList *sortWord(TList *syn);      // Alphabetical
TList *sortWord2(TList *syn);     // By character count
TList *sortWord3(TList *syn);     // By vowel count (descending)

// Core operations
TList *addWord(FILE *f, TList *syn, TList *ant, const char *word, const char *syne, const char *anton);
TList *deleteWord(FILE *f, TList *syn, TList *ant, const char *word);
TList *updateWord(FILE *f, TList *syn, TList *ant, const char *word, const char *syne, const char *anton);

// Search and filtering
TList *similarWord(TList *syn, const char *word, double rate);
TList *countWord(TList *syn, const char *prt);
TList *palindromWord(TList *syn);

// Merging
TList *merge(TList *syn, TList *ant);     // Doubly linked
TList *merge2(TList *syn, TList *ant);    // Circular linked

// ======== QUEUE OPERATIONS ========

TQueue *syllable(TList *syn);             // Filter by syllables (vowels)
TQueue *pronounciation(TList *syn);       // Dummy grouping by length/diphthong
TQueue *toQueue(TList *merged);           // Convert merged TList to queue

// ======== TEST FUNCTION ========
// For use in stack.c
TQueue *init_queue(void);
QNode *create_qnode(const char *w);
TList *init_list(void);
TNode *create_node(const char *word, const char *sa);

void test_TListQueue(void);

#endif

