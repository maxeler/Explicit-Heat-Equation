/*
 * heat_equation.c
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "heat_equation.h"
#include "memory.h"
#include "cpu_compute.h"
#include "dfe_compute.h"

#define ALPHA_DEFAULT 1.77;
#define DX 1.0;
#define DY 1.0;
#define DZ 1.0;

void print_help();

void set_options(int argc, char **argv, params* settings){


    settings->exit = 0;
    if(argc !=2){
        print_help();
        settings->exit = 1;
        return;
    }

    char* fname = argv[1];

    FILE *fp = fopen(fname, "r");
    if(!fp){
        printf("ERROR: Cannot find parameter file %s!\n", fname);
        settings->exit = 1;
        return;
    }


    //Read parm file

    //NX
    if(fscanf(fp, "%d\n", &settings->x) != 1){
        printf("ERROR: Unable to read the value of NX in parameter file.\n");
        settings->exit = 1;
    }

    //NY
    if(fscanf(fp, "%d\n", &settings->y) != 1){
        printf("ERROR: Unable to read the value of NY in parameter file.\n");
        settings->exit = 1;
    }

    //NZ
    if(fscanf(fp, "%d\n", &settings->z) != 1){
        printf("ERROR: Unable to read the value of NZ in parameter file.\n");
        settings->exit = 1;
    }

    //Number of iteration
    if(fscanf(fp, "%d\n", &settings->n_iterations) != 1){
        printf("ERROR: Unable to read the value of Num of iterations in parameter file.\n");
        settings->exit = 1;
    }

    //DT
    if(fscanf(fp, "%f\n", &settings->dt) != 1){
        printf("ERROR: Unable to read the value of DT in parameter file.\n");
        settings->exit = 1;
    }

    //DX
    if(fscanf(fp, "%f\n", &settings->dx) != 1){
        printf("ERROR: Unable to read the value of DX in parameter file.\n");
        settings->exit = 1;
    }


    //DY
    if(fscanf(fp, "%f\n", &settings->dy) != 1){
        printf("ERROR: Unable to read the value of DY in parameter file.\n");
        settings->exit = 1;
    }


    //DZ
    if(fscanf(fp, "%f\n", &settings->dz) != 1){
        printf("ERROR: Unable to read the value of DZ in parameter file.\n");
        settings->exit = 1;
    }


    //input
    if(fscanf(fp, "%d\n", &settings->has_input_file) != 1){
        printf("ERROR: Unable to read the value of input flag in parameter file.\n");
        settings->exit = 1;
    }

    //Read input file if it is expecting one.
    if(settings->has_input_file){
        if(fscanf(fp, "%s\n", settings->input_path) != 1){
            printf("ERROR: Unable to read the input file path in parameter file.\n");
            settings->exit = 1;
        }
    }

    //model
    if(fscanf(fp, "%d\n", &settings->has_model_file) != 1){
        printf("ERROR: Unable to read the value of model in parameter file.\n");
        settings->exit = 1;
    }

    //Read model value if it is expecting one.
    if(settings->has_model_file == 1){
        if(fscanf(fp, "%f\n", &settings->alpha_value) != 1){
            printf("ERROR: Unable to read the model value in parameter file.\n");
            settings->exit = 1;
        }
    }


    //Read model file if it is expecting one.
    if(settings->has_model_file == 2){
        if(fscanf(fp, "%s\n", settings->model_path) != 1){
            printf("ERROR: Unable to read the model file path in parameter file.\n");
            settings->exit = 1;
        }
    }

}

void print_help(){
    printf("USAGE:\n");
    printf("\tImplicitHeatEquation <path to parameters file>\n");
    printf("Note: Parameter file can be generated by generate_parmfile.py\n");
}



void initialise(params* settings){
    size_t total_size = settings->x * settings->y * settings->z;

    settings->input  = allocate(sizeof(*settings->input), total_size);
    settings->alpha  = allocate(sizeof(*settings->alpha), total_size);



    /* READ IN MODEL */
    FILE* fp = NULL;

    if(settings->has_model_file == 2){
        fp = fopen(settings->model_path, "r");
        if(!fp){
            printf("Model file not found. Setting alpha to default...\n");
            settings->has_model_file = 0;
        }
    }

    if(!fp){
        for(size_t i = 0; i < total_size; i++){
            settings->alpha[i] = settings->alpha_value;
        }
    }
    else{
        size_t members_read = fread(settings->alpha, sizeof(*settings->alpha), total_size, fp);
        if(total_size != members_read){
            printf("ERROR: model file does not contain enough members for this problem size (%ld read, expected %ld)\n",
                    members_read, total_size);
            exit(-1);
        }
        fclose(fp);
    }

    /* READ IN INPUT */

    if(settings->has_input_file){

        fp = fopen(settings->input_path, "r");

        size_t members_read = fread(settings->input, sizeof(*settings->input), total_size, fp);
        if(total_size != members_read){
            printf("ERROR: input file does not contain enough members for this problem size (%ld read, expected %ld)\n",
                    members_read, total_size);
            exit(-1);
        }
        fclose(fp);
    }
    else{
        /* SET DEFAULT INITIAL CONDITION */
        for( int i = settings->z/2-1; i < settings->z/2+1; i++)
            for( int j = settings->y/2-1; j < settings->y/2+1; j++)
                for( int k = settings->x/2-1; k < settings->x/2+1; k++){
                    size_t index = convert_to_1D_index(i, j, k, settings->z, settings->y, settings->x);
                    settings->input[index] = 12.0;
                }

    }





    initialise_dfe(settings);
}

