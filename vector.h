//
#include <malloc.h>
#include <string.h>
//#include <assert.h>

typedef struct{
    void** items;
    size_t length;
    size_t capacity;
} Vector;

void vector_free(Vector *v) {
    size_t i;
    for (i = 0; i < v->length; i++) {
        free(v->items[i]);
    }
    free(v->items);
}

Vector str_split(const char* str, const char* delims) {
    Vector Str;
    char *str_t, *p;
    void **pp;
    //
    str_t = strdup(str);
    //
    Str.length = 0;
    Str.items = NULL;
    //
    for (p = str_t; p = strtok(p, delims); p != NULL) {
        pp = realloc(Str.items, sizeof(char*) * (Str.length+1));
        //assert(pp != NULL);
        Str.items = pp;
        Str.items[Str.length] = strdup(p);
        Str.length++;
        p = NULL;
    }
    free(str_t);
    return Str;
}

int array_in_array(Vector *arr_v, Vector *arr_s) {
    size_t i, j;
    int flag_t, flag_i;
    flag_t = 0;
    for (i = 0; i < arr_s->length; i++) {
        flag_i = 0;
        for (j = 0; j < arr_v->length; j++) {
            if (strcmp(arr_s->items[i], arr_v->items[j]) == 0) {
                flag_i = 1;
                break;
            }
        }
        if (flag_i == 1) {
            flag_t = 1;
        }
        else {
            flag_t = 0;
            break;
        }
    }
    return flag_t;
}
