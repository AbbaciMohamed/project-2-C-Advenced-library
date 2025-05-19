#include "queuelist.h"
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/* ---------------- Structures ---------------- */

// Stack Node
typedef struct SNode {
    char *word;
    char *syn;
    char *ant;
    int num_chars;
    int num_vowels;
    struct SNode *next;
} SNode;

// Stack Structure
typedef struct TStack {
    SNode *top;
    int size;
} TStack;

// Queue Node (for conversions)

// List Node (for conversions)

/* ---------------- Core Stack Operations ---------------- */

TStack *addWordStack(TStack *stk, const char *word, const char *syne, const char *anton);
TStack *deleteWordStack(TStack *stk, const char *word);
TStack *updateWordStack(TStack *stk, const char *word, const char *syne, const char *anton);
TStack *getInfWordStack(TStack *stk, const char *word);
TStack *sortWordStack(TStack *stk);
TStack *syllableStack(TStack *stk);
void    pronounciationStack(TStack *stk);
char   *getSmallest(TStack *stk);
void    cycleSearch(TStack *stk);
bool    isPalyndromeStack(const char *word);
TStack *StackRev(TStack *stk);

/* ---------------- Conversions ---------------- */

TQueue *stackToQueue(TStack *stk);
TList  *StacktoList(TStack *stk);
TStack *toStack(TList *merged);

/* ---------------- Test Suite ---------------- */

void test_stack(void);

#endif // STACK_H

