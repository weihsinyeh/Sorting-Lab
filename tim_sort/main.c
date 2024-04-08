#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "sort_impl.h"
#include "cpucycles.h"
typedef struct {
    struct list_head list;
    int val;
    int seq;
} element_t;

int SAMPLES = 10;

static void create_sample(struct list_head *head, element_t *space, int samples)
{
    //printf("Creating sample\n");
    int sorted_number = 32;
    int sorted_start = 0;
    int sort = 0;
    for (int i = 0; i < samples; i++) {
        element_t *elem = space + i;
        if(sort == 0) {
            elem->val = rand();
            if(elem->val % 5 == 0){
                sorted_number = rand() % 32;
                sorted_start=elem->val;
                sort = 1;
            }
        }
        else{
            elem->val = ++sorted_start; 
            sorted_number--;
            if(sorted_number == 0)
                sort = 0;
        }
        list_add_tail(&elem->list, head);
    }
}

static void copy_list(struct list_head *from,
                      struct list_head *to,
                      element_t *space)
{
    if (list_empty(from))
        return;

    element_t *entry;
    list_for_each_entry (entry, from, list) {
        element_t *copy = space++;
        copy->val = entry->val;
        copy->seq = entry->seq;
        list_add_tail(&copy->list, to);
    }
}

int compare(void *priv, const struct list_head *a, const struct list_head *b)
{
    if (a == b)
        return 0;

    int res = list_entry(a, element_t, list)->val -
              list_entry(b, element_t, list)->val;

    if (priv)
        *((int *) priv) += 1;

    return res;
}

bool check_list(struct list_head *head, int count)
{
    if (list_empty(head))
        return 0 == count;

    element_t *entry, *safe;
    size_t ctr = 0;
    list_for_each_entry_safe (entry, safe, head, list) {
        ctr++;
    }
    int unstable = 0;
    list_for_each_entry_safe (entry, safe, head, list) {
        if (entry->list.next != head) {
            if (entry->val > safe->val) {
                fprintf(stderr, "\nERROR: Wrong order\n");
                return false;
            }
            if (entry->val == safe->val && entry->seq > safe->seq)
                unstable++;
        }
    }
    if (unstable) {
        fprintf(stderr, "\nERROR: unstable %d\n", unstable);
        return false;
    }

    if (ctr != SAMPLES) {
        fprintf(stderr, "\nERROR: Inconsistent number of elements: %ld\n", ctr);
        return false;
    }
    return true;
}

typedef void (*test_func_t)(void *priv,
                            struct list_head *head,
                            list_cmp_func_t cmp);

typedef struct {
    char *name;
    test_func_t impl;
} test_t;
const char *filename = "out_coutting_timsort_sorted_input.txt";
//const char *filename = "out_coutting_timsort_gallop_sorted_input.txt";
int main(void)
{
    struct list_head sample_head, warmdata_head, testdata_head;
    int count;
    int nums = SAMPLES;

    /* Assume ASLR */
    srand((uintptr_t) &main);

    test_t tests[] = {
        {.name = "timesort", .impl = timsort},
        {NULL, NULL},
    };
    test_t *test = tests;

    while (test->impl) {
        printf("==== Testing %s ====\n", test->name);
        for (int number = 10; number <= 10000; number++) {
            INIT_LIST_HEAD(&sample_head);
            element_t *samples = calloc(number, sizeof(*samples));
            element_t *testdata = calloc(number, sizeof(*testdata));

            create_sample(&sample_head, samples, number);
            /* Warm up */
            INIT_LIST_HEAD(&testdata_head);
            copy_list(&sample_head, &testdata_head, testdata);

            /* Test */
            int64_t *before_ticks = calloc(1, sizeof(int64_t));
            int64_t *after_ticks = calloc(1, sizeof(int64_t));
            int64_t *exec_times = calloc(1, sizeof(int64_t));
            SAMPLES = number;

            count = 0;
            before_ticks[0] = cpucycles();
            
            test->impl(&count, &testdata_head, compare);

            after_ticks[0] = cpucycles();
            exec_times[0] = after_ticks[0] - before_ticks[0];

            FILE *output_file = fopen(filename, "a");
            if (!output_file) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            //fprintf(output_file, "%ld\n", exec_times[0]);
            fprintf(output_file, "%d\n", count);
            fclose(output_file);

            // printf("  Comparisons:    %d\n", count);
            // printf("  List is %s\n",
            //    check_list(&testdata_head, nums) ? "sorted" : "not sorted");
            free(testdata);
            free(samples);
        }
        test++;
    }

    return 0;
}