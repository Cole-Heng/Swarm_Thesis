#include <stdlib.h>
#include <stdio.h>
#include "types.h"

FILE* init_output_file_for_simulation(char *filename, int num_boids, int num_frames, parameters_s *parameters, int dim)
{
	FILE *file_p;

	file_p = (FILE*)fopen(filename, "w");

	if (file_p == NULL)
	{
		printf("Error opening file %s", filename);
	}

	fprintf(file_p, "%d\n", num_boids);
	fprintf(file_p, "%d\n", num_frames);
	fprintf(file_p, "%d\n", parameters->dimension_size);
	fprintf(file_p, "%d\n", dim);
	fprintf(file_p, "%d\n", parameters->neighbour_distance);
	fprintf(file_p, "%d\n", parameters->neighbour_desired_seperation);
	fprintf(file_p, "%f\n", parameters->weight_rule1);
	fprintf(file_p, "%f\n", parameters->weight_rule2);
	fprintf(file_p, "%f\n", parameters->weight_rule3);

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
	}
}
