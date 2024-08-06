#pragma once

#define HEAP_ENTRY(type)                                                       \
  struct {                                                                     \
    struct type *child;                                                        \
    struct type *prev;                                                         \
    struct type *next;                                                         \
  }

#define HEAP_INIT(head)                                                        \
  do {                                                                         \
    (head)->root = NULL;                                                       \
  } while (0)

#define HEAP_INITIALIZER(root)                                                 \
  { NULL }

#define HEAP_HEAD(name, type)                                                  \
  struct name {                                                                \
    struct type *root;                                                         \
  }

#define HEAP_CHILD(elm, field) (elm)->field.child
#define HEAP_PREV(elm, field) (elm)->field.prev
#define HEAP_NEXT(elm, field) (elm)->field.next
#define HEAP_ROOT(head) (head)->root
#define HEAP_EMPTY(head) (HEAP_ROOT(head) == NULL)

#define HEAP_PEEK(head) (HEAP_ROOT(head))
