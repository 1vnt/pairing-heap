#include "heap.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct timer {
	int deadline;
	HEAP_ENTRY(timer) entry;
};

int timer_cmp(struct timer *a, struct timer *b)
{
	return a->deadline < b->deadline;
}

HEAP_HEAD(timer_heap, timer) head;
HEAP_IMPL(timer_heap, timer, entry, timer_cmp);

#define NUM_TIMERS 10 * 1000 * 1000
void million_values()
{
	struct timer *timers = calloc(NUM_TIMERS, sizeof(struct timer));
	assert(timers != NULL);

	int value = 0;
	for (int i = 0; i < NUM_TIMERS; i++) {
		if (i % 100 == 0)
			value += 1;
		timers[i].deadline = value;
	}

	HEAP_INIT(&head);

	for (int i = 0; i < NUM_TIMERS; i++) {
		HEAP_INSERT(timer_heap, &head, &timers[i]);
	}

	int count = 0;
	int last = 0;
	struct timer *elm;

	while ((elm = HEAP_POP(timer_heap, &head)) != NULL) {
		count++;
		assert(elm->deadline >= last);
		last = elm->deadline;
	}

	assert(HEAP_POP(timer_heap, &head) == NULL);
	assert(count == NUM_TIMERS);

	free(timers);
}

void simple_heap()
{
	struct timer t1 = {
		.deadline = 100,
		.entry = { .prev = NULL, .child = NULL, .next = NULL }
	};

	struct timer t2 = {
		.deadline = 150,
		.entry = { .prev = NULL, .child = NULL, .next = NULL }
	};

	struct timer t3 = {
		.deadline = 25,
		.entry = { .prev = NULL, .child = NULL, .next = NULL }
	};

	struct timer t4 = {
		.deadline = 10,
		.entry = { .prev = NULL, .child = NULL, .next = NULL }
	};

	struct timer t5 = {
		.deadline = 500,
		.entry = { .prev = NULL, .child = NULL, .next = NULL }
	};

	struct timer t6 = {
		.deadline = 50,
		.entry = { .prev = NULL, .child = NULL, .next = NULL }
	};

	struct timer t7 = {
		.deadline = 50,
		.entry = { .prev = NULL, .child = NULL, .next = NULL }
	};

	HEAP_INIT(&head);

	HEAP_INSERT(timer_heap, &head, &t1);
	HEAP_INSERT(timer_heap, &head, &t2);
	HEAP_INSERT(timer_heap, &head, &t3);
	HEAP_INSERT(timer_heap, &head, &t4);
	HEAP_INSERT(timer_heap, &head, &t5);
	HEAP_REMOVE(timer_heap, &head, &t1);
	HEAP_INSERT(timer_heap, &head, &t6);
	HEAP_INSERT(timer_heap, &head, &t7);
	assert(HEAP_POP(timer_heap, &head)->deadline == 10);
	assert(HEAP_POP(timer_heap, &head)->deadline == 25);
	assert(HEAP_POP(timer_heap, &head)->deadline == 50);
	assert(HEAP_POP(timer_heap, &head)->deadline == 50);
	assert(HEAP_POP(timer_heap, &head)->deadline == 150);
	assert(HEAP_POP(timer_heap, &head)->deadline == 500);
}

int main()
{
	simple_heap();

	million_values();

	return 0;
}
