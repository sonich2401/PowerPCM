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
        vec->child_deconstruct(vector_index(vec, vec->total_data - 1));
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

void vector_merge(vector * vec, vector * source){
    if(vec->type_size != source->type_size) abort();

    if(vec->buffer_health - vec->total_data >= source->total_data){
        memcpy(vec->dat + (vec->total_data * vec->type_size), source->dat, source->total_data * vec->type_size);
    }else{
        vec->dat = (void*)realloc(vec->dat, (vec->total_data + source->total_data) * vec->total_data);
        #ifdef DEBUG
            if(vec->dat == NULL){
                fprintf(stderr, "ERR: Out of memory!!\n");
                exit(EXIT_FAILURE);
            }
        #endif
    }
}
