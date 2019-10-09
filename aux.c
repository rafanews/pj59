#include "aux.h"

int long_compare(void * x, void * y) {
    long l = *((long *) x);
    long r = *((long *) y);
    return l < r ? -1 : l > r ? 1 : 0;
}

int string_compare(void * x, void * y) {
    return strcmp(x, y);
}

int char_create(struct sector_list * sector_list, struct string * string, char ** result) {
    int status = 0;
    char * object;

    object = sector_list_malloc(sector_list, string->offset + 1);
    if(!object) {
        status = panic("out of memory");
    } else {
        object[string->offset] = 0;
        *result = memcpy(object, string->string, string->offset);
    }

    return status;
}

int char_create2(struct sector_list * sector_list, char * string, size_t length, char ** result) {
    int status = 0;
    char * object;

    object = sector_list_malloc(sector_list, length + 1);
    if(!object) {
        status = panic("out of memory");
    } else {
        object[length] = 0;
        *result = memcpy(object, string, length);
    }

    return status;
}

void char_destroy(char * object) {
    sector_list_free(object);
}
