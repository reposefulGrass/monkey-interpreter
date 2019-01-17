
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef enum { OK, ERROR }
	status;

typedef void *
	generic_ptr;

typedef struct node 
	node, 
	*list; 	


struct node {
	generic_ptr data;
	list next;		
};


status 	ll_initialize (list *p_l);
bool	ll_is_empty (list p_l);
int 	ll_length (list p_l);
status	ll_prepend (list *p_l, generic_ptr data);
status	ll_append (list *p_l, generic_ptr data);
status 	ll_search (list l, generic_ptr data, int (*p_cmp_f)(), list *p_found);
status	ll_delete (list *p_l);
status 	ll_delete_node (list *p_l, list node);
status 	ll_traverse (list l, status (*p_func_f)());
list 	ll_iterator (list l, list lastreturn);
void 	ll_destroy (list *p_l, void (*p_func_f)());

/* L must be of type `list` */
#define DATA(L) ((L)->data)
#define NEXT(L) ((L)->next)

#endif 
