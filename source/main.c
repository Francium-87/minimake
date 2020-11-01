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

void str_split (char* line, int pos, char* part1, char* part2)
{
    for(int x = 0; x < strlen(line); x++)
    {
        // put everything before the delimiter and store it in part1
        if (x < pos)
        {
            part1[x] = line[x];
        }
        // put everything after the delimiter and store it in part2
        else if (x > pos)
        {
            part2[x-pos] = line[x];
        }
    }
    part1[pos] = '\0';
    part2[strlen(line) - pos] = '\0';
}

static char* get_delimiter (char* line) 
{
    for(int i = 0; i < strlen(line); i++)
    {
        if (line[i] == ':')
            return line[i];
        else if (line[i] == '=')
            return line[i];
    }
    // TODO RETURN QLQC POUR INDIQUER QUE C'EST UNE RECIPE
    return '\0';
}

int main (int argc, char** argv)
{   
    /* str_split usage
    char* line = "target:dependencies";
    char delimiter = ':';
    
    // get the position of the delimiter
    int i = 0;
    while (line[i] != delimiter)
        i++;
    
    // alloc the appropriate size
    char* part1 = (char*)malloc(sizeof(char) * i);
    char* part2 = (char*)malloc(sizeof(char) * (strlen(line) - i));

    str_split(line,i ,part1,part2);
    
    printf("line: %s\n\
    delimiter:  %c\n\
    part 1: %s\n\
    part 2: %s\n", line, delimiter, part1, part2);
    */
    FILE* f = fopen(argv[1], "r");

    if (f == NULL)
    {
        fprintf(stderr, "[Minimake] Main: Failed to open file %s\n", argv[1]);
        return(EXIT_FAILURE);
    }

    size_t* len = 0;
    char** line = NULL;

    while(geline(*line, len, f) != -1)
    {
        if (line[0] != "\n"){
            char* delimiter = get_delimiter(line);
            // TODO TRAITEMENT EN FONCTION DU RETOUR DE L'APPEL A GET DELIMITER
            if (delimiter == ':')
            {
                // TODO PARSING D'UNE TARGET
            }
            else if (delimiter == '='){
                // TODO PARSING D'UNE VARIABLE
            }
            else
            {
                // TODO ON A AFFAIRE À UNE RECIPE
            }
        }
    }
    return 0;
}