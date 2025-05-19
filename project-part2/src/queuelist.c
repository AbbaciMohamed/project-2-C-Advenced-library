#include "../include/queuelist.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 256

// --- Utility Functions ---

static int count_vowels(const char *s) {
    int count = 0;
    while (*s) {
        char c = tolower(*s);
        if (strchr("aeiou", c)) count++;
        s++;
    }
    return count;
}

static int is_palindrome(const char *s) {
    size_t i = 0, j = strlen(s);
    if (j == 0) return 1;
    j--;
    while (i < j) {
        if (tolower(s[i]) != tolower(s[j])) return 0;
        i++; j--;
    }
    return 1;
}

TList *init_list(void) {
    TList *L = malloc(sizeof *L);
    L->head = L->tail = NULL;
    L->size = 0;
    return L;
}

TNode *create_node(const char *w, const char *sa) {
    TNode *n = malloc(sizeof *n);
    n->word = strdup(w);
    n->syn_or_ant = strdup(sa);
    n->num_chars = strlen(w);
    n->num_vowels = count_vowels(w);
    n->prev = n->next = NULL;
    return n;
}

// --- File Parsers ---

TList *getSynWords(FILE *f) {
    TList *L = init_list();
    char word[100], syn[100], anton[100];
    while (fscanf(f, "%s %s %s", word, syn, anton) == 3) {
        TNode *n = create_node(word, syn);
        if (!L->head) L->head = L->tail = n;
        else { L->tail->next = n; n->prev = L->tail; L->tail = n; }
        L->size++;
    }
    rewind(f);
    return L;
}

TList *getAntoWords(FILE *f) {
    TList *L = init_list();
    char word[100], syn[100], anton[100];
    while (fscanf(f, "%s %s %s", word, syn, anton) == 3) {
        TNode *n = create_node(word, anton);
        if (!L->head) L->head = L->tail = n;
        else { L->tail->next = n; n->prev = L->tail; L->tail = n; }
        L->size++;
    }
    rewind(f);
    return L;
}

// --- Display Info Functions ---

void getInfWord(TList *syn, TList *ant, const char *word) {
    TNode *p;
    for (p = syn->head; p; p = p->next) {
        if (strcmp(p->word, word) == 0) {
            printf("Word: %s | Synonym: %s | Chars: %d | Vowels: %d\n",
                   p->word, p->syn_or_ant, p->num_chars, p->num_vowels);
        }
    }
    for (p = ant->head; p; p = p->next) {
        if (strcmp(p->word, word) == 0) {
            printf("Word: %s | Antonym: %s | Chars: %d | Vowels: %d\n",
                   p->word, p->syn_or_ant, p->num_chars, p->num_vowels);
        }
    }
}

void getInfWord2(TList *syn, TList *ant, const char *inf) {
    TNode *p;
    for (p = syn->head; p; p = p->next) {
        if (strcmp(p->syn_or_ant, inf) == 0) {
            printf("Synonym: %s -> Word: %s | Chars: %d | Vowels: %d\n",
                   inf, p->word, p->num_chars, p->num_vowels);
        }
    }
    for (p = ant->head; p; p = p->next) {
        if (strcmp(p->syn_or_ant, inf) == 0) {
            printf("Antonym: %s -> Word: %s | Chars: %d | Vowels: %d\n",
                   inf, p->word, p->num_chars, p->num_vowels);
        }
    }
}

// --- Sorting Helpers ---

static void swap_nodes(TNode *a, TNode *b) {
    char *tmp_word = a->word; a->word = b->word; b->word = tmp_word;
    char *tmp_s = a->syn_or_ant; a->syn_or_ant = b->syn_or_ant; b->syn_or_ant = tmp_s;
    int c = a->num_chars; a->num_chars = b->num_chars; b->num_chars = c;
    int v = a->num_vowels; a->num_vowels = b->num_vowels; b->num_vowels = v;
}

TList *sortWord(TList *syn) {
    int swapped;
    do {
        swapped = 0;
        for (TNode *p = syn->head; p && p->next; p = p->next) {
            if (strcmp(p->word, p->next->word) > 0) {
                swap_nodes(p, p->next);
                swapped = 1;
            }
        }
    } while (swapped);
    return syn;
}

TList *sortWord2(TList *syn) {
    int swapped;
    do {
        swapped = 0;
        for (TNode *p = syn->head; p && p->next; p = p->next) {
            if (p->num_chars > p->next->num_chars) {
                swap_nodes(p, p->next);
                swapped = 1;
            }
        }
    } while (swapped);
    return syn;
}

