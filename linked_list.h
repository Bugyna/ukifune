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
	SDL_Log("freeing: %p", l->last->VAL_NAME);\
	free(l->last->VAL_NAME);\
	NAME##_NODE* tmp = l->last->prev;\
	free(l->last);\
	tmp->next = NULL;\
	l->last = tmp;\
	l->length--;\
}\
void NAME##_LIST_INIT(NAME##_LIST* l)\
{\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->prev = tmp;\
	tmp->next = tmp;\
	l->length = 1;\
	l->first = tmp;\
	l->last = tmp;\
}\

