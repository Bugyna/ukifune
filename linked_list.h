#pragma once

#define DEFINE_LINKED_LIST(NAME, VAL_TYPE, VAL_NAME)\
typedef struct NAME##_NODE NAME##_NODE;\
typedef struct NAME##_LIST NAME##_LIST;\
struct NAME##_NODE\
{\
	VAL_TYPE* VAL_NAME;\
	NAME##_NODE* prev;\
	NAME##_NODE* next;\
};\
struct NAME##_LIST\
{\
	int length;\
	NAME##_NODE* first;\
	NAME##_NODE* last;\
};\
void NAME##_LIST_APPEND(NAME##_LIST* l, VAL_TYPE* val)\
{\
	if (l->last->VAL_NAME == NULL) {\
		l->last->VAL_NAME = val;\
		return;\
	}\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->VAL_NAME = val;\
	tmp->prev = l->last;\
	tmp->next = NULL;\
	l->last->next = tmp;\
	l->last = tmp;\
	l->length++;\
}\
void NAME##_LIST_POP(NAME##_LIST* l)\
{\
	if (l->length < 2) return;\
	free(l->last->VAL_NAME);\
	NAME##_NODE* tmp = l->last->prev;\
	free(l->last);\
	tmp->next = NULL;\
	l->last = tmp;\
	l->length--;\
}\
void NAME##_LIST_POP_INDEX(NAME##_LIST* l, int i)\
{\
	if (l->length < 2) return;\
	if (i > l->length) return;\
	NAME##_NODE* tmp = l->first+i*sizeof(NAME##_NODE);\
	free(tmp->VAL_NAME);\
	tmp->prev->next = tmp->next;\
	tmp->next->prev = tmp->prev;\
	free(tmp);\
	l->length--;\
}\
void NAME##_LIST_INIT(NAME##_LIST* l)\
{\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->prev = tmp;\
	tmp->next = tmp;\
	tmp->VAL_NAME = NULL;\
	l->length = 1;\
	l->first = tmp;\
	l->last = tmp;\
}\
void NAME##_LIST_INIT_VAL(NAME##_LIST* l, VAL_TYPE val)\
{\
	NAME##_LIST_INIT(l);\
	l->first->VAL_NAME = malloc(sizeof(VAL_TYPE));\
	*l->first->VAL_NAME = val;\
}\

