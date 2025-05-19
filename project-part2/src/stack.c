#include "../include/queuelist.h"
#include "../include/stack.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* ---------------- Utility Functions ---------------- */
int count_vowels(const char *s) {
    int count = 0;
    while (*s) {
        char c = tolower(*s);
        if (strchr("aeiou", c)) count++;
        s++;
    }
    return count;
}

bool isPalyndromeStack(const char *word) {
    size_t n = strlen(word);
    for (size_t i = 0; i < n / 2; i++) {
        if (tolower(word[i]) != tolower(word[n - 1 - i]))
            return false;
    }
    return true;
}

/* ---------------- Stack Core ---------------- */
static TStack *init_stack(void) {
    TStack *s = malloc(sizeof(TStack));
    s->top = NULL;
    s->size = 0;
    return s;
}

static SNode *create_snode(const char *w, const char *syn, const char *ant) {
    SNode *n = malloc(sizeof(SNode));
    n->word = strdup(w);
    n->syn = strdup(syn);
    n->ant = strdup(ant);
    n->num_chars = strlen(w);
    n->num_vowels = count_vowels(w);
    n->next = NULL;
    return n;
}

TStack *toStack(TList *merged) {
    TStack *s = init_stack();
    for (TNode *p = merged->head; p; p = p->next) {
        SNode *n = create_snode(p->word, p->syn_or_ant, ""); // No antonym in merged
        n->next = s->top;
        s->top = n;
        s->size++;
    }
    return s;
}

/* ---------------- Stack Features ---------------- */
TStack *addWordStack(TStack *stk, const char *word, const char *syne, const char *anton) {
    SNode *n = create_snode(word, syne, anton);
    n->next = stk->top;
    stk->top = n;
    stk->size++;
    return sortWordStack(stk);
}

TStack *deleteWordStack(TStack *stk, const char *word) {
    TStack *temp = init_stack();
    while (stk->top) {
        SNode *n = stk->top;
        stk->top = n->next;
        if (strcmp(n->word, word) != 0) {
            n->next = temp->top;
            temp->top = n;
            temp->size++;
        } else {
            free(n->word); free(n->syn); free(n->ant); free(n);
        }
    }
    while (temp->top) {
        SNode *n = temp->top;
        temp->top = n->next;
        n->next = stk->top;
        stk->top = n;
    }
    free(temp);
    return stk;
}

TStack *updateWordStack(TStack *stk, const char *word, const char *syne, const char *anton) {
    for (SNode *p = stk->top; p; p = p->next) {
        if (strcmp(p->word, word) == 0) {
            free(p->syn); free(p->ant);
            p->syn = strdup(syne);
            p->ant = strdup(anton);
            p->num_vowels = count_vowels(p->word);
            return stk;
        }
    }
    return stk;
}

TStack *getInfWordStack(TStack *stk, const char *word) {
    for (SNode *p = stk->top; p; p = p->next) {
        if (strcmp(p->word, word) == 0) {
            printf("Word: %s | Syn: %s | Ant: %s | Len: %d | Vowels: %d\n",
                   p->word, p->syn, p->ant, p->num_chars, p->num_vowels);
            return stk;
        }
    }
    printf("%s not found.\n", word);
    return stk;
}

TStack *sortWordStack(TStack *stk) {
    bool swapped;
    do {
        swapped = false;
        for (SNode *p = stk->top; p && p->next; p = p->next) {
            if (strcmp(p->word, p->next->word) > 0) {
                char *t;
                t = p->word; p->word = p->next->word; p->next->word = t;
                t = p->syn;  p->syn  = p->next->syn;  p->next->syn  = t;
                t = p->ant;  p->ant  = p->next->ant;  p->next->ant  = t;
                int i;
                i = p->num_chars;  p->num_chars  = p->next->num_chars;  p->next->num_chars = i;
                i = p->num_vowels; p->num_vowels = p->next->num_vowels; p->next->num_vowels = i;
                swapped = true;
            }
        }
    } while (swapped);
    return stk;
}

TStack *syllableStack(TStack *stk) {
    bool swapped;
    do {
        swapped = false;
        for (SNode *p = stk->top; p && p->next; p = p->next) {
            if (p->num_vowels > p->next->num_vowels) {
                char *t;
                t = p->word; p->word = p->next->word; p->next->word = t;
                t = p->syn;  p->syn  = p->next->syn;  p->next->syn  = t;
                t = p->ant;  p->ant  = p->next->ant;  p->next->ant  = t;
                int i;
                i = p->num_chars;  p->num_chars  = p->next->num_chars;  p->next->num_chars = i;
                i = p->num_vowels; p->num_vowels = p->next->num_vowels; p->next->num_vowels = i;
                swapped = true;
            }
        }
    } while (swapped);
    return stk;
}

void pronounciationStack(TStack *stk) {
    printf("\n--- Pronunciation Groups ---\n");

    printf("Short:\n");
    for (SNode *p = stk->top; p; p = p->next) {
        if (p->num_chars < 5)
            printf("  %s\n", p->word);
    }

    printf("Long:\n");
    for (SNode *p = stk->top; p; p = p->next) {
        if (p->num_chars >= 5 && p->num_chars <= 8)
            printf("  %s\n", p->word);
    }

    printf("Diphthong-like:\n");
    for (SNode *p = stk->top; p; p = p->next) {
        if (p->num_chars > 8 || strstr(p->word, "ou") || strstr(p->word, "ai") || strstr(p->word, "ei"))
            printf("  %s\n", p->word);
    }
}


