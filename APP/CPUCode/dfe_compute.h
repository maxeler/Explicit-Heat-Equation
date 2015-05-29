/*
 * dfe_compute.h
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#ifndef DFE_COMPUTE_H_
#define DFE_COMPUTE_H_

#include "heat_equation.h"



float* compute_dfe(params *settings);
void  initialise_dfe(params *settings);
void  cleanup_dfe(params *settings);

#endif /* DFE_COMPUTE_H_ */
