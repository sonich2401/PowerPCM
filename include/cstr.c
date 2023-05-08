#include "cstr.h"
#include "types.h"
#include <stdio.h>
#ifndef CSTR_NO_STRING_H

#include <string.h>
#endif

#ifndef CSTR_USE_CSTR_ALLOC
#include <stdlib.h>

#else
void INLINE abort(){
	puts("FATAL ERR");
	while(1){}
}
#endif


INLINE cstr cstrshl(cstr CSTR_RESTRICT string, size_t amount){
    size_t string_strlen = strlen(string);
    for(size_t i = 0; i < string_strlen - (amount - 1); i++){
        *(string + i) = *(string + i + amount);
    }
    return string;
}

INLINE cstr cstrshr(cstr CSTR_RESTRICT string, size_t amount){
    size_t string_strlen = strlen(string);
    for(size_t i = string_strlen - (amount - 1); i > 0; i--){
        *(string + i) = *(string + i - amount);
    }
    return string;
}

INLINE cstr cstrrm(char* string, char* CSTR_RESTRICT search){
    char* ret = string;
    char* old_string = string;
    while((string = strstr(string, search)) != NULL && string != old_string){
        old_string = string;
        //iterate through all letters and shift them over by 1
        string = cstrshl(string, strlen(search));
        string++;
    }
    return ret;
}


cstr cstrrep(cstr string, cstr __restrict__ search, cstr __restrict__ replacement){
    char* ret = strdup(string);

    //Count the number of times that the search string was found
    size_t token_count = cstrcnt(string, search);

    //No tokens found
    if(token_count == 0)
        return ret;

    size_t search_strlen = strlen(search);
    size_t replacement_strlen = strlen(replacement);

    //Should we realloc?
    if(search_strlen < replacement_strlen){
        size_t nchar_count = strlen(string) - (token_count * search_strlen);
        ret = (char*)realloc(ret, nchar_count + (token_count * replacement_strlen) + 1);
    }

    string = ret; //restore string ptr
    while(*string != 0){
        if(*string != search[0]){ //Dont waste an expensive strstr if we dont need it
            string++;
            continue;
        }
        //Is this a replacement?
        char* strstr_tmp = strstr(string, search);
        if(strstr_tmp != NULL && strstr_tmp == string){
            //replacement confirmed. Start making room for replacment
            if(replacement_strlen > search_strlen)
                //for(unsigned long long i = 0; i < replacement_strlen - search_strlen; i++)
                    string = cstrshr(string, replacement_strlen - search_strlen);
            else
                //for(unsigned long long i = 0; i < search_strlen - replacement_strlen; i++)
                    string = cstrshl(string, search_strlen - replacement_strlen);
                
            
            //Copy in replacement
            memcpy(string, replacement, replacement_strlen);
            string += replacement_strlen; //move cursor past the replaced string
            continue;
        }

        string++; //Letter was not equal to 'search'
    }

    return ret;
}

INLINE size_t cstrcnt(cstr __restrict__ string, cstr  __restrict__ search){
    size_t ret = 0;
    cstr string_old = string;
    while((string = strstr(string, search)) != NULL && string != string_old){
        ret++;
        string++;
    }
    return ret;
}

INLINE cstr cstrcat(cstr str1, cstr CSTR_RESTRICT str2){
	unsigned long size = sizeof(char) * (strlen(str1) + strlen(str2) + 1);
	str1 = (cstr)realloc(str1, size);
	#ifdef CSTR_DEBUG
		if(str1 == NULL){
			fprintf(stderr, "ERR: cstrcat(): Not Enough Ram! Tried to alloc %lu bytes.\n", size);
			abort();
		}
	#endif
	str1 = strcat(str1, str2);
	return str1;
}

INLINE cstr cstrcatc(cstr str1, char ch){
	unsigned long str1_size = strlen(str1);
	unsigned long size = sizeof(char) * (str1_size + 1 + 1);
	str1 = (cstr)realloc(str1, size);
	#ifdef CSTR_DEBUG
		if(str1 == NULL){
			fprintf(stderr, "ERR: cstrcatc(): Not Enough Ram! Tried to alloc %lu bytes\n", size);
			abort();
		}
	#endif
	str1[str1_size] = ch;
	str1[str1_size + 1] = 0;
	return str1;
}

cstr cstrdel(cstr str, unsigned int start, unsigned int end){
	#ifndef CSTR_COPY
		if(start == 0) return str + end;
	#endif
	unsigned long str_size = strlen(str);
	if(end == 0) end = str_size;
	
	#ifdef CSTR_DEBUG
		unsigned long size = sizeof(char) * (str_size - (end - start) + 1);
	#endif
	
	#ifdef CSTR_COPY
		cstr ret = (cstr)malloc(size);
	#else
		cstr ret = str;
	#endif

	#ifdef CSTR_DEBUG
		if(ret == NULL){
			fprintf(stderr, "ERR: cstrcatc(): Not Enough Ram! Tried to alloc %lu bytes\n", size);
			abort();
		}
	#endif

	unsigned long ret_i = 0;
	#ifdef CSTR_COPY	
		for(unsigned long i = 0; i < str_size; i++){
			if(i >= start && i < end) continue;
			ret[ret_i] = str[i];
			ret_i++;
		}
	#else
		ret_i = start;
		for(unsigned long i = end; i < str_size - end + 1; i++){
			ret[ret_i] = str[end + i];
			ret_i++;
		}
	#endif
	ret[str_size - (end - start)] = 0;
	return ret;
}


