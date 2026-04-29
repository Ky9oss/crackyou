// #include <stdalign.h>
// #include <stdio.h>
// #include <stdlib.h>

#define _GNU_SOURCE
#include "common_hdrs.h"

// #pragma pack(push, 1)

// typedef struct Node {
//     int key;
//     struct Node *prev;
//     struct Node *next;
// } Node;

// align = 8
typedef struct Node {
  int key;
  struct Node *next;
} Node;

// align = 8
typedef struct LinkedList {
  Node *head;
  int size;
} LinkedList;

LinkedList *createList();
Node *createNode(int data);
void destroyList(LinkedList *list);

void insertFront(LinkedList *list, int data);
void insertBack(LinkedList *list, int data);
void insertAt(LinkedList *list, int index, int data);

int deleteFront(LinkedList *list);
int deleteBack(LinkedList *list);
int deleteAt(LinkedList *list, int index);
int deleteValue(LinkedList *list, int value);

Node *find(LinkedList *list, int value);
void printList(LinkedList *list);
void printListReverse(LinkedList *list);
int getSize(LinkedList *list);

int main() {
  printf("%d\n", sizeof(Node));
  printf("%d\n", sizeof(LinkedList));
  return 0;
};

LinkedList *createList() {
  errno = 0;
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  if (list == NULL) {
    fatal_error(errno, "createList failed");
  };
  list->head = NULL;
  list->size = 0;
  return list;
}

Node *createNode(int key) {
  errno = 0;
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    fatal_error(errno, "createNode failed");
  };
  node->next = NULL;
  node->key = key;
  return node;
}
