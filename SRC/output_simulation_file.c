/**
 * @file output_simulation_file.c
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu) 
 * @author Peter Jamieson (Jamiespa@miamioh.edu) 
 * @brief Writes data describing a Boids simulation to a file.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "types.h"

FILE* init_output_file_for_simulation(char *filename, int num_boids, int num_frames, parameters_s *parameters, int dim, int num_objs, int num_ATONs)
{
	FILE *file_p;

	file_p = (FILE*)fopen(filename, "w");

	if (file_p == NULL)
	{
		printf("Error opening file %s", filename);
	}

	fprintf(file_p, "%d\n", num_boids);  //0
	fprintf(file_p, "%d\n", num_frames); //1
	fprintf(file_p, "%d\n", parameters->dimension_size); //2
	fprintf(file_p, "%d\n", dim); //3
	fprintf(file_p, "%d\n", parameters->neighbour_distance); //4
	fprintf(file_p, "%d\n", parameters->neighbour_desired_seperation); //5
	fprintf(file_p, "%f\n", parameters->weight_rule1); //6
	fprintf(file_p, "%f\n", parameters->weight_rule2); //7
	fprintf(file_p, "%f\n", parameters->weight_rule3); //8
	fprintf(file_p, "%d\n", num_objs); //9
	fprintf(file_p, "%d\n", num_ATONs);


	return file_p;
}

void write_frame_to_output_file(FILE *f_p, boids_s *boids_p)
{
	int i;

	for (i = 0; i < boids_p->num_boids; i++)
	{
		fprintf(f_p, "%f\n", boids_p->the_boids[i]->position->x);
		fprintf(f_p, "%f\n", boids_p->the_boids[i]->position->y);
#ifdef DIM_3D
		fprintf(f_p, "%f\n", boids_p->the_boids[i]->position->z);
#endif
		fprintf(f_p, "%f\n", boids_p->the_boids[i]->velocity->x); // DO we need the speed recorded...gives direction
		fprintf(f_p, "%f\n", boids_p->the_boids[i]->velocity->y);
#ifdef DIM_3D
		fprintf(f_p, "%f\n", boids_p->the_boids[i]->velocity->z);
#endif
		fprintf(f_p, "%d\n", boids_p->the_boids[i]->life_status);
		fprintf(f_p, "%d\n", boids_p->the_boids[i]->is_leader);
	}
	fflush(f_p);
}

void write_objects_to_file(FILE *f_p, objs_s *objs_p)
{
	for (int i = 0; i < objs_p->num_objs; i++) 
	{
		fprintf(f_p, "%f\n", objs_p->the_objs[i]->position->x);
		fprintf(f_p, "%f\n", objs_p->the_objs[i]->position->y);
		fprintf(f_p, "%d\n", objs_p->the_objs[i]->radius);
		fprintf(f_p, "%d\n", objs_p->the_objs[i]->is_waypoint);
	}
}

void write_atons_to_file(FILE *f_p, boids_s *CMs, boids_s *IDMs) {
	for (int i = 0; i < CMs->num_boids; i++) {
		fprintf(f_p, "%f\n", CMs->the_boids[i]->position->x);
		fprintf(f_p, "%f\n", CMs->the_boids[i]->position->y);
		fprintf(f_p, "%f\n", CMs->the_boids[i]->velocity->x);
		fprintf(f_p, "%f\n", CMs->the_boids[i]->velocity->y);
	}
	for (int i = 0; i < IDMs->num_boids; i++) {
		fprintf(f_p, "%f\n", IDMs->the_boids[i]->position->x);
		fprintf(f_p, "%f\n", IDMs->the_boids[i]->position->y);
		fprintf(f_p, "%f\n", IDMs->the_boids[i]->velocity->x);
		fprintf(f_p, "%f\n", IDMs->the_boids[i]->velocity->y);
	}
}
