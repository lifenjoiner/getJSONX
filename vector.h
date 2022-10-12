//
#include <malloc.h>
#include <tchar.h>
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
    v->length = 0;
    v->items = NULL;
}

Vector _tcs_split(const _TCHAR* str, const _TCHAR* delims) {
    Vector Str;
    _TCHAR *str_t, *p;
    void *pp;
    //
    str_t = _tcsdup(str);
    //
    Str.length = 0;
    Str.items = NULL;
    //
    for (p = str_t; p = _tcstok(p, delims), p != NULL; p = NULL) {
        pp = realloc(Str.items, sizeof(void*) * (Str.length+1));
        //assert(pp != NULL);
        Str.items = (void **)pp;
        Str.items[Str.length] = _tcsdup(p);
        Str.length++;
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
            if (_tcscmp((_TCHAR*)arr_s->items[i], (_TCHAR*)arr_v->items[j]) == 0) {
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
