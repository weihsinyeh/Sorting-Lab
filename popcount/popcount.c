#include <stdio.h>
#include <time.h>
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
int main(void)
{
    long long builtin_time = 0;
    long long lookuptable_time = 0;
    long long original_time = 0;
    long long nibble_time = 0;
    long long oct_time = 0;
    for (unsigned i = 0; i < 1000000000; i++) {
        builtin_time += bench(__builtin_popcount(i));
        lookuptable_time += bench(lookuptable_popcount(i));
        original_time += bench(popcount_branchless(i));
        oct_time += bench(popcount_oct(i));
        nibble_time += bench(popcount_nibble(i));
    }
    printf("popcount use builtin function avg time : %lf\n",
           (double) builtin_time / 1000000000);
    printf("popcount use lookup table avg time : %lf\n",
           (double) lookuptable_time / 1000000000);
    printf("popcount use branchless avg time : %lf\n",
           (double) original_time / 1000000000);
    printf("popcount use nibble avg time : %lf\n",
           (double) nibble_time / 1000000000);
    printf("popcount use oct avg time : %lf\n", (double) oct_time / 1000000000);

    return 0;
}