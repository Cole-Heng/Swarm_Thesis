/**
 * @file boids.c
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu)
 * @brief Main code to run a Boids simulation.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 * This is Peter Jamieson's attempt at implementing a C version of Boid's algorithm
 * - Started = Dec 7th, 2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "boids_struct.h"
#include "output_simulation_file.h"
#include "simulate.h"
#include "vector.h"
#include "objs_struct.h"


int main(int argc, char *argv[])
{
	int num_boids;
	int num_frames;
	int num_objects;
	char *out_filename;
	boids_s *boids_p;
	objs_s *objs_p;
	parameters_s parameters;
	FILE *file_p;
	int i;
	int RNG_seed;

	/* Default Parameter settings. 
	 * neighbour are distances for neighbour calc 
	 * wights are float that we divide vector by */
	parameters.dimension_size = 1000; // The size, N, of the arena, an NxN square
	parameters.neighbour_distance = 50; // Radius that a boid can see around it
	parameters.neighbour_desired_seperation = 25; // Desired distance to stay away from neighbors
	parameters.weight_rule1 = 75; //Center of mass
	parameters.weight_rule2 = 4; //Avoid others
	parameters.weight_rule3 = 10; //Common velocity
	parameters.boid_size_radius = 5; // The physical size of a boid
	parameters.object_radius = 5; // The physical size of an object

	/* read arguments for the size of the boids to implement, how long to simulate,
	 * and the file to ouput the simulation too:
	 *
	 * RAND_INIT
	 * {num_boids} = integer of boids 
	 * {num_frames} 
	 * {file_to_output_to} 
	 *
	 * FILE_INIT {file_name}
	 * {file_to_output_to}
	 * */
	if (argc > 1)
	{
		if (strcmp(argv[1], "RAND_INIT") == 0)
		{
			if (argc == 8)
			{
				printf("You chose | %s | %s | %s | %s | %s | %s | %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
				num_boids = atoi(argv[2]);
				num_frames = atoi(argv[3]);
				parameters.dimension_size = atoi(argv[4]);
				num_objects = atoi(argv[5]);
				out_filename = argv[6];
				RNG_seed = atoi(argv[7]);
			}
			else
			{
				printf("Error running - boids RAND_INIT num_boids (TICK num_ticks || STEADY_STATE) file_to_output_to\n");
				return -1;
			}
			/* Seed random */
			if (RNG_seed < 0) {
				srand(time(NULL));
			} else {
				srand(RNG_seed);
			}

			/* initialize the data structures for the boids */
			boids_p = init_random_boids(num_boids, parameters.dimension_size);
			objs_p = init_random_objects(num_objects, parameters.dimension_size, parameters.object_radius);
		}
		else if (strcmp(argv[1], "FILE_INIT") == 0)
		{
			/* FILE_INIT {file_name}
			 * {file_to_output_to} */
			if (argc == 5)
			{
				FILE *init_file;

				printf("You chose | %s | %s | %s | %s\n", argv[1], argv[2], argv[3], argv[4]);

				/* open the file to be read for initialization */
				init_file = fopen(argv[2], "r");
				if (init_file == NULL)
				{
					printf("Can't open file %s\n", argv[2]);
					exit(-1);
				}
				RNG_seed = atoi(argv[4]);
				/* Seed random */
				if (RNG_seed < 0) {
					srand(time(NULL));
				} else {
					srand(RNG_seed);
				}

				boids_p = init_boids_with_file(&num_boids, &num_frames, &parameters, init_file);
				objs_p = init_objects_with_file(&num_objects, init_file);

				fclose(init_file);

				/* setup the output filename */
				out_filename = argv[3];
			}
			else
			{
				printf("Error running - boids FILE_INIT file_name file_to_output_to\n");
				return -1;
			}
		} 
		else
		{
			printf("Error running - boids FILE_INIT file_name file_to_output_to\n");
			printf("OR - boids RAND_INIT num_boids num_frames file_to_output_to\n");
		}
	}

	/* initialize data structures for simulation */
	init_simulate(boids_p, &parameters, objs_p);
	boids_s *CMs = get_CMs_pointer();
	boids_s *IDMs = get_IDMs_pointer();

	int num_ATONs = CMs->num_boids + IDMs->num_boids;
	/* initialize the output simulation file */
	file_p = init_output_file_for_simulation(out_filename, num_boids, num_frames+1, &parameters, 2, num_objects, num_ATONs);

	/* Write objects to the file header */
	write_objects_to_file(file_p, objs_p);

	/* Write Aids TO Navigation to the file header */
	write_atons_to_file(file_p, CMs, IDMs);

	/* write initial state to the file */
	write_frame_to_output_file(file_p, boids_p);

	// TIMING
	clock_t start = clock();
	clock_t startw, stopw;
	double timew = 0;

	/* execute a tick of simulation */
	for (i = 0; i < num_frames; i++)
	{
		/* simulate the tick for Boids */
		simulate_a_frame(boids_p, &parameters, objs_p, i);

		startw = clock();
		/* write to the file */
		write_frame_to_output_file(file_p, boids_p);
		stopw = clock();
		timew += (double)(stopw - startw) * 1000.0 / CLOCKS_PER_SEC;
	}

	

	// TIMING
	clock_t stop = clock();
	double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("Time elapsed in ms: %f\n", elapsed);
	printf("Time write elapsed in ms: %f\n", timew);
	output_simulation_final_stats(boids_p, parameters.dimension_size, elapsed);

	/* cleanup */
	free_simulate();
	free_boids(boids_p);

	fflush(file_p);
	/* close output file */
	fclose(file_p);

	fflush(stdout);
	return 0;
}
