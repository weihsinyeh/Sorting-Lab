#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
void main(int argc, char **argv)
{
    struct list_head *list = list_new();

    size_t count = 100000;
    int *test_arr = calloc(sizeof(int), count);

    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    shuffle(test_arr, count);

    while (count--)
        list = list_construct(list, test_arr[count]);

    list = quick_sort(list);
    assert(list_is_ordered(list));
    // print(list);
    list_free(list);
    free(test_arr);

    return;
}