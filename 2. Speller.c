// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>

#include "dictionary.h"

void hash_table_insert(int hash, int array_start, int array_end);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 50000;

// Hash table
node *table[N];

// Total number of words in dictionary
int total_words;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word to obtain a hash value
    int hash_value = hash(word);

    // Set up a cursor to traverse the linked list at the hash vaue in hash table
    node *cursor = table[hash_value];

    while (cursor != NULL)
    {
        // Check if word matches in current element
        if ((strcasecmp(word, cursor->word)) == 0)
        {
            return true;
        }

        else
        {
            cursor = cursor->next;
        }
    }

    return false;

}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // PJW hash function - code source by Arash Partow
    // Based on work by Peter J. Weinberger of Renaissance Technologies
    // As well as the book Compilers (Principles, Techniques and Tools) by Aho, Sethi and Ulman
    // Can be found on: http://www.partow.net/programming/hashfunctions/#AvailableHashFunctions

    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i    = 0;
    int length = strlen(word);

    for (i = 0; i < length; ++word, ++i)
    {
        // Define individual character in word
        char character = *word;

        // Convert all upper case characters to lower case
        if (isupper(*word) != 0)
        {
            character = tolower(*word);
        }

        // Hash function
        hash = (hash << OneEighth) + (character);

        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    // Reduce large hash values to less than 100,000
    if (hash > 99999)
    {
        hash = (sqrt(hash)) * 1;
    }

    // Reduce any hash values bewteen 50,000 and 100,000
    // To fit in N number of buckets
    if (hash > 50000)
    {
        hash  = hash / 2;
    }

    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *p_dictionary = fopen(dictionary, "r");

    // Check for NULL
    if (dictionary == NULL)
    {
        printf("Dictionary cannot be read\n");
        return false;
    }

    // Set total number of words in dictionary to 0
    total_words = 0;

    // Define word in dictionary
    char dictionary_word[LENGTH + 1];

    while (fscanf(p_dictionary, "%s", dictionary_word) != EOF)
    {
        // Allocate memory for a new node
        node *element = malloc(sizeof(node));

        // Check for NULL and return false
        if (element == NULL)
        {
            printf("Element memory allocation failure\n");
            return false;
        }

        // Enter dictionary word into current node
        strcpy(element->word, dictionary_word);

        // Temporarily set pointer to NULL
        element->next = NULL;

        // Hash word using hash function - to obtain a hash value
        int hash_value = hash(dictionary_word);

        // Insert node into linked list in the hash table at the location given by the hash function

        // For first nodes in linked list - pointer points to NULL
        if (table[hash_value] == NULL)
        {
            // Point bucket head i - to current element
            table[hash_value] = element;
        }

        // For all other nodes - add element in start of linked list
        else
        {
            // 1. Enter pointer to next node (second node in linked list)
            element->next = table[hash_value];

            // 2. Reset head (first element in linked list) to point to new node
            table[hash_value] = element;
        }

        total_words++;
    }

    fclose(p_dictionary);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through buckets
    for (int i = 0; i < N; i++)
    {
        // Check for linked list in bucket i
        if (table[i] != NULL)
        {
            // Set cursor to traverse through linked list
            node *cursor = table[i];

            // Set a temporary cursor for deleting nodes
            node *temp;

            while (cursor != NULL)
            {
                // Point temporary cursor to original cursor
                temp = cursor;

                // Move original cursor to next element
                cursor = cursor->next;

                // Free memory of current element
                free(temp);
            }
            // End of linked list
        }
    }

    return true;
}
