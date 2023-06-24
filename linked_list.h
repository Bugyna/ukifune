#pragma once
#include "error.h"


#define ITERATE_LIST_PTR(L_TYPE, L_NAME, NAME)\
for (L_TYPE##_NODE* NAME = L_NAME->first; NAME != NULL && NAME->val != NULL; NAME = NAME->next)

#define ITERATE_LIST(L_TYPE, L_NAME, NAME)\
for (L_TYPE##_NODE* NAME = L_NAME.first; NAME != NULL && NAME->val != NULL; NAME = NAME->next)

#define DEFINE_LINKED_LIST(NAME, VAL_TYPE)\
typedef struct NAME##_NODE NAME##_NODE;\
typedef struct NAME##_LIST NAME##_LIST;\
struct NAME##_NODE\
{\
	VAL_TYPE* val;\
	NAME##_NODE* prev;\
	NAME##_NODE* next;\
};\
struct NAME##_LIST\
{\
	int length;\
	NAME##_NODE* first;\
	NAME##_NODE* last;\
};\
void NAME##_LIST_APPEND_VAL(NAME##_LIST* l, VAL_TYPE val)\
{\
	if (l->last->val == NULL) {\
		l->last->val = malloc(sizeof(VAL_TYPE));\
		*l->last->val = val;\
		l->last->next = NULL;\
		return;\
	}\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->val = malloc(sizeof(VAL_TYPE));\
	*tmp->val = val;\
	tmp->prev = l->last;\
	tmp->next = NULL;\
	l->last->next = tmp;\
	l->last = tmp;\
	l->length++;\
}\
void NAME##_LIST_APPEND(NAME##_LIST* l, VAL_TYPE* val)\
	{\
	if (l->last->val == NULL) {\
		l->last->val = val;\
		l->last->next = NULL;\
	return;\
}\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->val = val;\
	tmp->prev = l->last;\
	tmp->next = NULL;\
	l->last->next = tmp;\
	l->last = tmp;\
	l->length++;\
}\
NAME##_NODE* NAME##_LIST_GET_AT_INDEX(NAME##_LIST* l, int i)\
{\
	int j = 0;\
	ITERATE_LIST_PTR(NAME, l, n)\
	{\
	if (j == i)\
	return n;\
	j++;\
}\
return NULL;\
}\
void NAME##_LIST_POP_AT_PTR(NAME##_LIST* l, NAME##_NODE* n)\
{\
	if (l->length <= 0) return;\
	free(n->val);\
	if (n == l->first && n == l->last) {\
	n->val = NULL;\
return;\
}\
	if (n == l->first) l->first = n->next;\
	if (n == l->last) l->last = n->prev;\
	if (n->prev != NULL) n->prev->next = n->next;\
	if (n->next != NULL) n->next->prev = n->prev;\
	free(n);\
	l->length--;\
}\
void NAME##_LIST_POP(NAME##_LIST* l)\
{\
	NAME##_LIST_POP_AT_PTR(l, l->last);\
}\
void NAME##_LIST_POP_AT_INDEX(NAME##_LIST* l, int i)\
{\
	if (l->length < 2) return;\
	if (i > l->length) return;\
	NAME##_LIST_POP_AT_PTR(l, NAME##_LIST_GET_AT_INDEX(l, i));\
}\
void NAME##_LIST_INIT(NAME##_LIST* l)\
{\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->prev = NULL;\
	tmp->next = NULL;\
	tmp->val = NULL;\
	l->length = 1;\
	l->first = tmp;\
	l->last = tmp;\
}\
void NAME##_LIST_INIT_VAL(NAME##_LIST* l, VAL_TYPE val)\
{\
	NAME##_LIST_INIT(l);\
	NAME##_LIST_APPEND_VAL(l, val);\
}

