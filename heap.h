#pragma once

#include <stddef.h>
#include <assert.h>

#define HEAP_ENTRY(type)            \
	struct {                    \
		struct type *child; \
		struct type *prev;  \
		struct type *next;  \
	}

#define HEAP_INIT(head)              \
	do {                         \
		(head)->root = NULL; \
	} while (0)

#define HEAP_INITIALIZER(root) \
	{                      \
		NULL           \
	}

#define HEAP_HEAD(name, type)      \
	struct name {              \
		struct type *root; \
	}

#define HEAP_CHILD(elm, field) (elm)->field.child
#define HEAP_PREV(elm, field) (elm)->field.prev
#define HEAP_NEXT(elm, field) (elm)->field.next
#define HEAP_ROOT(head) (head)->root
#define HEAP_EMPTY(head) (HEAP_ROOT(head) == NULL)

#define HEAP_PEEK(head) (HEAP_ROOT(head))
#define HEAP_POP(name, head) (name##_HEAP_DELETE_MIN((head)))
#define HEAP_INSERT(name, head, x) (name##_HEAP_INSERT((head), (x)))
#define HEAP_REMOVE(name, head, x) (name##_HEAP_REMOVE((head), (x)))

#define HEAP_PROTOTYPE(name, type)                                    \
	void name##_HEAP_INSERT(struct name *head, struct type *elm); \
	struct type *name##_HEAP_DELETE_MIN(struct name *head);       \
	void name##_HEAP_REMOVE(struct name *head, struct type *elm);

#define HEAP_IMPL(name, type, field, cmp)                                    \
	static struct type *name##_HEAP_MELD(struct type *a, struct type *b) \
	{                                                                    \
		assert(HEAP_NEXT(a, field) == NULL);                         \
		if ((cmp)(a, b)) {                                           \
			if (HEAP_CHILD(a, field) != NULL) {                  \
				HEAP_PREV(HEAP_CHILD(a, field), field) = b;  \
			}                                                    \
			if (HEAP_NEXT(b, field) != NULL) {                   \
				HEAP_PREV(HEAP_NEXT(b, field), field) = a;   \
			}                                                    \
			HEAP_NEXT(a, field) = HEAP_NEXT(b, field);           \
			HEAP_NEXT(b, field) = HEAP_CHILD(a, field);          \
			HEAP_CHILD(a, field) = b;                            \
			HEAP_PREV(b, field) = a;                             \
			return a;                                            \
		}                                                            \
		if (HEAP_CHILD(b, field) != NULL) {                          \
			HEAP_PREV(HEAP_CHILD(b, field), field) = a;          \
		}                                                            \
		if (HEAP_PREV(a, field) != NULL &&                           \
		    HEAP_CHILD(HEAP_PREV(a, field), field) != a) {           \
			HEAP_NEXT(HEAP_PREV(a, field), field) = b;           \
		}                                                            \
		HEAP_PREV(b, field) = HEAP_PREV(a, field);                   \
		HEAP_PREV(a, field) = b;                                     \
		HEAP_NEXT(a, field) = HEAP_CHILD(b, field);                  \
		HEAP_CHILD(b, field) = a;                                    \
		return b;                                                    \
	}                                                                    \
	static struct type *name##_HEAP_COMBINE_SIBLINGS(struct type *left)  \
	{                                                                    \
		HEAP_PREV(left, field) = NULL;                               \
		struct type *root = NULL;                                    \
		struct type *a = left;                                       \
		while (1) {                                                  \
			if (HEAP_NEXT(a, field) == NULL) {                   \
				root = a;                                    \
				break;                                       \
			}                                                    \
			struct type *b = HEAP_NEXT(a, field);                \
			HEAP_NEXT(a, field) = NULL;                          \
			b = name##_HEAP_MELD(a, b);                          \
			if (HEAP_NEXT(b, field) == NULL) {                   \
				root = b;                                    \
				break;                                       \
			}                                                    \
			a = HEAP_NEXT(b, field);                             \
		}                                                            \
		while (1) {                                                  \
			if (HEAP_PREV(root, field) == NULL) {                \
				return root;                                 \
			}                                                    \
			struct type *b = HEAP_PREV(root, field);             \
			HEAP_NEXT(b, field) = NULL;                          \
			root = name##_HEAP_MELD(b, root);                    \
		}                                                            \
	}                                                                    \
	struct type *name##_HEAP_DELETE_MIN(struct name *head)               \
	{                                                                    \
		if (HEAP_ROOT(head) == NULL)                                 \
			return NULL;                                         \
		struct type *root = HEAP_ROOT(head);                         \
		if (HEAP_CHILD(root, field) != NULL) {                       \
			struct type *root_child = HEAP_CHILD(root, field);   \
			HEAP_ROOT(head) =                                    \
				name##_HEAP_COMBINE_SIBLINGS(root_child);    \
		} else {                                                     \
			HEAP_ROOT(head) = NULL;                              \
		}                                                            \
		HEAP_CHILD(root, field) = NULL;                              \
		HEAP_PREV(root, field) = NULL;                               \
		HEAP_NEXT(root, field) = NULL;                               \
		return root;                                                 \
	}                                                                    \
	void name##_HEAP_REMOVE(struct name *head, struct type *elm)         \
	{                                                                    \
		if (HEAP_PREV(elm, field) == NULL) {                         \
			assert(HEAP_ROOT(head) == elm);                      \
			name##_HEAP_DELETE_MIN(head);                        \
			return;                                              \
		}                                                            \
		struct type *prev = HEAP_PREV(elm, field);                   \
		if (HEAP_NEXT(elm, field) != NULL) {                         \
			struct type *elm_next = HEAP_NEXT(elm, field);       \
			HEAP_PREV(elm_next, field) = prev;                   \
		}                                                            \
		if (HEAP_CHILD(prev, field) == elm) {                        \
			HEAP_CHILD(prev, field) = HEAP_NEXT(elm, field);     \
		} else {                                                     \
			HEAP_NEXT(prev, field) = HEAP_NEXT(elm, field);      \
		}                                                            \
		HEAP_PREV(elm, field) = NULL;                                \
		HEAP_NEXT(elm, field) = NULL;                                \
		if (HEAP_CHILD(elm, field) == NULL)                          \
			return;                                              \
		struct type *elm_child = HEAP_CHILD(elm, field);             \
		struct type *x = name##_HEAP_COMBINE_SIBLINGS(elm_child);    \
		HEAP_ROOT(head) = name##_HEAP_MELD(x, HEAP_ROOT(head));      \
	}                                                                    \
	void name##_HEAP_INSERT(struct name *head, struct type *elm)         \
	{                                                                    \
		assert(HEAP_CHILD(elm, field) == NULL &&                     \
		       HEAP_PREV(elm, field) == NULL &&                      \
		       HEAP_NEXT(elm, field) == NULL);                       \
		if (HEAP_ROOT(head) != NULL) {                               \
			HEAP_ROOT(head) =                                    \
				name##_HEAP_MELD(elm, HEAP_ROOT(head));      \
		} else {                                                     \
			HEAP_ROOT(head) = elm;                               \
		}                                                            \
	}\
