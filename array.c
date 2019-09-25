#include "array.h"

int array_create(struct array * array, size_t size, size_t count) {
    int status = 0;

    if(!size) {
        status = panic("size is zero");
    } else if(!count) {
        status = panic("count is zero");
    } else {
        array->size = size;
        array->count = count;
        array->buffer = malloc(array->size * array->count);
        if(!array->buffer)
            status = panic("out of memory");
    }

    return status;
}

void array_destroy(struct array * array) {
    free(array->buffer);
}

int array_expand(struct array * array) {
    int status = 0;
    void * buffer;

    buffer = realloc(array->buffer, array->size * array->count * 2);
    if(!buffer) {
        status = panic("out of memory");
    } else {
        array->count *= 2;
        array->buffer = buffer;
    }

    return status;
}

void * array_index(struct array * array, size_t index) {
    void * element = NULL;

    if(index < array->count)
        element = (char *) array->buffer + array->size * index;

    return element;
}

int stack_create(struct stack * stack, size_t size, size_t count) {
    int status = 0;

    stack->top = 0;
    if(array_create(&stack->array, size, count))
        status = panic("failed to create stack object");

    return status;
}

void stack_destroy(struct stack * stack) {
    array_destroy(&stack->array);
}

int stack_push_value(struct stack * stack, void * element) {
    int status = 0;

    if(stack->top >= stack->array.count && array_expand(&stack->array)) {
        status = panic("stack is full");
    } else {
        memcpy(array_index(&stack->array, stack->top), element, stack->array.size);
        stack->top++;
    }

    return status;
}

void * stack_push_reference(struct stack * stack) {
    void * element = NULL;

    if(stack->top < stack->array.count || !array_expand(&stack->array)) {
        element = array_index(&stack->array, stack->top);
        stack->top++;
    }

    return element;
}

void * stack_pop(struct stack * stack) {
    void * element = NULL;

    if(stack->top) {
        element = array_index(&stack->array, stack->top - 1);
        stack->top--;
    }

    return element;
}

void stack_clear(struct stack * stack) {
    stack->top = 0;
}

int string_create(struct string * string, size_t length) {
    int status = 0;

    if(!length) {
        status = panic("length is zero");
    } else {
        string->offset = 0;
        string->length = length;
        string->string = calloc(length, sizeof(char));
        if(!string->string)
            status = panic("out of memory");
    }

    return status;
}

void string_destroy(struct string * string) {
    free(string->string);
}

int string_expand(struct string * string, size_t expand) {
    int status = 0;
    size_t length;
    char * buffer;

    if(!expand) {
        status = panic("expand is zero");
    } else {
        length = string->offset + string->length;
        buffer = realloc(string->string, length + expand);
        if(!buffer) {
            status = panic("out of memory");
        } else {
            memset(buffer + length, 0, expand);
            string->length += expand;
            string->string = buffer;
        }
    }

    return status;
}

int string_putc(struct string * string, char c) {
    int status = 0;

    if(string->length < 2 && string_expand(string, string->offset + string->length)) {
        status = panic("out of memory");
    } else {
        string->string[string->offset] = c;
        string->offset++;
        string->length--;
    }

    return status;
}

int string_strdup(struct string * string, char * buffer, size_t length) {
    int status = 0;

    if(string->length < length + 1 && string_expand(string, length + 1 - string->length)) {
        status = panic("out of memory");
    } else {
        memcpy(string->string + string->offset, buffer, length);
        string->offset += length;
        string->length -= length;
    }

    return status;
}

void string_clear(struct string * string) {
    memset(string->string, 0, string->offset);
    string->length += string->offset;
    string->offset = 0;
}
