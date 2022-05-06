#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void vector_init(vector * vec, unsigned short type_size, void (*child_deconstruct)(void *)){
    vec->type_size = type_size;
    vec->total_data = 0;
    vec->child_deconstruct = child_deconstruct;

    vec->buffer_health = 30;
    vec->dat = malloc(vec->buffer_health * vec->type_size); //Reserve 30 slots for the vector

    //Add function pointers to struct
    vec->size = vector_size;
    vec->index = vector_index;
    vec->push_back = vector_push_back;
    vec->pop_back = vector_pop_back;
    vec->replace = vector_replace;
}


//Allocate a new vector and return the pointer to the new vector
vector vector_create(unsigned short type_size, void (*child_deconstruct)(void *)){
    vector ret;
    vector_init(&ret, type_size, child_deconstruct);
    return ret;
}


//Checks if the buffer needs to be expanded
void vector_add_check(vector * vec){
    if(vec->buffer_health <= vec->total_data+1){
        vec->buffer_health += 20;
        vec->dat = realloc(vec->dat,  vec->buffer_health * vec->type_size);
    }
}

//Push a block of data to the end of the vector
void vector_push_back(vector * vec, void * data){
   vector_add_check(vec); //Make sure we have enough memory

   for(unsigned short i = 0; i < vec->type_size; i++){
    ((char*)vec->dat)[vec->total_data * vec->type_size + i] = *(char *)(data + i);
   }
   vec->total_data++;
}

//pop a block of data off of the end of the vector
void vector_pop_back(vector * vec){
    if(vec->child_deconstruct != NULL)
        vec->child_deconstruct(vec->index(vec, vec->total_data - 1));
    vec->total_data--;
}

//Get the size of the vector
unsigned long int vector_size(vector * vec){
    return vec->total_data;
}

//Get the value stored at the index in the vector
void * vector_index(vector * vec, unsigned long int index){
    return vec->dat + (index * vec->type_size);
}

//Replace data at a given index
void vector_replace(vector * vec, unsigned long index, void* data){
   for(unsigned short i = 0; i < vec->type_size; i++){
    ((char*)vec->dat)[index * vec->type_size + i] = *(char *)(data + i);
   }
}


//Cleans up self and children
void vector_deconstruct(vector * vec){
    for(unsigned long int i = 0; i < vec->total_data; i++){
        if(vec->child_deconstruct != NULL){
        	vec->child_deconstruct(vec->dat + (i * vec->type_size));
        }
    }
    free(vec->dat);
    //free(vec);
}

