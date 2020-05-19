#include <stdio.h>

#define _pr(l, fmt, ...) { \
    if (l <= _pl_) { \
        printf(fmt, ##__VA_ARGS__); \
        if (!l) { \
            while (1); \
        } \
    } \
}

#define _pl(l, fmt, ...) \
    _pr(l, "%10s: " fmt "\n", __func__, ##__VA_ARGS__)
