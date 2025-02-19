/*
Agris Pudāns
st. apl. nr. ap08426

PD3 -- Direktoriju koka apstaigāšana
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


// Structure for a node in a tree
typedef struct Node {
  char *name;
  struct Node *children;
  struct Node *next;
} Node; 

// Function to create a new node
Node *createNode(char *name) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->name = (char *)malloc(strlen(name) + 1);
  strcpy(node->name, name);
  node->children = NULL;
  node->next = NULL;
  return node;
}

// Function to add a child to a node
void addChild(Node *parent, Node *child) {
  if (parent->children == NULL) {
    parent->children = child;
  } else {
    Node *current = parent->children;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = child;
  }
}

// Function to print a tree
void printTree(Node *node, int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  printf("%s\n", node->name);
  Node *current = node->children;
  while (current != NULL) {
    printTree(current, depth + 1);
    current = current->next;
  }
}

// Function to free a tree
void freeTree(Node *node) {
  Node *current = node->children;
  while (current != NULL) {
    Node *next = current->next;
    freeTree(current);
    current = next;
  }
  free(node->name);
  free(node);
}

// Function to read a directory tree
Node *readDirectory(const char *path) {
  Node *node = createNode(path);
  Node *current = node;
  char command[1024];
  sprintf(command, "ls -1 %s", path);
  FILE *pipe = popen(command, "r");
  if (pipe == NULL) {
    return node;
  }
  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
    buffer[strlen(buffer) - 1] = '\0';
    if (buffer[0] == '.') {
      continue;
    }
    Node *child = createNode(buffer);
    addChild(current, child);
    if (buffer[strlen(buffer) - 1] == '/') {
      char path[1024];
      sprintf(path, "%s%s", current->name, buffer);
      readDirectory(path);
    }
  }
  pclose(pipe);
  return node;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Izmantošana: %s <direktorija>\n", argv[0]);
    return -1;
  }
  Node *root = readDirectory(argv[1]);
  printTree(root, 0);
  freeTree(root);
  return 0;
}
