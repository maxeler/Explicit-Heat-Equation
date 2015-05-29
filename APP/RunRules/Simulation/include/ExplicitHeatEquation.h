/**\file */
#ifndef SLIC_DECLARATIONS_ExplicitHeatEquation_H
#define SLIC_DECLARATIONS_ExplicitHeatEquation_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ExplicitHeatEquation_PCIE_ALIGNMENT (16)
/**
 * \brief Initialise a maxfile.
 */
max_file_t* ExplicitHeatEquation_init(void);

/* Error handling functions */
int ExplicitHeatEquation_has_errors(void);
const char* ExplicitHeatEquation_get_errors(void);
void ExplicitHeatEquation_clear_errors(void);
/* Free statically allocated maxfile data */
void ExplicitHeatEquation_free(void);
/* returns: -1 = error running command; 0 = no error reported */
int ExplicitHeatEquation_simulator_start(void);
/* returns: -1 = error running command; 0 = no error reported */
int ExplicitHeatEquation_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_ExplicitHeatEquation_H */

