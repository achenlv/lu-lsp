#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 64
#define MAX_PERSONS 1000

// 1. datu struktūras definēšana

typedef struct Person {
    char name[MAX_NAME_LENGTH];
    char mother[MAX_NAME_LENGTH];
    char father[MAX_NAME_LENGTH];
    struct Person* next;
} Person;

typedef struct {
    Person* persons[MAX_PERSONS];
    int count;
} PersonTable;

PersonTable* createPersonTable() {
    PersonTable* table = (PersonTable*)malloc(sizeof(PersonTable));
    table->count = 0;
    for (int i = 0; i < MAX_PERSONS; i++) {
        table->persons[i] = NULL;
    }
    return table;
}

void addPerson(PersonTable* table, const char* name, const char* mother, const char* father) {
    Person* person = (Person*)malloc(sizeof(Person));
    strncpy(person->name, name, MAX_NAME_LENGTH);
    strncpy(person->mother, mother, MAX_NAME_LENGTH);
    strncpy(person->father, father, MAX_NAME_LENGTH);
    person->next = table->persons[table->count];
    table->persons[table->count++] = person;
}

// 2. Datu ievade un parsēšana
void parseInput(PersonTable* table) {
    char line[128];
    char name[MAX_NAME_LENGTH] = "";
    char mother[MAX_NAME_LENGTH] = "";
    char father[MAX_NAME_LENGTH] = "";

    while (fgets(line, sizeof(line), stdin)) {
        if (sscanf(line, "VARDS %s", name) == 1) {
            continue;
        }
        if (sscanf(line, "MATE %s", mother) == 1) {
            continue;
        }
        if (sscanf(line, "TEVS %s", father) == 1) {
            continue;
        }
        if (strlen(name) > 0) {
            addPerson(table, name, mother, father);
            name[0] = '\0';
            mother[0] = '\0';
            father[0] = '\0';
        }
    }
}

// 3. grafu izveide
typedef struct Node {
    char name[MAX_NAME_LENGTH];
    struct Node* next;
} Node;

typedef struct {
    Node* nodes[MAX_PERSONS];
    int count;
} Graph;

Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->count = 0;
    for (int i = 0; i < MAX_PERSONS; i++) {
        graph->nodes[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, const char* parent, const char* child) {
    Node* node = (Node*)malloc(sizeof(Node));
    strncpy(node->name, child, MAX_NAME_LENGTH);
    node->next = NULL;

    for (int i = 0; i < graph->count; i++) {
        if (strcmp(graph->nodes[i]->name, parent) == 0) {
            node->next = graph->nodes[i]->next;
            graph->nodes[i]->next = node;
            return;
        }
    }

    Node* parentNode = (Node*)malloc(sizeof(Node));
    strncpy(parentNode->name, parent, MAX_NAME_LENGTH);
    parentNode->next = node;
    graph->nodes[graph->count++] = parentNode;
}

// 4. topoloģiskā kārtošana un ciklu dtekcija
void topologicalSortUtil(Node* node, int* visited, char stack[MAX_PERSONS][MAX_NAME_LENGTH], int* stackIndex) {
    visited[node->name[0] - 'A'] = 1;

    Node* temp = node->next;
    while (temp) {
        if (!visited[temp->name[0] - 'A']) {
            topologicalSortUtil(temp, visited, stack, stackIndex);
        }
        temp = temp->next;
    }

    strncpy(stack[(*stackIndex)++], node->name, MAX_NAME_LENGTH);
}

void topologicalSort(Graph* graph) {
    int visited[MAX_PERSONS] = {0};
    char stack[MAX_PERSONS][MAX_NAME_LENGTH];
    int stackIndex = 0;

    for (int i = 0; i < graph->count; i++) {
        if (!visited[graph->nodes[i]->name[0] - 'A']) {
            topologicalSortUtil(graph->nodes[i], visited, stack, &stackIndex);
        }
    }

    for (int i = stackIndex - 1; i >= 0; i--) {
        printf("%s\n", stack[i]);
    }
}

// 5. rezultātu izdruka
void printResults(Graph* graph) {
    topologicalSort(graph);
}

// 6. klūdu apstrāde
int detectCycleUtil(Node* node, int* visited, int* recStack) {
    if (!visited[node->name[0] - 'A']) {
        visited[node->name[0] - 'A'] = 1;
        recStack[node->name[0] - 'A'] = 1;

        Node* temp = node->next;
        while (temp) {
            if (!visited[temp->name[0] - 'A'] && detectCycleUtil(temp, visited, recStack)) {
                return 1;
            } else if (recStack[temp->name[0] - 'A']) {
                return 1;
            }
            temp = temp->next;
        }
    }
    recStack[node->name[0] - 'A'] = 0;
    return 0;
}

int detectCycle(Graph* graph) {
    int visited[MAX_PERSONS] = {0};
    int recStack[MAX_PERSONS] = {0};

    for (int i = 0; i < graph->count; i++) {
        if (detectCycleUtil(graph->nodes[i], visited, recStack)) {
            return 1;
        }
    }
    return 0;
}

int main() {
    PersonTable* table = createPersonTable();
    parseInput(table);

    Graph* graph = createGraph();
    for (int i = 0; i < table->count; i++) {
        if (strlen(table->persons[i]->mother) > 0) {
            addEdge(graph, table->persons[i]->mother, table->persons[i]->name);
        }
        if (strlen(table->persons[i]->father) > 0) {
            addEdge(graph, table->persons[i]->father, table->persons[i]->name);
        }
    }

    if (detectCycle(graph)) {
        fprintf(stderr, "Error: Cycle detected\n");
        return 1;
    }

    printResults(graph);
    return 0;
}