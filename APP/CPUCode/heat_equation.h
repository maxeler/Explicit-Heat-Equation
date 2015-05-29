/*
 * heat_equation.h
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#ifndef HEAT_EQUATION_H_
#define HEAT_EQUATION_H_

typedef struct dfe_handle dfe_handle_t;

typedef struct params{
    int x;
    int y;
    int z;
    int n_iterations;
    float threshold;
    float alpha_value;

    int has_input_file;
    int has_model_file;
    char* model_path;
    char* input_path;

    float* input;
    float* alpha;
    float dt;
    float dx;
    float dy;
    float dz;

    int exit;

    dfe_handle_t *dfe_handle;


} params;

#endif /* HEAT_EQUATION_H_ */