TList *sortWord3(TList *syn) {
    int swapped;
    do {
        swapped = 0;
        for (TNode *p = syn->head; p && p->next; p = p->next) {
            if (p->num_vowels < p->next->num_vowels) {
                swap_nodes(p, p->next);
                swapped = 1;
            }
        }
    } while (swapped);
    return syn;
}

// --- Core Add/Update/Delete ---

TList *addWord(FILE *f, TList *syn, TList *ant,
               const char *word, const char *syne, const char *anton) {
    fprintf(f, "%s %s %s\n", word, syne, anton);
    TNode *n1 = create_node(word, syne), *n2 = create_node(word, anton);

    if (!syn->head) syn->head = syn->tail = n1;
    else { syn->tail->next = n1; n1->prev = syn->tail; syn->tail = n1; }

    if (!ant->head) ant->head = ant->tail = n2;
    else { ant->tail->next = n2; n2->prev = ant->tail; ant->tail = n2; }

    syn->size++; ant->size++;
    return syn;
}

TList *deleteWord(FILE *f, TList *syn, TList *ant, const char *word) {
    if (!f || !syn || !ant || !word) return syn;

    // Update file contents: rewrite without the deleted word
    FILE *tmp = fopen("data/temp.txt", "w");
    if (!tmp) {
        perror("Error creating temporary file");
        return syn;
    }

    char w[100], s[100], a[100];
    rewind(f);
    while (fscanf(f, "%s %s %s", w, s, a) == 3) {
        if (strcmp(w, word) != 0)
            fprintf(tmp, "%s %s %s\n", w, s, a);
    }
    fclose(f);
    fclose(tmp);
    remove("data/words.txt");
    rename("data/temp.txt", "data/words.txt");

    // Remove from synonym list
    for (TNode *p = syn->head; p;) {
        TNode *next = p->next;
        if (strcmp(p->word, word) == 0) {
            if (p->prev) p->prev->next = p->next;
            if (p->next) p->next->prev = p->prev;
            if (p == syn->head) syn->head = p->next;
            if (p == syn->tail) syn->tail = p->prev;
            free(p->word); free(p->syn_or_ant); free(p);
            syn->size--;
        }
        p = next;
    }

    // Remove from antonym list
    for (TNode *p = ant->head; p;) {
        TNode *next = p->next;
        if (strcmp(p->word, word) == 0) {
            if (p->prev) p->prev->next = p->next;
            if (p->next) p->next->prev = p->prev;
            if (p == ant->head) ant->head = p->next;
            if (p == ant->tail) ant->tail = p->prev;
            free(p->word); free(p->syn_or_ant); free(p);
            ant->size--;
        }
        p = next;
    }

    return syn;
}


TList *updateWord(FILE *f, TList *syn, TList *ant,
                  const char *word, const char *syne, const char *anton) {
    deleteWord(f, syn, ant, word);
    f = fopen("data/words.txt", "a");
    addWord(f, syn, ant, word, syne, anton);
    fclose(f);
    return syn;
}

TList *similarWord(TList *syn, const char *word, double rate) {
    TList *out = init_list();
    for (TNode *p = syn->head; p; p = p->next) {
        int match = 0;
        for (size_t i = 0; i < strlen(word) && i < strlen(p->word); i++) {
            if (word[i] == p->word[i]) match++;
        }
        double match_rate = (double)match / strlen(word);
        if (match_rate >= rate) {
            TNode *n = create_node(p->word, p->syn_or_ant);
            if (!out->head) out->head = out->tail = n;
            else { out->tail->next = n; n->prev = out->tail; out->tail = n; }
            out->size++;
        }
    }
    return out;
}

TList *countWord(TList *syn, const char *prt) {
    TList *out = init_list();
    for (TNode *p = syn->head; p; p = p->next) {
        if (strstr(p->word, prt)) {
            TNode *n = create_node(p->word, p->syn_or_ant);
            if (!out->head) out->head = out->tail = n;
            else {
                out->tail->next = n;
                n->prev = out->tail;
                out->tail = n;
            }
            out->size++;
        }
    }
    return out;
}

