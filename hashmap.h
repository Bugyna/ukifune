#pragma once
#include "util.h"

#define DEFINE_HASHMAP(NAME, VAL_TYPE)\
typedef struct NAME NAME;\
typedef struct NAME##_BUCKET NAME##_BUCKET;\
struct NAME##_BUCKET\
{\
	const char* key;\
	VAL_TYPE* val;\
\
	NAME##_BUCKET* next;\
};\
struct NAME \
{\
	NAME##_BUCKET* list;\
	int size, occupied;\
};\
void NAME##_INIT(NAME* hm, int size)\
{\
	if (size < 0) size = 20;\
	hm->list = calloc(size, sizeof(NAME##_BUCKET));\
	hm->size = size;\
	hm->occupied = 0;\
}\
\
static inline NAME##_BUCKET* NAME##_GET_BUCKET(NAME* hm, const char* key)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
	check:\
	if (b != NULL && b->val && b->key != NULL && strcmp(b->key, key)) { b = b->next; goto check;}\
	if (b == NULL || b->val == NULL || b->key == NULL) {\
		UKI_ERROR("'%s' could not be found", key);\
	}\
\
	return b;\
}\
static inline VAL_TYPE* NAME##_GET(NAME* hm, const char* key)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	check:\
	if (b != NULL && b->val && b->key != NULL && strcmp(b->key, key)) { b = b->next; goto check;}\
	if (b == NULL || b->val == NULL || b->key == NULL) {\
		UKI_ERROR("'%s' could not be found", key);\
	}\
\
	return b->val;\
}\
\
static inline void NAME##_ADD(NAME* hm, const char* key, VAL_TYPE* val)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	int i = 0;\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && b->next != NULL) { b = b->next; goto check;}\
\
	b->key = key;\
	b->val = malloc(sizeof(VAL_TYPE));\
	memcpy(b->val, val, sizeof(VAL_TYPE));\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
}\
\
static inline void NAME##_REMOVE(NAME* hm, const char* key)\
{\
	NAME##_BUCKET* b = NAME##_GET_BUCKET(hm, key);\
	NAME##_BUCKET* tmp = b->next;\
	if (tmp != NULL && tmp->key != NULL) b = tmp;\
	else {\
		b->val = NULL;\
		b->key = NULL;\
	}\
\
	hm->occupied--;\
}



