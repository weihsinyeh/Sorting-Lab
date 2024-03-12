#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cpucycles.h"
#include "list.h"
typedef struct __node {
    int value;
    struct list_head list;
} node_t;


struct list_head *list_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));

    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}


struct list_head *list_tail(struct list_head *head)
{
    return head->prev;
}

int list_length(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    int count = 0;
    struct list_head *node;
    list_for_each (node, head)
        count += 1;

    return count;
}

struct list_head *list_construct(struct list_head *head, int n)
{
    if (!head)
        return NULL;

    node_t *new_element = (node_t *) malloc(sizeof(node_t));
    if (!new_element)
        return NULL;

    new_element->value = n;
    list_add_tail(&(new_element->list), head);

    return head;
}

void list_free(struct list_head *head)
{
    if (!head)
        return;

    node_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list)
        free(entry);

    free(head);
    return;
}
/* Verify if list is order */
static bool list_is_ordered(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    node_t *entry = NULL, *safe = NULL;
    list_for_each_entry_safe (entry, safe, head, list) {
        if (&safe->list == head)
            break;
        if (entry->value > safe->value)
            return false;
    }

    return true;
}
static inline void list_replace(struct list_head *old, struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}
void list_swap(struct list_head *entry1, struct list_head *entry2)
{
    struct list_head *pos = entry2->prev;

    list_del(entry2);
    list_replace(entry1, entry2);
    if (pos == entry1)
        pos = entry2;
    list_add(entry1, pos);
}
void random_pivot(struct list_head *head)
{
    if (!head || list_is_singular(head))
        return;
    int r = rand() % list_length(head);
    struct list_head *cur;
    list_for_each (cur, head) {
        if (r == 0)
            break;
        r--;
    }
    if (head->next != cur)
        list_swap(head->next, cur);
}
/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n)
{
    if (n <= 0)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}
void print(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    node_t *entry = NULL;

    list_for_each_entry (entry, head, list)
        printf("%d ", entry->value);
}
struct list_head *quick_sort(struct list_head *head)
{
    if (!head || list_empty(head))
        return NULL;

    int n = list_length(head);
    int i = 0, value;
    int max_level = 2 * n;

    struct list_head **begin = calloc(sizeof(struct list_head *), max_level);

    struct list_head *result = NULL, *left = NULL, *right = NULL, *mid = NULL;

    result = list_new();
    begin[0] = head;

    while (i >= 0) {
        struct list_head *L = begin[i]->next, *R = begin[i]->prev;
        if (L != R) {
            right = list_new();
            left = list_new();
            mid = list_new();

            random_pivot(L);

            node_t *pivot = list_entry(L, node_t, list);
            value = pivot->value;

            list_del(&pivot->list);
            list_add(&pivot->list, mid);

            struct list_head *iter = L->next;  // iterator node_t
            struct list_head *next = NULL;

            list_for_each_safe (iter, next, begin[i]) {
                list_del(iter);
                list_add(iter, list_entry(iter, node_t, list)->value < value
                                   ? right
                                   : left);
            }

            begin[i] = left;
            begin[i + 1] = mid;
            begin[i + 2] = right;
            left = right = mid = NULL;

            i += 2;

        } else {
            if (!(!L || list_empty(L)))
                list_splice_tail(begin[i], result);

            i--;
        }
    }
    return result;
}
const char *filename = "out_ra.txt";
void main(int argc, char **argv)
{
    for (int number = 10; number <= 10000; number++) {
        size_t count = number;
        int64_t *before_ticks = calloc(1, sizeof(int64_t));
        int64_t *after_ticks = calloc(1, sizeof(int64_t));
        int64_t *exec_times = calloc(1, sizeof(int64_t));

        struct list_head *list = list_new();

        int *test_arr = calloc(count, sizeof(int));

        for (int i = 0; i < count; ++i)
            test_arr[i] = i;
        //shuffle(test_arr, count);
        while (count--)
            list = list_construct(list, test_arr[count]);

        before_ticks[0] = cpucycles();
        list = quick_sort(list);
        after_ticks[0] = cpucycles();
        exec_times[0] = after_ticks[0] - before_ticks[0];

        FILE *output_file = fopen(filename, "a");
        if (!output_file) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fprintf(output_file, "%ld\n", exec_times[0]);
        fclose(output_file);

        assert(list_is_ordered(list));

        list_free(list);
        free(test_arr);
    }
    return;
}