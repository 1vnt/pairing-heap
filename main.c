#include "heap.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct timer {
  int deadline;
  HEAP_ENTRY(timer) entry;
};

int timer_cmp(struct timer *a, struct timer *b) {
  return a->deadline < b->deadline;
}

HEAP_HEAD(heap, timer) head;

#define field entry
#define type timer
#define name heap
#define cmp timer_cmp

void printPairingHeap(struct type *root);

static struct type *heap_HEAP_MELD(struct name *head, struct type *a,
                                   struct type *b) {
  assert(HEAP_NEXT(a, field) == NULL);

  if ((cmp)(a, b)) {
    if (HEAP_CHILD(a, field) != NULL) {
      HEAP_PREV(HEAP_CHILD(a, field), field) = b;
    }
    if (HEAP_NEXT(b, field) != NULL) {
      HEAP_PREV(HEAP_NEXT(b, field), field) = a;
    }
    HEAP_NEXT(a, field) = HEAP_NEXT(b, field);
    HEAP_NEXT(b, field) = HEAP_CHILD(a, field);
    HEAP_CHILD(a, field) = b;
    HEAP_PREV(b, field) = a;
    return a;
  }

  if (HEAP_CHILD(b, field) != NULL) {
    HEAP_PREV(HEAP_CHILD(b, field), field) = a;
  }

  if (HEAP_PREV(a, field) != NULL &&
      HEAP_CHILD(HEAP_PREV(a, field), field) != a) {
    HEAP_NEXT(HEAP_PREV(a, field), field) = b;
  }

  HEAP_PREV(b, field) = HEAP_PREV(a, field);
  HEAP_PREV(a, field) = b;
  HEAP_NEXT(a, field) = HEAP_CHILD(b, field);
  HEAP_CHILD(b, field) = a;
  return b;
}

static struct type *heap_HEAP_COMBINE_SIBLINGS(struct name *head,
                                               struct type *left) {
  HEAP_PREV(left, field) = NULL;

  struct type *root = NULL;
  struct type *a = left;
  while (1) {
    if (HEAP_NEXT(a, field) == NULL) {
      root = a;
      break;
    }
    struct type *b = HEAP_NEXT(a, field);
    HEAP_NEXT(a, field) = NULL;
    b = heap_HEAP_MELD(head, a, b);
    if (HEAP_NEXT(b, field) == NULL) {
      root = b;
      break;
    }
    a = HEAP_NEXT(b, field);
  }

  while (1) {
    if (HEAP_PREV(root, field) == NULL) {
      return root;
    }
    struct type *b = HEAP_PREV(root, field);
    HEAP_NEXT(b, field) = NULL;
    root = heap_HEAP_MELD(head, b, root);
  }
}

struct type *heap_HEAP_DELETE_MIN(struct name *head) {
  if (HEAP_ROOT(head) == NULL)
    return NULL;

  struct type *root = HEAP_ROOT(head);

  if (HEAP_CHILD(root, field) != NULL) {
    struct type *root_child = HEAP_CHILD(root, field);
    HEAP_ROOT(head) = heap_HEAP_COMBINE_SIBLINGS(head, root_child);
  } else {
    HEAP_ROOT(head) = NULL;
  }

  HEAP_CHILD(root, field) = NULL;
  HEAP_PREV(root, field) = NULL;
  HEAP_NEXT(root, field) = NULL;

  return root;
}

void heap_HEAP_REMOVE(struct name *head, struct type *elm) {
  if (HEAP_PREV(elm, field) == NULL) {
    // if not current root and no prev, elm not on this heap
    // thus assert
    assert(HEAP_ROOT(head) == elm);
    heap_HEAP_DELETE_MIN(head);
    return;
  }

  struct type *prev = HEAP_PREV(elm, field);

  if (HEAP_NEXT(elm, field) != NULL) {
    struct type *elm_next = HEAP_NEXT(elm, field);
    HEAP_PREV(elm_next, field) = prev;
  }

  if (HEAP_CHILD(prev, field) == elm) {
    HEAP_CHILD(prev, field) = HEAP_NEXT(elm, field);
  } else {
    HEAP_NEXT(prev, field) = HEAP_NEXT(elm, field);
  }

  HEAP_PREV(elm, field) = NULL;
  HEAP_NEXT(elm, field) = NULL;

  if (HEAP_CHILD(elm, field) == NULL)
    return;

  // if children, merge back in
  struct type *elm_child = HEAP_CHILD(elm, field);
  struct type *x = heap_HEAP_COMBINE_SIBLINGS(head, elm_child);
  HEAP_ROOT(head) = heap_HEAP_MELD(head, x, HEAP_ROOT(head));
}

