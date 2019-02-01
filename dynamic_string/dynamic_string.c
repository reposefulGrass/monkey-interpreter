
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_string.h"


ds_t *
ds_initialize_with (unsigned int capacity) {
    ds_t *str = (ds_t *) malloc(sizeof(ds_t));
    if (str == NULL) {
        fprintf(
            stderr, 
            "Error in function 'ds_initialize': Unable to allocate ds_t 'str'!\n"
        );
        exit(EXIT_FAILURE);
    }

    str->string = (char *) malloc(sizeof(char) * capacity); 
    // null-terminate the str so that the strcat in 'ds_append' works properly
    memset(str->string, '\0', 1);

    str->capacity = capacity;
    str->size = 0;

    return str;
}


ds_t *
ds_initialize () {
    return ds_initialize_with(DS_INITIAL_CAPACITY);
}


void
ds_append (ds_t *str, char *s) {
    int length = strlen(s);

    int new_capacity = str->capacity;
    while (str->size + length >= str->capacity) {
        new_capacity *= DS_CAPACITY_GROWTH;
    }

    str->string = realloc(str->string, new_capacity);
    if (str->string == NULL) {
        fprintf(
            stderr, 
            "Error in function 'ds_append': Unable to reallocate ds_t 'str'!\n"
        );
        exit(EXIT_FAILURE);
    }

    str->capacity = new_capacity;

    strncat(str->string, s, length);
    str->size += length;
}


char *
ds_to_string (ds_t *str) {
    char *string = str->string;
    free(str);
    return string;
}


void    
ds_destroy (ds_t *str) {
    free(str->string);
    free(str);
}


