#include "../include/recursion.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/* --- File Recursion Helpers --- */

// Counts occurrences of a word recursively
int countWordOccurrence(FILE *f, const char *word) {
    char w[256];
    if (fscanf(f, "%255s", w) != 1) return 0;
    int count = (strcmp(w, word) == 0);
    return count + countWordOccurrence(f, word);
}

// Internal helper to recursively remove a word
static void removeRecursive(FILE *in, FILE *out, const char *word) {
    char w[256];
    if (fscanf(in, "%255s", w) != 1) return;
    if (strcmp(w, word) != 0) fprintf(out, "%s ", w);
    removeRecursive(in, out, word);
}

// Public API to remove all occurrences of word
FILE *removeWordOccurrence(FILE *f, const char *word) {
    rewind(f);
    FILE *tmp = tmpfile();
    if (!tmp) return NULL;
    removeRecursive(f, tmp, word);
    rewind(tmp);
    return tmp;
}

// Internal helper to recursively replace a word
static void replaceRecursive(FILE *in, FILE *out, const char *word, const char *rep) {
    char w[256];
    if (fscanf(in, "%255s", w) != 1) return;
    if (strcmp(w, word) == 0)
        fprintf(out, "%s ", rep);
    else
        fprintf(out, "%s ", w);
    replaceRecursive(in, out, word, rep);
}

// Public API to replace all occurrences of word with rep
FILE *replaceWordOccurrence(FILE *f, const char *word, const char *rep) {
    rewind(f);
    FILE *tmp = tmpfile();
    if (!tmp) return NULL;
    replaceRecursive(f, tmp, word, rep);
    rewind(tmp);
    return tmp;
}

/* --- String Recursion Helpers --- */

// Print all permutations of a string
void wordPermutation(char *word) {
    size_t n = strlen(word);
    void permute(char *s, int l, int r) {
        if (l == r) {
            printf("%s\n", s);
            return;
        }
        for (int i = l; i <= r; i++) {
            char t = s[l]; s[l] = s[i]; s[i] = t;
            permute(s, l + 1, r);
            t = s[l]; s[l] = s[i]; s[i] = t;  // backtrack
        }
    }
    permute(word, 0, n - 1);
}

// Print all subsequences of a string
void subseqWord(const char *word) {
    if (!word || !*word) {
        printf("(empty)\n");
        return;
    }

    size_t n = strlen(word);
    char *buffer = malloc(n + 1);
    if (!buffer) {
        perror("Failed to allocate memory");
        return;
    }

    // Inner recursive helper function
    void subseq(size_t i, size_t j) {
        if (i == n) {
            buffer[j] = '\0';
            printf("%s\n", buffer);
            return;
        }

        // Include current character
        buffer[j] = word[i];
        subseq(i + 1, j + 1);

        // Exclude current character
        subseq(i + 1, j);
    }

    printf("All subsequences of \"%s\":\n", word);
    subseq(0, 0);

    free(buffer);
}

// Return length of longest common subsequence between s1 and s2
int longestSubseqWord(const char *s1, const char *s2) {
    if (!*s1 || !*s2) return 0;
    if (*s1 == *s2) return 1 + longestSubseqWord(s1 + 1, s2 + 1);
    int a = longestSubseqWord(s1 + 1, s2);
    int b = longestSubseqWord(s1, s2 + 1);
    return (a > b ? a : b);
}

// Count number of distinct subsequences (optimized)
int distinctSubseqWord(const char *word) {
    if (!*word) return 1;
    int count = distinctSubseqWord(word + 1) * 2;
    const char *ch = strchr(word + 1, *word);
    if (ch) count -= distinctSubseqWord(ch + 1);
    return count;
}

// Check if a word is a palindrome using recursion
bool isPalindromWord(const char *word) {
    size_t n = strlen(word);
    if (n <= 1) return true;
    if (tolower(word[0]) != tolower(word[n - 1])) return false;
    char *sub = strndup(word + 1, n - 2);
    if (!sub) return false;
    bool res = isPalindromWord(sub);
    free(sub);
    return res;
}

#include "../include/recursion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_recursion() {
    int choice;
    char word[100], word2[100], rep[100];
    FILE *f;

    do {
        printf("\n--- Recursion Menu ---\n");
        printf("1. Count word occurrence in file\n");
        printf("2. Remove word occurrence from file\n");
        printf("3. Replace word in file\n");
        printf("4. Word permutations\n");
        printf("5. Word subsequences\n");
        printf("6. Longest common subsequence\n");
        printf("7. Distinct subsequences\n");
        printf("8. Is palindrome (recursively)\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter word to count: ");
                scanf("%s", word);
                f = fopen("data/words.txt", "r");
                if (!f) { printf("Cannot open file.\n"); break; }
                printf("Occurrences of '%s': %d\n", word, countWordOccurrence(f, word));
                fclose(f);
                break;

            case 2:
                printf("Enter word to remove: ");
                scanf("%s", word);
                f = fopen("data/words.txt", "r");
                if (!f) { printf("Cannot open file.\n"); break; }
                FILE *newF = removeWordOccurrence(f, word);
                printf("Updated file content:\n");
                char temp[256];
                while (fscanf(newF, "%s", temp) == 1)
                    printf("%s ", temp);
                printf("\n");
                fclose(f);
                fclose(newF);
                break;

            case 3:
                printf("Enter word to replace: ");
                scanf("%s", word);
                printf("Enter replacement: ");
                scanf("%s", rep);
                f = fopen("data/words.txt", "r");
                if (!f) { printf("Cannot open file.\n"); break; }
                FILE *repl = replaceWordOccurrence(f, word, rep);
                printf("Replaced file content:\n");
                while (fscanf(repl, "%s", temp) == 1)
                    printf("%s ", temp);
                printf("\n");
                fclose(f);
                fclose(repl);
                break;

            case 4:
                printf("Enter word for permutations: ");
                scanf("%s", word);
                printf("Permutations:\n");
                wordPermutation(word);
                break;

            case 5:
                printf("Enter word for subsequences: ");
                scanf("%s", word);
                printf("Subsequences:\n");
                subseqWord(word);
                break;

            case 6:
                printf("Enter first string: ");
                scanf("%s", word);
                printf("Enter second string: ");
                scanf("%s", word2);
                printf("Longest common subsequence length: %d\n", longestSubseqWord(word, word2));
                break;

            case 7:
                printf("Enter word: ");
                scanf("%s", word);
                printf("Number of distinct subsequences: %d\n", distinctSubseqWord(word));
                break;

            case 8:
                printf("Enter word: ");
                scanf("%s", word);
                printf("Is '%s' a palindrome? %s\n", word,
                       isPalindromWord(word) ? "Yes" : "No");
                break;

            case 0:
                printf("Exiting Recursion Test...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

