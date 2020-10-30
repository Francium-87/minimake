#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct rule 
{
    char* target;
    char* dependencies;
    char* receipe;
};

struct var 
{
    char* label;
    char* value;
};

struct node 
{
    struct rule r;
    struct var v;
    struct node* next;
};

int main (int argc, char** argv)
{
    FILE* f = fopen(argv[1], 'r');

    if(f == NULL)
    {
        fprintf(stderr, "[Minimake] Main: Failed to open file %s\n", argv[1]);
        return(EXIT_FAILURE);
    }


    return 0;
}