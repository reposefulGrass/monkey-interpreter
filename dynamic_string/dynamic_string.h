
#ifndef DSTR_H
#define DSTR_H

// The initial capacity of ds_t.string when it is created
#define DS_INITIAL_CAPACITY 100

// The growth rate of ds_t.string whenever it is reallocated
#define DS_CAPACITY_GROWTH 1.25

typedef struct {
    char *string;
    int capacity;
    int size;    
} ds_t;

ds_t   *ds_initialize ();
ds_t   *ds_initialize_with (unsigned int capacity);
void    ds_append (ds_t *str, char *s); 
char   *ds_to_string (ds_t *str);
void    ds_destroy (ds_t *str);

#endif 



