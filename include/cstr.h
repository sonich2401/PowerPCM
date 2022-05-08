#pragma once


typedef char* cstr;

//aliases
#define cstradd(str1, str2) (cstrcat(str1, str2))
#define cstraddc(str1, ch) (cstrcat(str1, ch))
#define cstrstr strstr
#define cstrcstr strstr
#define cstrtok strtok
#define cstrfind strstr
#define cstrtokc cstrsplitc
#define cstrdup strdup
#define cstrdup_stack(src) (strcpy(alloca(strlen(src) + 1), src)) 

//#define CSTR_DEBUG
#define CSTR_RESTRICT __restrict__
//#define CSTR_COPY
//#define CSTR_NO_STRING_H
//#define CSTR_USE_CSTR_ALLOC
//#define CSTR_NO_STDIO_H
///////

#ifndef CSTR_NO_STRING_H
#include <string.h>
#endif


cstr cstrcat(cstr str1, cstr CSTR_RESTRICT str2);

cstr cstrcatc(cstr str1, char ch);

cstr cstrdel(cstr str, unsigned int start, unsigned int end);

cstr cstrsubs(cstr CSTR_RESTRICT str, unsigned int start, unsigned int end);

cstr* cstrsplit(cstr CSTR_RESTRICT str, cstr CSTR_RESTRICT find);

cstr* cstrsplitc(cstr CSTR_RESTRICT str, char find);

cstr cstrrem(cstr CSTR_RESTRICT str, char find);

cstr cstrrep(cstr string, cstr CSTR_RESTRICT search, cstr CSTR_RESTRICT replacement);

cstr cstrshl(cstr string, size_t amount);

cstr cstrshr(cstr string, size_t amount);

cstr cstrrm(cstr string, cstr CSTR_RESTRICT search);

size_t cstrcnt(cstr CSTR_RESTRICT string, cstr CSTR_RESTRICT search);


#ifdef CSTR_NO_STRING_H
#ifdef CSTR_USE_CSTR_ALLOC

void* malloc(unsigned long long size);
void* realloc(void* CSTR_RESTRICT ptr, unsigned long long size);
void free(void* CSTR_RESTRICT ptr);
#endif

void* memcpy(void* CSTR_RESTRICT dest, void* CSTR_RESTRICT src, unsigned long long bytes);
unsigned long long strlen(cstr CSTR_RESTRICT str);
cstr strstr(cstr CSTR_RESTRICT needle, cstr CSTR_RESTRICT haystack);
cstr* strtok(cstr CSTR_RESTRICT str, cstr CSTR_RESTRICT find);
cstr strcat(cstr CSTR_RESTRICT dest, cstr CSTR_RESTRICT src);

#endif

#ifdef CSTR_NO_STDIO_H

#define fprintf(a, str,  ...) (void)
#define stderr (void)
#define stdout (void)

#endif