cstr* cstrsplit(cstr CSTR_RESTRICT str, cstr CSTR_RESTRICT find){
	unsigned long find_strlen = strlen(find);
	//unsigned long str_strlen = strlen(str);
	
	unsigned long list_size = 0;
	cstr* ret = malloc(1);
	
	cstr last_cursor = str;
	cstr cursor = str;
	while((cursor = strstr(cursor, find)) != NULL){
		unsigned long size = cursor - last_cursor;
		list_size++;
		ret = (cstr*)realloc(ret, sizeof(cstr*) * list_size);
		ret[list_size - 1] = (cstr)malloc(sizeof(char) * size);
		memcpy(ret[list_size - 1], last_cursor, size);
		cursor += find_strlen;
		last_cursor = cursor;
	}
	
	if(list_size == 0){
		free(ret);
		return NULL;
	}
	return ret;
}



INLINE cstr cstrsubs(cstr CSTR_RESTRICT str, unsigned int start, unsigned int end){
	//unsigned long str_size = strlen(str);
	unsigned long size = sizeof(char) * (end - start) + 1;
	cstr ret = (cstr)malloc(size);
	#ifdef CSTR_DEBUG
		if(ret == NULL){
			fprintf(stderr, "ERR: cstrsubs(): Not Enough Ram! Tried to alloc %lu bytes\n", size);
			abort();
		}
	#endif
	ret = (cstr)memcpy(ret, str + start, (end - start));
	ret[(end - start)] = 0;
	return ret;
}

INLINE cstr* cstrsplitc(cstr CSTR_RESTRICT str, char find){
	char tmpstr[2];
	tmpstr[0] = find;
	tmpstr[1] = 0;
	return cstrsplit(str, tmpstr);
}

INLINE cstr cstrrem(cstr CSTR_RESTRICT str, char find){
	char* cursor = strrchr(str, find);
	
	while(cursor != NULL){
		while(*cursor != 0){
			cursor[0] = cursor[1];
			cursor++;
		}
		cursor = strrchr(str, find);
	}
	
	return str;
}


/*
cstr* cstrrep(cstr string, cstr CSTR_RESTRICT find, cstr CSTR_RESTRICT replaceWith){
		unsigned long string_strlen = strlen(string);
		
		char* tmp_str = alloca(string_strlen);
		*tmp_str = 0;
		char* cursor = strtok(string, find);
		if(cursor == NULL) return string;
		while(cursor != NULL){
			strcat(tmp_str, cursor);
			strcat(tmp_str, replaceWith);
			cursor = strtok(NULL, replaceWith);
		}
		strcpy(string, tmp_str);
    
    
    
    return string;
}*/




#ifdef CSTR_NO_STRING_H
#ifdef CSTR_USE_CSTR_ALLOC

#define CSTR_RAM_SIZE (1000 * 100 * 50)


/*typedef struct{
	void* ptr;
	unsigned long long size;
}malloc_entry;*/

typedef unsigned long long alloc_t;

struct malloc_entry{
	void* ptr;
	alloc_t size;
	//struct malloc_entry* last;
	struct malloc_entry* next;
};

#define CSTR_MAX_ENTRIES ((1000 * 1000) / sizeof(struct malloc_entry))

char RAM[CSTR_RAM_SIZE];
unsigned long long allocs = 0;

struct malloc_entry table_entries[CSTR_MAX_ENTRIES];

void init_alloc(struct malloc_entry* CSTR_RESTRICT ptr, alloc_t size){
	ptr->ptr = NULL;
	ptr->size = size;
	ptr->next = NULL;
	//ptr->last = NULL;
}

void delete_alloc(struct malloc_entry* CSTR_RESTRICT ptr, struct malloc_entry* CSTR_RESTRICT ptr_last){
	ptr->ptr = NULL;
	ptr->size = 0;
	if(ptr_last != NULL)
	ptr_last->next = ptr->next;
	//ptr->next->last = ptr->last;
}

#define is_null(i) (table_entries[i].ptr == NULL)

struct malloc_entry* root =NULL;

struct malloc_entry* get_back(struct malloc_entry* CSTR_RESTRICT ptr){
	malloc_entry* node = ptr;
	while(node->next != NULL){
		node = node->next;
	}
	return node;
}

void init_heap(){
	for(alloc_t i = 0; i < CSTR_MAX_ENTRIES; i++){
		init_alloc(&table_entries[i], 0);
	}
}