char *getSmallest(TStack *stk) {
    if (!stk || !stk->top) return NULL;
    char *min = stk->top->word;
    for (SNode *p = stk->top; p; p = p->next)
        if (strcmp(p->word, min) < 0) min = p->word;
    return min;
}

void cycleSearch(TStack *stk) {
    for (SNode *p = stk->top; p; p = p->next) {
        for (SNode *q = stk->top; q; q = q->next) {
            if (strcmp(p->syn, q->word) == 0)
                printf("Cycle: %s -> %s\n", p->word, q->word);
        }
    }
}

TStack *StackRev(TStack *stk) {
    if (!stk || !stk->top) return stk;
    SNode *head = stk->top;
    stk->top = head->next;
    TStack *rev = StackRev(stk);
    head->next = NULL;
    if (!rev->top) rev->top = head;
    else {
        SNode *p = rev->top;
        while (p->next) p = p->next;
        p->next = head;
    }
    return rev;
}

TQueue *stackToQueue(TStack *stk) {
    TQueue *Q = init_queue();
    for (SNode *p = stk->top; p; p = p->next) {
        QNode *n = create_qnode(p->word);
        if (!Q->front) Q->front = Q->rear = n;
        else { Q->rear->next = n; Q->rear = n; }
        Q->size++;
    }
    return Q;
}

TList *StacktoList(TStack *stk) {
    TList *L = init_list();
    for (SNode *p = stk->top; p; p = p->next) {
        TNode *n = create_node(p->word, p->syn);
        if (!L->head) L->head = L->tail = n;
        else { L->tail->next = n; n->prev = L->tail; L->tail = n; }
        L->size++;
    }
    return L;
}

void test_stack() {
    TStack *stack = NULL;
    char word[100], syn[100], ant[100];
    int choice;

    do {
        printf("\n========= STACK MODULE =========\n");
        printf("1. Add Word\n");
        printf("2. Delete Word\n");
        printf("3. Update Word\n");
        printf("4. Display Info by Word\n");
        printf("5. Sort Alphabetically\n");
        printf("6. Sort by Syllables (vowel count)\n");
        printf("7. Check Palindrome\n");
        printf("8. Get Smallest Word\n");
        printf("9. Detect Cycles\n");
        printf("10. Reverse Stack Recursively\n");
        printf("11. Pronunciation Categories\n");
        printf("12. Convert to List & Display\n");
        printf("13. Convert to Queue & Display\n");
        printf("0. Exit Stack Module\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter word, synonym, antonym: ");
                scanf("%s %s %s", word, syn, ant);
                stack = addWordStack(stack, word, syn, ant);
                printf("✅ Added and sorted.\n");
                break;

            case 2:
                printf("Enter word to delete: ");
                scanf("%s", word);
                stack = deleteWordStack(stack, word);
                printf("✅ Deleted if found.\n");
                break;

            case 3:
                printf("Enter word to update and new synonym & antonym: ");
                scanf("%s %s %s", word, syn, ant);
                stack = updateWordStack(stack, word, syn, ant);
                printf("✅ Updated if found.\n");
                break;

            case 4:
                printf("Enter word to view info: ");
                scanf("%s", word);
                getInfWordStack(stack, word);
                break;

            case 5:
                stack = sortWordStack(stack);
                printf("✅ Stack sorted alphabetically.\n");
                break;

            case 6:
                stack = syllableStack(stack);
                printf("✅ Sorted by vowel count.\n");
                break;

            case 7:
                printf("Enter word to check palindrome: ");
                scanf("%s", word);
                if (isPalyndromeStack(word))
                    printf("✅ It is a palindrome.\n");
                else
                    printf("❌ Not a palindrome.\n");
                break;

            case 8:
                {
                    char *min = getSmallest(stack);
                    if (min)
                        printf("🔍 Smallest word alphabetically: %s\n", min);
                    else
                        printf("⚠️ Stack is empty.\n");
                }
                break;

            case 9:
                printf("🔁 Detecting cycles...\n");
                cycleSearch(stack);
                break;

            case 10:
                stack = StackRev(stack);
                printf("🔄 Stack reversed.\n");
                break;

            case 11:
                printf("📢 Pronunciation groups:\n");
                pronounciationStack(stack); // print grouping
                break;

            case 12: {
                TList *list = StacktoList(stack);
                printf("📋 Converted Stack -> List:\n");
                for (TNode *p = list->head; p; p = p->next)
                    printf("  %s (%s)\n", p->word, p->syn_or_ant);
                break;
            }

            case 13: {
                TQueue *Q = stackToQueue(stack);
                printf("📦 Converted Stack -> Queue:\n");
                for (QNode *p = Q->front; p; p = p->next)
                    printf("  %s\n", p->word);
                break;
            }

            case 0:
                printf("🔚 Exiting Stack Module.\n");
                break;

            default:
                printf("❌ Invalid choice.\n");
        }

    } while (choice != 0);
}


