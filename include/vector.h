#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct{
    unsigned long buffer_health;
    unsigned long total_data;

    void * dat;
    unsigned short type_size;
    void (*child_deconstruct)(void *);
    void (*push_back)(void*, void*);
    void* (*index)(void*, unsigned long);
    unsigned long (*size)(void*);
    void (*pop_back)(void*);
    void (*replace)(void*, unsigned long , void*);
}vector;

//Initalize a vector
void vector_init(vector * vec, unsigned short type_size, void (*child_deconstruct)(void *));

/*Allocate a new vector and return it
  Set Child decon to NULL for int, char, float, etc*/
vector vector_create(unsigned short type_size,  void (*child_deconstruct)(void *));

//Push a block of data to the end of the vector
void vector_push_back(vector * vec, void * data);

//pop a block of data off of the end of the vector
void vector_pop_back(vector * vec);

//Get the value stored at the index in the vector
void * vector_index(vector * vec, unsigned long index);

//Erase element at 'index'
void vector_erase(vector* vec, unsigned long index);

//Replace data at a given index
void vector_replace(vector * vec, unsigned long index, void* data);

//Get the size of the vector
unsigned long vector_size(vector * vec);

//Cleans up self and children
void vector_deconstruct(vector * vec);

#ifdef __cplusplus
};
#endif


#endif