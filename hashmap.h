#pragma once

#define DEFINE_HASHMAP(NAME, KEY_NAME, VAL_TYPE, VAL_NAME)\
typedef struct NAME##_BUCKET NAME##_BUCKET;\
struct NAME##_BUCKET\
{\
	const char* KEY_NAME;\
	VAL_TYPE VAL_NAME;\
	NAME##_BUCKET* next;\
};\
\
typedef struct \
{\
	NAME##_BUCKET* list;\
	int size, occupied;\
} NAME;
