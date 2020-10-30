#include <stdio.h>
#include <string.h>
#include <malloc.h>

struct rule {
    char* target;
    char* dependencies;
    char* receipe;
};

struct var {
    char* label;
    char* value;
};

struct node {
    struct rule r;
    struct var v;
    struct node* next;
};

int main (int argc, char** argv){
    return 0;
}