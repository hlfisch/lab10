#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie {
    struct Trie *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct Trie *current = pTrie;
    int index;
    for (int i = 0; word[i] != '\0'; i++) {
        index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = (struct Trie *)malloc(sizeof(struct Trie));
            if (current->children[index] == NULL) {
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            current->children[index]->isEndOfWord = 0;
            current->children[index]->count = 0;
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                current->children[index]->children[j] = NULL;
            }
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    current->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word) {
    struct Trie *current = pTrie;
    int index;
    for (int i = 0; word[i] != '\0'; i++) {
        index = word[i] - 'a';
        if (!current->children[index]) {
            return 0;
        }
        current = current->children[index];
    }
    return (current != NULL && current->isEndOfWord) ? current->count : 0;
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *pTrie) {
    if (!pTrie)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pTrie->children[i]) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node) {
        node->isEndOfWord = 0;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[count] = strdup(word);
        count++;
    }

    fclose(file);
    return count;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }
    deallocateTrie(pTrie);
    return 0;
}
