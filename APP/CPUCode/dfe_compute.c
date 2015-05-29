/*
 * dfe_compute.c
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#include "dfe_compute.h"
#include "memory.h"
#include <maxlibfd.h>

struct dfe_handle{
    maxlib_context maxlib;
    maxlib_lmem_array lmem_wave_in;
    maxlib_lmem_array lmem_wave_out;
    maxlib_lmem_array lmem_model;

};

float* compute_dfe(struct params *settings){
    struct dfe_handle *handle = settings->dfe_handle;

    size_t total_size = settings->x * settings->y * settings->z;
    float* result     = allocate(sizeof(*result), total_size);

    float inv_dx2 = settings->dt / (settings->dx * settings->dx);
    float inv_dy2 = settings->dt / (settings->dy * settings->dy);
    float inv_dz2 = settings->dt / (settings->dz * settings->dz);

    for (int t = 0; t < settings->n_iterations; t++){
        maxlib_stream_from_lmem(handle->maxlib, "in_T" , handle->lmem_wave_in);
        maxlib_stream_to_lmem  (handle->maxlib, "out_T", handle->lmem_wave_out);

        maxlib_stream_earthmodel_from_lmem(handle->maxlib, handle->lmem_model);

        maxlib_set_scalar(handle->maxlib, "inv_dx2", inv_dx2);
        maxlib_set_scalar(handle->maxlib, "inv_dy2", inv_dy2);
        maxlib_set_scalar(handle->maxlib, "inv_dz2", inv_dz2);

        maxlib_stream_region_to_host(handle->maxlib, "receiver", t==settings->n_iterations-1? result : NULL,
                0, 0, 0, settings->x, settings->y, settings->z);

        maxlib_run(handle->maxlib);

        maxlib_lmem_array tmp;
        tmp = handle->lmem_wave_in;
        handle->lmem_wave_in  = handle->lmem_wave_out;
        handle->lmem_wave_out = tmp;

    }

    return result;


}

void initialise_dfe(struct params *settings){
    struct dfe_handle *handle = allocate(sizeof(struct dfe_handle), 1);
    handle->maxlib = maxlib_open(settings->x, settings->y, settings->z);
    maxlib_print_status(handle->maxlib);

    handle->lmem_wave_in  = maxlib_lmem_alloc_wavefield (handle->maxlib);
    handle->lmem_wave_out = maxlib_lmem_alloc_wavefield (handle->maxlib);
    handle->lmem_model    = maxlib_lmem_alloc_earthmodel(handle->maxlib);

    maxlib_lmem_load_wavefield(handle->maxlib, handle->lmem_wave_in, settings->input);
    maxlib_lmem_load_wavefield_with_zeros(handle->maxlib, handle->lmem_wave_out);

    maxlib_earthmodel em =  maxlib_earthmodel_create_in_memory(settings->x, settings->y, settings->z);
    maxlib_earthmodel_set_data(em, "alpha", settings->alpha);
    maxlib_lmem_load_earthmodel(handle->maxlib, handle->lmem_model, em);
    maxlib_earthmodel_release(em);

    settings->dfe_handle = handle;
}

void cleanup_dfe(struct params *settings){
    maxlib_close(settings->dfe_handle->maxlib);

}
