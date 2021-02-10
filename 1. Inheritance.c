// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

// Define root of trie
person *root = NULL;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Allocate memory for root
    root = malloc(sizeof(person));
    if (root == NULL)
    {
        printf("Memory allocation fault\n");
        return 1;
    }

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);

    // Free root node
    free(root);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    // TODO: Allocate memory for new person
    person *p = malloc(sizeof(person));

    if (p == NULL)
    {
        printf("Memory allocation fault\n");
        return p;
    }

    // For generations with parent data
    if (generations > 1)
    {
        // TODO: Recursively create blood type histories for parents

        // Recursively create previous generations by updating person's parents
        for (int i = 0; i < 2; i++)
        {
            p->parents[i] = create_family(generations - 1);
        }

        // TODO: Randomly assign child alleles based on parents

        // Assign allele from each parent
        for (int j = 0; j < 2; j++)
        {
            // Assign random allele from parent
            int r = rand() % 2;
            if (r == 0)
            {
                // Assign parent's allele 0
                p->alleles[j] = p->parents[j]->alleles[0];
            }

            else
            {
                // Assign parent's allele 1
                p->alleles[j] = p->parents[j]->alleles[1];
            }
        }
    }

    // For oldest generation without parent data
    else
    {
        // TODO: Set parent pointers to NULL
        for (int i = 0; i < 2; i++)
        {
            p->parents[i] = NULL;
        }

        // TODO: Randomly assign alleles
        for (int j = 0; j < 2; j++)
        {
            p->alleles[j] = random_allele();
        }
    }

    // TODO: Return newly created person
    return p;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{

    // TODO: Handle base case

    // If person has no parents - free memory
    if (p->parents[0] == NULL && p->parents[1] == NULL)
    {
        free(p);
        return;
    }

    // TODO: Free parents

    // Recursively free both of the person’s parents
    for (int i = 0; i < 2; i++)
    {
        free_family(p->parents[i]);
    }

    // TODO: Free child

    // When youngest generation is found - free memory
    if (GENERATIONS == 3)
    {
        free(p);
        return;
    }
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