int verify_results(float* golden, float* res, params *settings){

    float maximum_error = 0.0;
    float relative_error = 0.0;

    float sum_error = 0.0;

    float max_value = 0;
    float min_value = INFINITY;

    int pass = 1;

    for( int i = 0; i < settings->z; i++)
        for( int j = 0; j < settings->y; j++)
            for( int k = 0; k < settings->x; k++){
                size_t index = convert_to_1D_index(i, j, k, settings->z, settings->y, settings->x);
                float err = fabs(golden[index] - res[index]);

                sum_error += err;

                if(err > settings->threshold){
                    printf("[%d, %d, %d] Actual %g\tExpected %g\tError%g\n", i, j, k, res[index], golden[index], err);
                    pass = 0;
                }

                if(err > maximum_error){
                    maximum_error = err;
                    relative_error = err/golden[index];
                }

                if(res[index] > max_value){
                    max_value = res[index];
                }

                if(res[index] < min_value){
                    min_value = res[index];
                }

            }

    printf("Total error   : %g\n", sum_error);
    printf("Average error : %g\n", sum_error/(settings->z*settings->y*settings->x));
    printf("Maximum error : %g\n", maximum_error);
    printf("Relative error: %g\n", relative_error);
    printf("max value : %g\n", max_value);
    printf("min value: %g\n", min_value);

    return pass;
}


void print_settings(params *settings){
    printf("Explicit Heat Equation - Configuration\n");
    printf("======================================\n");
    printf("\t Dimensions: %d x %d x %d\n", settings->x, settings->y, settings->z);
    printf("\t Number of iterations: %d\n", settings->n_iterations);
    printf("\t dt: %g\n", settings->dt);
    printf("\t dx: %g\n", settings->dx);
    printf("\t dy: %g\n", settings->dy);
    printf("\t dz: %g\n", settings->dz);
    if(settings->has_input_file)
        printf("\t Using input file: %s\n", settings->input_path);
    else
        printf("\t Using default input configurations\n");
    switch(settings->has_model_file){
        case 0:
            printf("\t Using default alpha value : %g\n", settings->alpha_value);
            break;
        case 1:
            printf("\t Using constant alpha value : %g\n", settings->alpha_value);
            break;
        case 2:
            printf("\t Using model file: %s\n", settings->model_path);
    }
}

params* create_settings(){
    //create and initialise with default settings
    params *settings = (params *) allocate(sizeof(*settings), 1);
    settings->dt = 0.1;
    settings->dx = DX;
    settings->dy = DY;
    settings->dz = DZ;
    settings->alpha_value = ALPHA_DEFAULT;
    settings->threshold = 1e-5;

    return settings;
}

int main(int argc, char **argv){
    params* settings = create_settings();

    set_options(argc, argv, settings);


	if(settings->exit){
	    exit(-1);
	}

	initialise(settings);

    print_settings(settings);

	printf("Computing on the CPU...\n");
	float* result_cpu = compute_cpu(settings);
	printf("Done.\n");

	printf("Computing on the DFE...\n");
	float* result_dfe = compute_dfe(settings);
	printf("Done.\n");

	int pass = verify_results(result_cpu, result_dfe, settings);

	if(pass){
	    printf("============PASS============");
	}
	else{
        printf("============FAIL============");
	}
	cleanup_dfe(settings);

	deallocate((void**)&settings->dfe_handle);
	deallocate((void**)&result_dfe);
	deallocate((void**)&result_cpu);
	deallocate((void**)&settings->input);
	deallocate((void**)&settings->alpha);
	deallocate((void**)&settings);

	return !pass;
}