TList *palindromWord(TList *syn) {
    TList *out = init_list();
    for (TNode *p = syn->head; p; p = p->next) {
        if (is_palindrome(p->word)) {
            TNode *n = create_node(p->word, p->syn_or_ant);
            if (!out->head) {
                out->head = out->tail = n;
            } else {
                TNode *curr = out->head;
                while (curr && strcmp(n->word, curr->word) > 0)
                    curr = curr->next;
                if (!curr) {
                    out->tail->next = n;
                    n->prev = out->tail;
                    out->tail = n;
                } else if (curr == out->head) {
                    n->next = out->head;
                    out->head->prev = n;
                    out->head = n;
                } else {
                    n->next = curr;
                    n->prev = curr->prev;
                    curr->prev->next = n;
                    curr->prev = n;
                }
            }
            out->size++;
        }
    }
    return out;
}

TList *merge(TList *syn, TList *ant) {
    TList *out = init_list();
    TNode *p1 = syn->head, *p2 = ant->head;
    while (p1 && p2) {
        TNode *n1 = create_node(p1->word, p1->syn_or_ant);
        TNode *n2 = create_node(p2->word, p2->syn_or_ant);

        if (!out->head) {
            out->head = n1;
            n1->next = n2; n2->prev = n1;
            out->tail = n2;
        } else {
            out->tail->next = n1;
            n1->prev = out->tail;
            n1->next = n2;
            n2->prev = n1;
            out->tail = n2;
        }

        out->size += 2;
        p1 = p1->next;
        p2 = p2->next;
    }
    return out;
}

TList *merge2(TList *syn, TList *ant) {
    TList *c = merge(syn, ant);
    if (c->tail && c->head) {
        c->tail->next = c->head;
        c->head->prev = c->tail;
    }
    return c;
}

TQueue *init_queue(void) {
    TQueue *Q = malloc(sizeof *Q);
    Q->front = Q->rear = NULL;
    Q->size = 0;
    return Q;
}

QNode *create_qnode(const char *w) {
    QNode *n = malloc(sizeof *n);
    n->word = strdup(w);
    n->next = NULL;
    return n;
}

TQueue *syllable(TList *syn) {
    TQueue *Q = init_queue();
    for (TNode *p = syn->head; p; p = p->next) {
        int syllables = count_vowels(p->word); // proxy
        if (syllables >= 2) {
            QNode *n = create_qnode(p->word);
            if (!Q->front) Q->front = Q->rear = n;
            else { Q->rear->next = n; Q->rear = n; }
            Q->size++;
        }
    }
    return Q;
}

TQueue *pronounciation(TList *syn) {
    TQueue *Q = init_queue();
    for (TNode *p = syn->head; p; p = p->next) {
        QNode *n = create_qnode(p->word);
        if (!Q->front) Q->front = Q->rear = n;
        else { Q->rear->next = n; Q->rear = n; }
        Q->size++;
    }
    return Q;
}

TQueue *toQueue(TList *merged) {
    TQueue *Q = init_queue();
    for (TNode *p = merged->head; p; p = p->next) {
        QNode *n = create_qnode(p->word);
        if (!Q->front) Q->front = Q->rear = n;
        else { Q->rear->next = n; Q->rear = n; }
        Q->size++;
    }
    return Q;
}