void emplace_node(struct malloc_entry* CSTR_RESTRICT at, struct malloc_entry* CSTR_RESTRICT at_last, struct malloc_entry* CSTR_RESTRICT node){
	//node->last = at;
	node->next = at->next;
	node->ptr = at->ptr + at->size;
	//if(at->next != NULL) at->next->last = node;
	at->next = node;
	//if(at_last != NULL)
	//at_last->next = node;
}

void* malloc(unsigned long long size){
	if(root == NULL) init_heap();
	alloc_t i = 0;
	if(is_null(allocs)) {
		i = allocs;
		goto __malloc__index__found__;
	}
	//If there is no open space at end then keep going and search
	for(; i < CSTR_MAX_ENTRIES; i++){
		if(is_null(i)){
			break;
		}
	}
	if(i == CSTR_MAX_ENTRIES) return NULL;
	
	//it is found
	__malloc__index__found__:
	init_alloc(&table_entries[i], size);
	
	if(allocs == 0){
		root = &table_entries[i];
		root->ptr = RAM;
		allocs++;
		return root->ptr;
	}
	//not first alloc
	struct malloc_entry* new = &table_entries[i];
	struct malloc_entry* ptr = root;
	struct malloc_entry* ptr_last = NULL;
	while(1){
		if(ptr->next == NULL && allocs != CSTR_MAX_ENTRIES){
			new->ptr = ptr->ptr + ptr->size;
			emplace_node(ptr, ptr_last, new);
			break;
		}
		void* ram_end = ptr->ptr + ptr->size + size;
		//printf("end %p, ptr %p\nnext %p, next->ptr %p\n", ram_end, ptr, ptr->next, ptr->next->ptr);
		if(ram_end < ptr->next->ptr){
			new->ptr = ptr->ptr + ptr->size;
			emplace_node(ptr, ptr_last, new);
			allocs++;
			return new->ptr;
		}
		ptr_last = ptr;
		ptr = ptr->next;
	}
	return NULL;
}

struct malloc_entry* search_alloc(struct malloc_entry* CSTR_RESTRICT node, void* CSTR_RESTRICT ptr){
	if(node == NULL) return NULL;
	if(node->ptr == ptr){
		return node;
	}
	return search_alloc(node->next, ptr);
}


void* realloc(void* CSTR_RESTRICT ptr, alloc_t size){
	char* ret = (char*)malloc(size);
	if(ret == NULL) return NULL;
	if(ptr == NULL) return ret;
	struct malloc_entry* node = search_alloc(root, ret);
	for(alloc_t i = 0; i < node->size; i++){
	  ret[i] = ((char*)ptr)[i];
	}
	free(ptr);
	return ret;
}


void free(void* CSTR_RESTRICT ptr){	
	struct malloc_entry* node_last = NULL;
	struct malloc_entry* node = root;
	
	while(1){
		if(node == NULL)
			abort();
		if(node->ptr == ptr){
			break;
		}
		node_last = node;
		node = node->next;
	}
	delete_alloc(node, node_last);
	allocs--;
}

void heap_info(){
	printf("allocs %llu\n", allocs);
	printf("sizeof(struct malloc_entry) = %llu\n", sizeof(struct malloc_entry));
	for(alloc_t i = 0; i < CSTR_MAX_ENTRIES && i < 30; i++){
		printf("B#%llu: ", i);
		if(is_null(i)){
			puts("NULL");
		}else{
			printf("ptr = %p, size = %llu, end = %p\n", table_entries[i].ptr, table_entries[i].size, table_entries[i].ptr + table_entries[i].size);
		}
	}
}

#endif

void* memcpy(void* CSTR_RESTRICT dest, void* CSTR_RESTRICT src, unsigned long long bytes){
	for(unsigned long long i = 0; i < bytes; i++){
		((char*)dest)[i] = ((char*)src)[i];
	}
  return dest;
}
unsigned long long strlen(cstr CSTR_RESTRICT str){
	unsigned long long size = 0;
	
	while(str[size] != 0){
		size++;
	}
	return size;
}
cstr strstr(cstr CSTR_RESTRICT needle, cstr CSTR_RESTRICT haystack){
	cstr str = haystack;
	unsigned long long needle_len = strlen(needle);
	char match = 1;
	while(*str != 0){
		for(unsigned long long i = 0; i < needle_len; i++){
			if(str[i] != needle[i]){
				i = needle_len; //break
				match = 0;
			}
		}
		if(!match){
			str++;
			match = 1;
		}else{
			return str;
		}
	}
	return NULL;
}
cstr* strtok(cstr CSTR_RESTRICT str, cstr CSTR_RESTRICT find);

cstr strcat(cstr CSTR_RESTRICT dest, cstr CSTR_RESTRICT src){
	unsigned long long dest_len, src_len;
	dest_len = strlen(dest);
	src_len = strlen(src);
	
	for(unsigned long long i = dest_len; i < src_len + dest_len; i++){
		dest[i] = src[i - dest_len];
	}
	dest[src_len + dest_len] = 0;
	return dest;
}

#endif
