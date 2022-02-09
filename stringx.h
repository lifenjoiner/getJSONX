//
#include <malloc.h>
#include <string.h>

size_t strchrn(const char *str, char c) {
    char *p;
    size_t n;
    //
    n = 0;
    p = (char*)str - 1;
    while ((p = strchr(p + 1, c)) != NULL) { n++; }
    return n;
}
