// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Global variable to keep track of the size of the dictionary
unsigned int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char temp[LENGTH + 1];
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        temp[i] = tolower(word[i]);
    }
    temp[len] = '\0';

    unsigned int index = hash(temp);
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, temp) == 0)
        {
            return true;
        }
    }
    return false;
}
// Size Function
unsigned int size(void)
{
    unsigned int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (node *cursor = table[i]; cursor != NULL; cursor = cursor->next)
        {
            count++;
        }
    }
    return count;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // hash function accounting for the first two letters of the word
    unsigned int hash = 0;
    for (int i = 0; i < 2 && word[i] != '\0'; i++)
    {
        hash = (hash << 3) ^ toupper(word[i]);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }
        strcpy(new_node->word, word);

        // Hash word to obtain a hash value
        unsigned int index = hash(word);

        // Insert node into hash table
        new_node->next = table[index];
        table[index] = new_node;

        // Increment word count
        word_count++;
    }

    fclose(file);
    return true;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
