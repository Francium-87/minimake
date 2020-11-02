#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rule {
  char *target;
  char *dependencies;
  char *recipe;
};

struct var {
  char *label;
  char *value;
};

struct node {
  struct rule *r;
  struct var *v;
  struct node *next;
  struct node *prev;
};

static const void node_pretty_print(const struct node *n) {
  if (n->r) {
    printf("Target: \"%s\"\n", n->r->target);
    printf("Dependencies: \"%s\"\n", n->r->dependencies);
    printf("Rule: \"%s\"\n", n->r->recipe);
  } else if (n->v) {
    printf("Label: \"%s\"\n", n->v->label);
    printf("Value: \"%s\"\n", n->v->value);
  }
}

static char get_delimiter(const char *line) {
  for (int i = 0; i < strlen(line); i++) {
    if (line[i] == ':')
      return line[i];
    else if (line[i] == '=')
      return line[i];
  }

  return '\0';
}

static int is_valid_var(const char *line) {
  for (size_t i = 0; i < (strlen(line)); i++) {
    switch (line[i]) {
    case ':':
      return -1;
      break;
    case '=':
      return -1;
      break;
    case '#':
      return -1;
      break;
    case ' ':
      return -1;
      break;
    default:
      break;
    }
  }
  return 0;
}

void str_split(const char *line, int pos, char *part1, char *part2) {
  for (int x = 0; x < strlen(line); x++) {
    // put everything before the delimiter and store it in part1
    if (x < pos) {
      part1[x] = line[x];
    }
    // put everything after the delimiter and store it in part2
    else if (x > pos) {
      part2[x - (pos + 1)] = line[x];
    }
  }

  part1[pos] = '\0';
  part2[strlen(line) - (pos + 1)] = '\0';
}

void *parse_line(const char *line, const char delimiter) {
  int i = 0;
  while (line[i] != delimiter)
    i++;

  char *part1 = malloc(sizeof(char) * i);
  char *part2 = malloc(sizeof(char) * strlen(line) - i);
  str_split(line, i, part1, part2);

  if (delimiter == ':') {
    struct rule *ret = malloc(sizeof(struct rule));
    ret->target = part1;
    ret->dependencies = part2;
    ret->recipe = NULL;
    return ret;
  } else {
    struct var *ret = malloc(sizeof(struct var));
    ret->label = part1;
    ret->value = part2;
    return ret;
  }
}

int get_new_node(const char *line, struct node *prev_node,
                 struct node *new_node) {

  new_node->next = NULL;
  new_node->prev = NULL;
  new_node->v = NULL;
  new_node->r = NULL;

  char delimiter = get_delimiter(line);
  if (delimiter == ':') {
    printf("(DEBUG)[Minimake] Main: parsing \"%s\" as rule\n", line);
    struct rule *new_rule = parse_line(line, delimiter);
    new_node->r = new_rule;
    return 0;
  } else if (delimiter == '=') {
    printf("(DEBUG)[Minimake] Main: parsing \"%s\" as variable\n", line);

    if (!is_valid_var(line)) {
      fprintf(stderr, "(ERROR)[Minimake] Main: Unexpected character in "
                      "variable declaration \"%s\"\n",
              line);
      return EXIT_FAILURE;
    }

    struct var *new_var = parse_line(line, delimiter);
    new_node->v = new_var;
    return 0;
  } else if (line[0] == '\t') {
    printf("(DEBUG)[Minimake] Main: parsing \"%s\" as recipe\n", line);

    if (!prev_node->r) {
      fprintf(stderr, "(ERROR)[Minimake] Syntaxe error: no target and "
                      "dependencies for \"%s\"",
              line);
      return EXIT_FAILURE;
    }
    prev_node->r->recipe = strdup(line);
    return 0;
  }
}

int main(int argc, char *argv[]) {
  FILE *f = fopen(argv[1], "r");

  if (f == NULL) {
    fprintf(stderr, "(ERROR)[Minimake] Main: Failed to open file %s\n",
            argv[1]);
    return (EXIT_FAILURE);
  }

  size_t len = 0;
  char *line = NULL;
  struct node *prev_node = malloc(sizeof(struct node));
  prev_node->next = NULL;
  prev_node->prev = NULL;
  prev_node->r = NULL;
  prev_node->v = NULL;
  while (getline(&line, &len, f) != -1) {
    if (line[0] != '\n') {
      struct node *new_node = malloc(sizeof(struct node));
      get_new_node(line, prev_node, new_node);
      new_node->prev = prev_node;
      prev_node->next = new_node;
      prev_node = new_node;
    }
  }
  printf("(DEBUG)[Minimake]: EOF\n");
  node_pretty_print(prev_node);
  while (!prev_node->prev) {
    node_pretty_print(prev_node);
    prev_node = prev_node->prev;
  }

  return 0;
}