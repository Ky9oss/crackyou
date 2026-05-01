#define _GNU_SOURCE
#include "common_hdrs.h"

#define ARRAY_MAX 1000

// #pragma pack(push, 1)

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

void deleteFront(LinkedList *list);
void deleteBack(LinkedList *list);
void deleteAt(LinkedList *list, int index);
void deleteValue(LinkedList *list, int value);

Node *find(LinkedList *list, int value);
void printList(LinkedList *list);
void printListReverse(LinkedList *list);
int getSize(LinkedList *list);

int main() {
  // printf("%d\n", sizeof(Node));
  // printf("%d\n", sizeof(LinkedList));

  LinkedList *list = createList();
  insertFront(list, 3);
  insertFront(list, 4);
  insertFront(list, 5);
  insertBack(list, 8);
  insertBack(list, 9);
  // 54389

  insertAt(list, 3, 7);
  // 543789

  printList(list);
  printListReverse(list);

  find(list, 7);
  find(list, 8);
  find(list, 9);
  find(list, 0);
  find(list, 4);
  find(list, 5);
  find(list, 6);
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

void destroyList(LinkedList *list) {
  Node *node = list->head;
  while (node->next != NULL) {
    Node *next_node = node->next;
    free(node);
    node = next_node;
  };
  free(node);
  free(list);
  printf("Destroyed\n");
}

void insertFront(LinkedList *list, int data) {
  Node *node = createNode(data);
  node->next = list->head;
  list->head = node;
  list->size += 1;
}

void insertBack(LinkedList *list, int data) {
  Node *node = createNode(data);
  node->next = nullptr;

  Node *last_node = list->head;
  for (int i = 0; i < list->size - 1; i++) {
    last_node = last_node->next;
  };

  last_node->next = node;
  list->size += 1;
}

void insertAt(LinkedList *list, int index, int data) {

  if (index == 0) {
    insertBack(list, data);
  }

  Node *node = createNode(data);

  Node *current_node = list->head;

  for (int i = 0; i < index - 1; i++) {
    current_node = current_node->next;
  };

  if (index == list->size) {
    node->next = nullptr;
  } else {
    node->next = current_node->next;
  }

  current_node->next = node;

  list->size += 1;
}

Node *find(LinkedList *list, int value) {

  int found = 0;
  Node *current_node = list->head;
  for (int i = 0; i < list->size - 1; i++) {
    if (current_node->key == value) {
      found = 1;
      break;
    };
    current_node = current_node->next;
  };

  if (current_node->key == value) {
    found = 1;
  };

  if (found == 0) {
    printf("Node(key = %d) nod found\n", value);
  } else {
    printf("Found Node: key: %d\n", current_node->key);
  };

  return current_node;
}

void printList(LinkedList *list) {
  Node *current_node = list->head;
  for (int i = 0; i < list->size - 1; i++) {
    printf("Index: %d, Key: %d\n", i, current_node->key);
    current_node = current_node->next;
  };
  printf("Index: %d, Key: %d\n", list->size - 1, current_node->key);
}

void printListReverse(LinkedList *list) {
  int arr[ARRAY_MAX];
  Node *current_node = list->head;
  arr[0] = current_node->key;

  for (int i = 0; i < list->size - 1; i++) {
    current_node = current_node->next;
    arr[i + 1] = current_node->key;
  };

  for (int i = list->size - 1; i >= 0; i--) {
    printf("%d\n", arr[i]);
  };
}
