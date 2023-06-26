#pragma once
#include "util.h"

#define __ITERATE_HASHMAP(TYPE, ENTRY, VAR_NAME)\
for (TYPE##_MAP_BUCKET* VAR_NAME = ENTRY; VAR_NAME != NULL && VAR_NAME->val != NULL; VAR_NAME = VAR_NAME->next)

#define ITERATE_HASHMAP(MAP, MAP_TYPE, VAL_TYPE, KEY)\
MAP_TYPE##_BUCKET* BUCKET = MAP_TYPE##_GET_BUCKET(MAP, KEY);\
VAL_TYPE* val;\
if (BUCKET != NULL) val = BUCKET->val;\
for (VAL_TYPE* val = BUCKET->val; BUCKET != NULL && val != NULL && BUCKET->next->key != NULL; BUCKET = BUCKET->next, val=BUCKET->val)

#define ITERATE_HASHMAP_INDEX(MAP, MAP_TYPE, VAL_TYPE, N)\
MAP_TYPE##_BUCKET* BUCKET = MAP_TYPE##_GET_BUCKET_AT_INDEX(MAP, N);\
VAL_TYPE* val;\
if (BUCKET != NULL) val = BUCKET->val;\
for (; BUCKET != NULL && val != NULL && BUCKET->next->key != NULL; BUCKET = BUCKET->next, val=BUCKET->val)

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
		UKI_WARNING("'%s' could not be found", key);\
		return NULL;\
	}\
\
	return b;\
}\
static inline NAME##_BUCKET* NAME##_GET_BUCKET_AT_INDEX(NAME* hm, int n)\
{\
	char* key = calloc(20, sizeof(char));\
	sprintf(key, "%d", n);\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
	check:\
	if (b != NULL && b->key != NULL && strcmp(b->key, key) && b->val != NULL) { b = b->next; goto check;}\
	if (b == NULL || b->val == NULL || b->key == NULL) {\
		UKI_WARNING("'%s' could not be found", key);\
		free(key);\
		return NULL;\
	}\
\
	free(key);\
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
		UKI_WARNING("'%s' could not be found", key);\
		return NULL;\
	}\
\
	return b->val;\
}\
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
	b->val = val;\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
}\
\
static inline void NAME##_ADD_AT_INDEX(NAME* hm, int n, VAL_TYPE* val)\
{\
	char* key = calloc(20, sizeof(char));\
	sprintf(key, "%d", n);\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	int i = 0;\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && b->next != NULL) { b = b->next; goto check;}\
\
	b->key = key;\
	b->val = val;\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
}\
\
static inline void NAME##_ADD_COPY(NAME* hm, const char* key, VAL_TYPE* val)\
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
static inline void NAME##_ADD_COPY_AT_INDEX(NAME* hm, int n, VAL_TYPE* val)\
{\
	char* key = calloc(20, sizeof(char));\
	sprintf(key, "%d", n);\
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
static inline void NAME##_ADD_DUPLICATE(NAME* hm, const char* key, VAL_TYPE* val)\
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



