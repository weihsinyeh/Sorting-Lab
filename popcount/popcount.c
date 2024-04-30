#include <stdio.h>
#include <time.h>
#include "cpucycles.h"
#define bench(statement)                                                  \
    ({                                                                    \
        struct timespec _tt1, _tt2;                                       \
        clock_gettime(CLOCK_MONOTONIC, &_tt1);                            \
        statement;                                                        \
        clock_gettime(CLOCK_MONOTONIC, &_tt2);                            \
        long long time = (long long) (_tt2.tv_sec * 1e9 + _tt2.tv_nsec) - \
                         (long long) (_tt1.tv_sec * 1e9 + _tt1.tv_nsec);  \
        time;                                                             \
    })

int popcount_nibble(unsigned v)
{
    unsigned tmp = v - ((v >> 1) & 0x77777777) - ((v >> 2) & 0x33333333) -
                   ((v >> 3) & 0x11111111);
    return ((tmp + (tmp >> 4)) & 0x0F0F0F0F) % 255;
}
int popcount_oct(unsigned v)
{
    unsigned tmp = v - ((v >> 1) & 033333333333) - ((v >> 2) & 011111111111);
    return ((tmp + (tmp >> 3)) & 030707070707) % 63;
}
unsigned popcount_branchless(unsigned v)
{
    unsigned n;
    n = (v >> 1) & 0x77777777;
    v -= n;
    n = (n >> 1) & 0x77777777;
    v -= n;
    n = (n >> 1) & 0x77777777;
    v -= n;

    v = (v + (v >> 4)) & 0x0F0F0F0F;
    v *= 0x01010101;

    return v >> 24;
}
static char table[256] =
  {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
  };
int lookuptable_popcount(unsigned v)
{
    return table[v & 0xff] + table[(v >> 8) & 0xff] +
           table[(v >> 16) & 0xff] + table[(v >> 24) & 0xff];
}
int (*function_p[])(unsigned) = {   lookuptable_popcount, \
                                    popcount_branchless, \ 
                                    popcount_oct, \
                                    popcount_nibble};
static char* function_name[5] = {"__builtin_popcount", "lookup table", "branchless", "oct", "nibble"};
int main(void)
{
    int64_t execution_time[5] = {0};
    int64_t builtin_time = 0;
    int64_t lookuptable_time = 0;
    int64_t original_time = 0;
    int64_t nibble_time = 0;
    int64_t oct_time = 0;
    int64_t *before_ticks = calloc(1, sizeof(int64_t));
    int64_t *after_ticks = calloc(1, sizeof(int64_t));
    int64_t *exec_times = calloc(1, sizeof(int64_t));

    for (unsigned i = 0; i < 1000000000; i++) {
       
        before_ticks[0] = cpucycles();
        __builtin_popcount(i);
        after_ticks[0] = cpucycles();
        execution_time[0] += after_ticks[0] - before_ticks[0];
        
        for (int j =0;j<4;j++){
            before_ticks[0] = cpucycles();
            function_p[j](i);
            after_ticks[0] = cpucycles();
            execution_time[j+1] += after_ticks[0] - before_ticks[0];
        }
    }
    for(int j=0;j<5;j++){
        printf("popcount use %s avg cpu cycle : %ld\n",function_name[j], \
                (int64_t) execution_time[j] / 1000000000);
    }

    return 0;
}