#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 64

// 1. datu struktūras definēšana

typedef struct Person {
    char name[MAX_NAME_LENGTH];
    char mother[MAX_NAME_LENGTH];
    char father[MAX_NAME_LENGTH];
    struct Person* next;
    struct Person* children;
} Person;

typedef struct {
    Person* head;
    int count;
} PersonTable;

PersonTable* createPersonTable() {
    PersonTable* table = (PersonTable*)malloc(sizeof(PersonTable));
    table->head = NULL;
    table->count = 0;
    return table;
}

void addPerson(PersonTable* table, const char* name, const char* mother, const char* father) {
    Person* person = (Person*)malloc(sizeof(Person));
    strncpy(person->name, name, MAX_NAME_LENGTH);
    strncpy(person->mother, mother, MAX_NAME_LENGTH);
    strncpy(person->father, father, MAX_NAME_LENGTH);
    person->next = table->head;
    person->children = NULL;
    table->head = person;
    table->count++;
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

// 3. Ģimenes koka izveide
Person* findPerson(PersonTable* table, const char* name) {
    Person* current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void addChild(Person* parent, Person* child) {
    child->next = parent->children;
    parent->children = child;
}

void buildFamilyTree(PersonTable* table) {
    Person* current = table->head;
    while (current) {
        if (strlen(current->mother) > 0) {
            Person* mother = findPerson(table, current->mother);
            if (mother) {
                addChild(mother, current);
            }
        }
        if (strlen(current->father) > 0) {
            Person* father = findPerson(table, current->father);
            if (father) {
                addChild(father, current);
            }
        }
        current = current->next;
    }
}

// 4. Rezultātu izdruka
void printFamilyTree(Person* person, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%s\n", person->name);
    Person* child = person->children;
    while (child) {
        printFamilyTree(child, level + 1);
        child = child->next;
    }
}

void printResults(PersonTable* table) {
    Person* current = table->head;
    while (current) {
        if (strlen(current->mother) == 0 && strlen(current->father) == 0) {
            printFamilyTree(current, 0);
        }
        current = current->next;
    }
}

int main() {
    PersonTable* table = createPersonTable();
    parseInput(table);
    buildFamilyTree(table);
    printResults(table);
    return 0;
}