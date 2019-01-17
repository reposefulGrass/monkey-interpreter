
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

status 
allocate_node (list *p_l, generic_ptr data) {
	list l = (list) malloc(sizeof(node));		

	if (l == NULL) {
		return ERROR;
	}

	DATA(l) = data;
	NEXT(l) = NULL;
	*p_l = l;

	return OK;
}

void 
free_node (list *p_l) {
	free(*p_l);
	*p_l = NULL;
}

status ll_initialize (list *p_l) {
	*p_l = NULL;

	return OK;
}

bool ll_is_empty (list l) {
	return (l == NULL)? true : false;
}

int ll_length (list p_l) {
	int length = 0;

	list cursor = NULL;
	while ((cursor = ll_iterator(p_l, cursor)) != NULL) {
		length++;	
	}

	return length;
}

status ll_prepend (list *p_l, generic_ptr data) {
	list l;

	if (allocate_node(&l, data) == ERROR) {
		return ERROR;
	}	

	NEXT(l) = *p_l;
	*p_l = l;

	return OK;
}

status ll_append (list *p_l, generic_ptr data) {
	list l;

	if (allocate_node(&l, data) == ERROR) {
		return ERROR;
	}

	if (ll_is_empty(*p_l)) {
		*p_l = l;
	}
	else {
		list temp = *p_l;
		for (; NEXT(temp) != NULL; temp = NEXT(temp))
			;

		NEXT(temp) = l;
	}

	return OK;
}

status ll_traverse (list l, status (*p_func_f)()) {
	if (ll_is_empty(l)) {
		return OK;
	}

	if ((*p_func_f)(DATA(l)) == ERROR) {
		return ERROR;
	}	
	else {
		return ll_traverse(NEXT(l), p_func_f);
	}
}

status ll_search (list l, generic_ptr data, int (*p_cmp_f)(), list *p_found) {
	list cursor = NULL;
	while ((cursor = ll_iterator(l, cursor)) != NULL) {
		if ((*p_cmp_f)(DATA(cursor), data) == 0) {
			*p_found = cursor;
			return OK;
		}	
	} 

	return ERROR;
}	

/* This function does not delete the data contained by the node */
status 
ll_delete_node (list *p_l, list node) {
	if (ll_is_empty(*p_l)) {
		return ERROR;
	}

	/* if `node` is the first node, simply skip it. */
	if (*p_l == node) {
		*p_l = NEXT(*p_l);
	}
	else /* find it */ {
		list l;

		/* set `l`  to the node infront of `node` */
		for (l = *p_l; l != NULL && NEXT(l) != node; l = NEXT(l))
			;

		if (l == NULL) {
			return ERROR;
		}
		else {
			NEXT(l) = NEXT(node); /* erase node */
		}
	}

	free_node(&node);
	return OK;
}

status ll_delete (list *p_l) {
	if (ll_is_empty(*p_l)) {
		return ERROR;
	}		

	return ll_delete_node(p_l, *p_l);
}

void ll_destroy (list *p_l, void (*p_func_f)()) {
	if (!ll_is_empty(*p_l)) {
		ll_destroy(&NEXT(*p_l), p_func_f);

		if (p_func_f != NULL) {
			(*p_func_f)(DATA(*p_l));
		}	

		free_node(p_l);
	}
}

list ll_iterator (list l, list lastreturn) {
	return (lastreturn == NULL)? l : NEXT(lastreturn);
}

