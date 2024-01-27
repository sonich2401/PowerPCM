#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef INLINE
#define INLINE inline __attribute__((always_inline))
#endif



INLINE void vector_init(vector* __restrict__ vec, unsigned short type_size, void (*child_deconstruct)(void *)){
    vec->type_size = type_size;
    vec->total_data = 0;
    vec->child_deconstruct = child_deconstruct;

    vec->buffer_health = 30;
    vec->dat = malloc(30 * type_size); //Reserve 30 slots for the vector

    //Add function pointers to struct
    vec->size = vector_size;
    vec->index = vector_index;
    vec->push_back = vector_push_back;
    vec->pop_back = vector_pop_back;
    vec->replace = vector_replace;
}


//Allocate a new vector and return the pointer to the new vector
INLINE vector vector_create(unsigned short type_size, void (*child_deconstruct)(void *)){
    vector ret;
    vector_init(&ret, type_size, child_deconstruct);
    return ret;
}


//Checks if the buffer needs to be expanded
static INLINE void vector_add_check(vector* __restrict__ vec){
    if(vec->buffer_health <= vec->total_data+1){
        vec->dat = realloc(vec->dat,  (vec->buffer_health + 20) * vec->type_size);
        vec->buffer_health += 20;
    }
}

//Push a block of data to the end of the vector
INLINE void vector_push_back(vector* vec, void* __restrict__ data){
   vector_add_check(vec); //Make sure we have enough memory

   for(unsigned short i = 0; i < vec->type_size; i++){
    ((char*)vec->dat)[vec->total_data * vec->type_size + i] = *(char *)(data + i);
   }
   vec->total_data++;
}

//pop a block of data off of the end of the vector
INLINE void vector_pop_back(vector* __restrict__ vec){
    if(vec->child_deconstruct != NULL)
        vec->child_deconstruct(vec->index(vec, vec->total_data - 1));
    vec->total_data--;
}

//Get the size of the vector
INLINE unsigned long int vector_size(vector* __restrict__ vec){
    return vec->total_data;
}

//Get the value stored at the index in the vector
INLINE void* vector_index(vector* __restrict__ vec, unsigned long int index){
    return vec->dat + (index * vec->type_size);
}

//Erase element at 'index'
void vector_erase(vector* vec, unsigned long index){
    if(vec->total_data == 0) return;
    if(index == vec->total_data - 1){
        vec->total_data--;
        return;
    }
    memcpy(vector_index(vec, index), vector_index(vec, index + 1), (vec->total_data - index - 1) * vec->type_size);
    vec->total_data--;
    return;
}

//Replace data at a given index
INLINE void vector_replace(vector* __restrict__ vec, unsigned long index, void* __restrict__ data){
   for(unsigned short i = 0; i < vec->type_size; i++){
    ((char*)vec->dat)[index * vec->type_size + i] = *(char *)(data + i);
   }
}


//Cleans up self and children
INLINE void vector_deconstruct(vector* __restrict__ vec){
    for(unsigned long int i = 0; i < vec->total_data; i++){
        if(vec->child_deconstruct != NULL){
        	vec->child_deconstruct(vec->dat + (i * vec->type_size));
        }
    }
    free(vec->dat);
    //free(vec);
}
