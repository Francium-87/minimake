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

void str_split (char* line, char delimiter, char* part1, char* part2)
{
    // get the position of the delimiter
    int i = 0;
    while (line[i] != delimiter)
        i++;
    
    // alloc the appropriate size
    part1 = (char*)malloc(sizeof(char) * i);
    part2 = (char*)malloc(sizeof(char) * (strlen(line) - i));

    for(int x = 0; x < strlen(line); x++)
    {
        // put everything before the delimiter and store it in part1
        if (x < i)
        {
            part1[x] = line[x];
        }
        // put everything after the delimiter and store it in part2
        else if (x > i)
        {
            part2[x-i] = line[x];
        }
    }
    part1[i] = '\0';
    part2[strlen(line) - i] = '\0';
}

int main (int argc, char** argv)
{
    char* line = "target:dependencies";
    char delimiter = ':';
    char* part1 = NULL;
    char* part2 = NULL;
    str_split(line,delimiter,part1,part2);
    
    printf("line: %s\n\
    delimiter:  %c\n\
    part 1: %s\n\
    part 2: %s\n", line, delimiter, part1, part2);
    
    FILE* f = fopen(argv[1], "r");

    if (f == NULL)
    {
        fprintf(stderr, "[Minimake] Main: Failed to open file %s\n", argv[1]);
        return(EXIT_FAILURE);
    }
    return 0;
}