#ifndef RECURSION_H
#define RECURSION_H

#include <stdio.h>
#include <stdbool.h>

// --- File Recursion Helpers ---
int countWordOccurrence(FILE *f, const char *word);
FILE *removeWordOccurrence(FILE *f, const char *word);
FILE *replaceWordOccurrence(FILE *f, const char *word, const char *rep);

// --- String Recursion Helpers ---
void wordPermutation(char *word);
void subseqWord(const char *word);
int longestSubseqWord(const char *s1, const char *s2);
int distinctSubseqWord(const char *word);
bool isPalindromWord(const char *word);

// --- Test Entry ---
void test_recursion();

#endif

