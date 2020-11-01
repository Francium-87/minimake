#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct rule
{
    char *target;
    char *dependencies;
    char *recipe;
};

struct var
{
    char *label;
    char *value;
};

struct node
{
    struct rule *r;
    struct var *v;
    struct node *next;
    struct node *prev;
};

void str_split(const char *line, int pos, char *part1, char *part2)
{
    for (int x = 0; x < strlen(line); x++)
    {
        // put everything before the delimiter and store it in part1
        if (x < pos)
        {
            part1[x] = line[x];
        }
        // put everything after the delimiter and store it in part2
        else if (x > pos)
        {
            part2[x - pos] = line[x];
        }
    }
    part1[pos] = '\0';
    part2[strlen(line) - pos] = '\0';
}

static char get_delimiter(char *line)
{
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == ':')
            return line[i];
        else if (line[i] == '=')
            return line[i];
    }
    // TODO RETURN QLQC POUR INDIQUER QUE C'EST UNE RECIPE
    return '\0';
}

void* parse_line (const char* line, const char delimiter)
{
    int i = 0;
    while (line[i] != delimiter)
        i++;
    
    char *part1 = malloc(sizeof(char) * i);
    char *part2 = malloc(sizeof(char) * strlen(line) - i);
    str_split(line, i, part1, part2);

    if (delimiter == ':')
    {
        struct rule *ret = malloc(sizeof(struct rule));
        ret->target = part1;
        ret->dependencies = part2;
        ret->recipe = NULL;
        return ret;
    }
    else
    {
        struct var *ret = malloc(sizeof(struct var));
        ret->label = part1;
        ret->value = part2;
        return ret;
    }
}

int main(int argc, char *argv[])
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
    FILE *f = fopen(argv[1], "r");

    if (f == NULL)
    {
        fprintf(stderr, "(ERROR)[Minimake] Main: Failed to open file %s\n", argv[1]);
        return (EXIT_FAILURE);
    }

    size_t len = 0;
    char *line = NULL;
    struct node *prev_node = malloc(sizeof(struct node));
    while (getline(&line, &len, f) != -1)
    {
        if (line[0] != '\n')
        {
            struct node *new_node = malloc(sizeof(struct node));
            new_node->r = NULL;
            new_node->v = NULL;

            char delimiter = get_delimiter(line);
            printf("(INFO)[Minimake] Main: \"%s\"\n", line);
            if (delimiter == ':')
            {
                printf("(DEBUG)[Minimake] Main: parsing \"%s\" as rule\n", line);
                struct rule *new_rule = parse_line(line, delimiter);
                new_node->r = new_rule;
            }
            else if (delimiter == '=')
            {
                printf("(DEBUG)[Minimake] Main: parsing \"%s\" as variable\n", line);
                struct var *new_var = parse_line(line, delimiter);
                new_node->v = new_var;
            }
            else
            {
                printf("(DEBUG)[Minimake] Main: parsing \"%s\" as recipe\n", line);
                // ON PREND LA NODE D'AVANT ET ON MODIFIE SA RULE POUR Y AJOUTER LA RECIPE
                if (!prev_node->r)
                {
                    fprintf(strerror, "(ERROR)[Minimake] Syntaxe error: no target and dependencies for \"%s\"", line);
                    return EXIT_FAILURE;
                }
                prev_node->r->recipe = line;
            }
            // ON LINK LE NODE RECU AVEC LE RESTE
            new_node->prev = prev_node;
            prev_node->next = new_node;
            prev_node = new_node;
        }
    }
    printf("(DEBUG)[Minimake]: EOF\n");
    return 0;
}