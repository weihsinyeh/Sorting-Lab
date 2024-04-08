#include <math.h>
int i_sqrt(int x)
{
    if (x <= 1) /* Assume x is always positive */
        return x;
    int c = 0;

    for (int d = 1UL << ((31 - __builtin_clz(x)) & ~1UL);  d; d >>= 2) {
        int y = c + d;
        c >>= 1;
        if (x >= y)
            x -= y, c += d;       
    }
    return c;
}
/** version 2
int i_sqrt(int N)
{
    // version 1 
    // int msb = (int) log2(N);
    int msb = 0;
    int n = N;
    while (n > 1) {
        n >>= 1;
        msb++;
    }
    printf("%d\n",msb);
    int a = 1 << msb;
    int result = 0;
    while (a != 0) {
        if ((result + a) * (result + a) <= N)
            result += a;
        a >>= 1;
    }
    return result;
}*/
#include <stdio.h>
int main()
{
    int X = 64;
    printf("%d\n", i_sqrt(X));
    return 0;
}