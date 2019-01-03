
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef enum { OK, ERROR }
	status;

typedef enum { FALSE = 0, TRUE = 1 }
	bool;

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
status	ll_append (list *p_l, generic_ptr data);
status	ll_prepend (list *p_l, generic_ptr data);
status	ll_delete (list *p_l);
bool	ll_equal (list *p_list1, list *p_list2, bool (*p_cmp_f)());
status 	ll_copy (list from, list *to, size_t size, void (*p_copy_f)());
list 	ll_iterator (list l, list lastreturn);
void 	ll_destroy (list *p_l, void (*p_func_f)());
status 	ll_delete_node (list *p_l, list node);
status 	ll_traverse (list l, status (*p_func_f)());
status 	ll_insert (list *p_l, generic_ptr data, int (*p_cmp_f)());
status 	ll_search (list l, generic_ptr data, int (*p_cmp_f)(), list *p_found);
status 	ll_reverse (list *p_l);

/* L must be of type `list` */
#define DATA(L) ((L)->data)
#define NEXT(L) ((L)->next)

#endif 
