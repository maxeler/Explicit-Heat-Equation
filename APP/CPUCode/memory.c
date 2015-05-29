/*
 * memory.c
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#include <stdio.h>
#include "memory.h"
#include "stdlib.h"

void* allocate(int size, int n){
    void* ptr = calloc(n, size);
    if(!ptr){
        printf("Error: Unable to allocate %d bytes of memory!\n", size);
        exit(-1);
    }

    return ptr;
}

void deallocate(void** ptr){
    free(*ptr);
    *ptr = NULL;
}

size_t convert_to_1D_index (int i, int j, int k, int nz, int ny, int nx){
    return (i*ny +j)*nx +k;
}
