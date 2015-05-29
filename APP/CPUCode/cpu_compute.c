/*
 * cpu_compute.h
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#include <string.h>
#include "cpu_compute.h"
#include "memory.h"



float* compute_cpu(struct params *settings){
    size_t total_size = settings->x * settings->y * settings->z;
    float* in = allocate(sizeof(*in), total_size);
    float* out = allocate(sizeof(*out), total_size);

    memcpy(in, settings->input, total_size * sizeof(*in));

    float dt = settings->dt;
    float dx = settings->dx;
    float dy = settings->dy;
    float dz = settings->dz;

    int nx = settings->x;
    int ny = settings->y;
    int nz = settings->z;

    float* alpha = settings->alpha;


    float* tmp;

    for(int t = 0; t < settings->n_iterations; t++){
        for(int i = 1; i < nz-1; i++)
            for(int j = 1; j < ny-1; j++)
                for(int k = 1; k < nx-1; k++){

                    size_t index = convert_to_1D_index(i, j, k, nz, ny, nx);

                    size_t index_xm1 = convert_to_1D_index(i  , j  , k-1, nz, ny, nx);
                    size_t index_xp1 = convert_to_1D_index(i  , j  , k+1, nz, ny, nx);
                    size_t index_ym1 = convert_to_1D_index(i  , j-1, k  , nz, ny, nx);
                    size_t index_yp1 = convert_to_1D_index(i  , j+1, k  , nz, ny, nx);
                    size_t index_zm1 = convert_to_1D_index(i-1, j  , k  , nz, ny, nx);
                    size_t index_zp1 = convert_to_1D_index(i+1, j  , k  , nz, ny, nx);


                    const float coeff_x = dt * alpha[index] / (dx*dx);
                    const float coeff_y = dt * alpha[index] / (dy*dy);
                    const float coeff_z = dt * alpha[index] / (dz*dz);

                    out[index] = in[index]
                                    + coeff_x * (in[index_xm1] - 2*in[index] +in[index_xp1])
                                    + coeff_y * (in[index_ym1] - 2*in[index] +in[index_yp1])
                                    + coeff_z * (in[index_zm1] - 2*in[index] +in[index_zp1]);

                }


        //swap the fields around for next iteration
        tmp = in;
        in  = out;
        out = in;
    }

    //free out, return in (due to swap)
    deallocate((void**)&out);
    return in;

}