void test_TListQueue() {
    FILE *f = fopen("data/words.txt", "r+");
    if (!f) {
        printf("❌ Error: Cannot open data/words.txt\n");
        return;
    }

    TList *syn = getSynWords(f);
    TList *ant = getAntoWords(f);

    printf("\n--- Loaded Synonyms (%d words) ---\n", syn->size);
    for (TNode *p = syn->head; p; p = p->next)
        printf("Word: %-12s  Synonym: %-12s  [Chars: %2d, Vowels: %d]\n",
               p->word, p->syn_or_ant, p->num_chars, p->num_vowels);

    printf("\n--- Loaded Antonyms (%d words) ---\n", ant->size);
    for (TNode *p = ant->head; p; p = p->next)
        printf("Word: %-12s  Antonym: %-12s  [Chars: %2d, Vowels: %d]\n",
               p->word, p->syn_or_ant, p->num_chars, p->num_vowels);

    int choice;
    char word[100], synon[100], anton[100], input[100];
    double rate;

    do {
        printf("\n📘 === Queue & List Test Menu ===\n");
        printf("1.  Get Word Info\n");
        printf("2.  Get Info by Syn/Ant\n");
        printf("3.  Sort Alphabetically\n");
        printf("4.  Sort by Char Count\n");
        printf("5.  Sort by Vowel Count\n");
        printf("6.  Add Word\n");
        printf("7.  Delete Word\n");
        printf("8.  Update Word\n");
        printf("9.  Similarity Search\n");
        printf("10. Substring Match Count\n");
        printf("11. Palindrome Words\n");
        printf("12. Merge Lists (Doubly)\n");
        printf("13. Merge Lists (Circular)\n");
        printf("14. Convert Merge to Queue\n");
        printf("15. Queue by Syllables\n");
        printf("16. Queue by Pronunciation\n");
        printf("0.  Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter word: ");
                scanf("%s", input);
                getInfWord(syn, ant, input);
                break;

            case 2:
                printf("Enter synonym or antonym: ");
                scanf("%s", input);
                getInfWord2(syn, ant, input);
                break;

            case 3:
                sortWord(syn);
                printf("✅ Sorted Alphabetically:\n");
                for (TNode *p = syn->head; p; p = p->next)
                    printf("- %s\n", p->word);
                break;

            case 4:
                sortWord2(syn);
                printf("✅ Sorted by Char Count:\n");
                for (TNode *p = syn->head; p; p = p->next)
                    printf("- %s (%d chars)\n", p->word, p->num_chars);
                break;

            case 5:
                sortWord3(syn);
                printf("✅ Sorted by Vowel Count:\n");
                for (TNode *p = syn->head; p; p = p->next)
                    printf("- %s (%d vowels)\n", p->word, p->num_vowels);
                break;

            case 6:
                printf("Enter word synonym antonym: ");
                scanf("%s %s %s", word, synon, anton);
                addWord(f, syn, ant, word, synon, anton);
                printf("✅ Word added and saved.\n");
                break;

            case 7:
                printf("Enter word to delete: ");
                scanf("%s", word);
                deleteWord(f, syn, ant, word);
                printf("🗑️ Word deleted from file.\n");
                break;

            case 8:
                printf("Enter word new_synonym new_antonym: ");
                scanf("%s %s %s", word, synon, anton);
                updateWord(f, syn, ant, word, synon, anton);
                printf("♻️ Word updated.\n");
                break;

            case 9:
                printf("Enter word and match rate (0.0 - 1.0): ");
                scanf("%s %lf", word, &rate);
                {
                    TList *sim = similarWord(syn, word, rate);
                    printf("🧮 Similar Words:\n");
                    for (TNode *p = sim->head; p; p = p->next)
                        printf("- %s\n", p->word);
                }
                break;

            case 10:
                printf("Enter substring to match: ");
                scanf("%s", input);
                {
                    TList *matches = countWord(syn, input);
                    printf("🔎 Matches:\n");
                    for (TNode *p = matches->head; p; p = p->next)
                        printf("- %s\n", p->word);
                }
                break;

            case 11:
                {
                    TList *pal = palindromWord(syn);
                    printf("🔁 Palindrome Words:\n");
                    for (TNode *p = pal->head; p; p = p->next)
                        printf("- %s\n", p->word);
                }
                break;

            case 12:
                {
                    TList *m = merge(syn, ant);
                    printf("🔗 Merged (Doubly Linked):\n");
                    for (TNode *p = m->head; p; p = p->next)
                        printf("- %s\n", p->word);
                }
                break;

            case 13:
                {
                    TList *c = merge2(syn, ant);
                    printf("🔄 Circular Merged:\n");
                    TNode *p = c->head;
                    for (int i = 0; i < c->size && i < 10; i++, p = p->next)
                        printf("- %s\n", p->word);
                }
                break;

            case 14:
                {
                    TList *m = merge(syn, ant);
                    TQueue *q = toQueue(m);
                    printf("📦 Queue from Merge:\n");
                    for (QNode *n = q->front; n; n = n->next)
                        printf("- %s\n", n->word);
                }
                break;

            case 15:
                {
                    TQueue *q = syllable(syn);
                    printf("🗣️ Queue by Syllable:\n");
                    for (QNode *n = q->front; n; n = n->next)
                        printf("- %s\n", n->word);
                }
                break;

            case 16:
                {
                    TQueue *q = pronounciation(syn);
                    printf("🔊 Queue by Pronunciation:\n");
                    for (QNode *n = q->front; n; n = n->next)
                        printf("- %s\n", n->word);
                }
                break;

            case 0:
                printf("👋 Exiting Queue/List Tests.\n");
                break;

            default:
                printf("❌ Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    fclose(f);
}