void heap_HEAP_INSERT(struct name *head, struct type *elm) {
  assert(HEAP_CHILD(elm, field) == NULL && HEAP_PREV(elm, field) == NULL &&
         HEAP_NEXT(elm, field) == NULL);

  if (HEAP_ROOT(head) != NULL) {
    HEAP_ROOT(head) = heap_HEAP_MELD(head, elm, HEAP_ROOT(head));
  } else {
    HEAP_ROOT(head) = elm;
  }
}

#define NUM_TIMERS 10 * 1000 * 1000
void million_values() {
  struct timer *timers = calloc(NUM_TIMERS, sizeof(struct timer));
  assert(timers != NULL);

  int value = 0;
  for (int i = 0; i < NUM_TIMERS; i++) {
    if (i % 100 == 0)
      value += 1;
    timers[i].deadline = value;
  }

  struct heap heap;
  HEAP_INIT(&heap);

  for (int i = 0; i < NUM_TIMERS; i++) {
    heap_HEAP_INSERT(&heap, &timers[i]);
  }

  int count = 0;
  int last = 0;
  struct timer *elm;

  while ((elm = heap_HEAP_DELETE_MIN(&heap)) != NULL) {
    count++;
    assert(elm->deadline >= last);
    last = elm->deadline;
  }

  assert(heap_HEAP_DELETE_MIN(&heap) == NULL);
  assert(count == NUM_TIMERS);

  free(timers);
}

int main() {
  HEAP_INIT(&head);

  struct timer t1 = {.deadline = 100,
                     .entry = {.prev = NULL, .child = NULL, .next = NULL}};

  struct timer t2 = {.deadline = 150,
                     .entry = {.prev = NULL, .child = NULL, .next = NULL}};

  struct timer t3 = {.deadline = 25,
                     .entry = {.prev = NULL, .child = NULL, .next = NULL}};

  struct timer t4 = {.deadline = 10,
                     .entry = {.prev = NULL, .child = NULL, .next = NULL}};

  struct timer t5 = {.deadline = 500,
                     .entry = {.prev = NULL, .child = NULL, .next = NULL}};

  struct timer t6 = {.deadline = 50,
                     .entry = {.prev = NULL, .child = NULL, .next = NULL}};

  struct timer t7 = {.deadline = 50,
                     .entry = {.prev = NULL, .child = NULL, .next = NULL}};

  assert(HEAP_EMPTY(&head) == 1);
  heap_HEAP_INSERT(&head, &t6);
  heap_HEAP_INSERT(&head, &t4);
  heap_HEAP_INSERT(&head, &t3);
  heap_HEAP_INSERT(&head, &t1);
  heap_HEAP_INSERT(&head, &t2);
  heap_HEAP_INSERT(&head, &t5);
  heap_HEAP_INSERT(&head, &t7);
  assert(HEAP_EMPTY(&head) == 0);
  assert(heap_HEAP_DELETE_MIN(&head)->deadline == 10);
  assert(heap_HEAP_DELETE_MIN(&head)->deadline == 25);
  assert(heap_HEAP_DELETE_MIN(&head)->deadline == 50);
  assert(heap_HEAP_DELETE_MIN(&head)->deadline == 50);
  assert(heap_HEAP_DELETE_MIN(&head)->deadline == 100);
  assert(heap_HEAP_DELETE_MIN(&head)->deadline == 150);
  assert(heap_HEAP_DELETE_MIN(&head)->deadline == 500);
  assert(heap_HEAP_DELETE_MIN(&head) == NULL);
  assert(HEAP_EMPTY(&head) == 1);

  million_values();

  return 0;
}
