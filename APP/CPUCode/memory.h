/*
 * memory.h
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>

void* allocate(int size, int n);

void  deallocate(void** ptr);

size_t convert_to_1D_index(int i, int j, int k, int nz, int ny, int nx);

#endif /* MEMORY_H_ */
