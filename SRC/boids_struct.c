#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "vector.h"
#include "boids_struct.h"

boids_s *init_random_boids(int num_boids, int dimension_size)
{
	int i;
	vector_s speed_vector;
	vector_s position_vector;

	boids_s *boids = allocate_boids(num_boids);

	for (i = 0; i < num_boids; i++)
	{
		/* initialize location */
		rand_positive_vector(&position_vector, dimension_size);
		copy_vector(&position_vector, boids->the_boids[i]->position);

		/* initalize speed assuming 1 */
		rand_pos_or_neg_vector(&speed_vector, dimension_size);
		normalize_vector(&speed_vector); // all speed is 1 presently
		copy_vector(&speed_vector, boids->the_boids[i]->velocity);
	}

	return boids;
}

boids_s *init_boids_with_file(int *num_boids, int *num_frames, parameters_s* parameters, FILE *fp)
{
	int i, j;
	float read[6];
	char string_numbers[8];

	fgets(string_numbers, 8, fp);
	if (string_numbers != NULL)
	{
		*num_boids = atoi(string_numbers);
	}
	else
	{
		printf("Failed to read init file - num_boids\n");
		exit(-1);
	}


	fgets(string_numbers, 8, fp);
	if (string_numbers != NULL)
	{
		*num_frames = atoi(string_numbers);
	}
	else
	{
		printf("Failed to read init file - num_frames\n");
		exit(-1);
	}

	fgets(string_numbers, 8, fp);
	if (string_numbers != NULL)
	{
		parameters->dimension_size = atoi(string_numbers);
	}
	else
	{
		printf("Failed to read init file - num_frames\n");
		exit(-1);
	}

	fgets(string_numbers, 8, fp); // skip dimension

	fgets(string_numbers, 8, fp);
	if (string_numbers != NULL)
	{
		parameters->neighbour_distance = atoi(string_numbers);
	}
	else
	{
		printf("Failed to read init file - num_frames\n");
		exit(-1);
	}

	fgets(string_numbers, 8, fp);
	if (string_numbers != NULL)
	{
		parameters->neighbour_desired_seperation = atoi(string_numbers);
	}
	else
	{
		printf("Failed to read init file - num_frames\n");
		exit(-1);
	}

	fscanf(fp, "%f", &parameters->weight_rule1);
	fscanf(fp, "%f", &parameters->weight_rule2);
	fscanf(fp, "%f", &parameters->weight_rule3);

	/* allocate the boids */
	boids_s *boids = allocate_boids(*num_boids);

	for (i = 0; i < *num_boids; i++)
	{
#ifdef DIM_3D
		/* assume next 6 values are for position then velocity */
		for (j = 0; j < 6; j++)
		{
			fscanf(fp, "%f", &read[j]);
		}

		set_boid_position_xyz(boids->the_boids[i], read[0], read[1], read[2]);
		set_boid_velocity_xyz(boids->the_boids[i], read[3], read[4], read[5]);
#else
		/* assume next 4 values are for position then velocity */
		for (j = 0; j < 4; j++)
		{
			fscanf(fp, "%f", &read[j]);
		}

		set_boid_position_xy(boids->the_boids[i], read[0], read[1]);
		set_boid_velocity_xy(boids->the_boids[i], read[2], read[3]);
#endif
	}

	return boids;
}

/*
 * Allocate the boids
 */
boids_s *allocate_boids(int num_boids)
{
	boids_s *boids;
	int i;

	boids = (boids_s*)malloc(sizeof(boids_s));
	boids->the_boids = (iboid_s**)malloc(sizeof(iboid_s*)*num_boids);
	boids->num_boids = num_boids;

	for (i = 0; i < num_boids; i++)
	{
		boids->the_boids[i] = allocate_boid();
		boids->the_boids[i]->id = i;
	}

	return boids;
}

/*
 * Free
 */
void free_boids(boids_s *boids)
{
	int i;

	if (boids == NULL)
	{
		printf("Error freeing boids\n");
		exit(-1);
	}

	for (i = 0; i < boids->num_boids; i++)
	{
		free_boid(boids->the_boids[i]);
	}
	free(boids->the_boids);

	free(boids);
}

#ifdef DIM_3D
void set_boid_velocity_xyz(iboid_s *boid, float x, float y, float z)
{
	boid->velocity->x = x;
	boid->velocity->y = y;
	boid->velocity->z = z;
}

void set_boid_position_xyz(iboid_s *boid, float x, float y, float z)
{
	boid->position->x = x;
	boid->position->y = y;
	boid->position->z = z;
}
#else
void set_boid_velocity_xy(iboid_s *boid, float x, float y)
{
	boid->velocity->x = x;
	boid->velocity->y = y;
}

void set_boid_position_xy(iboid_s *boid, float x, float y)
{
	boid->position->x = x;
	boid->position->y = y;
}
#endif 

/*
 * Allocate the boid
 */
iboid_s *allocate_boid()
{
	iboid_s *boid;

	boid = (iboid_s*)malloc(sizeof(iboid_s));
	boid->position = allocate_vector();
	boid->velocity = allocate_vector();
	boid->mass = 0;
	#ifdef TRACK_DEATH
		boid->life_status = ALIVE;
	#endif
	return boid;
}

/*
 * Free
 */
void free_boid(iboid_s *boid)
{
	if (boid == NULL)
	{
		printf("Error freeing boid\n");
		exit(-1);
	}
	free_vector(boid->position);
	free_vector(boid->velocity);

	free(boid);
}

void update_boid(iboid_s *boid, int dimension_size)
{
	add_vector(boid->position, boid->velocity);
	wrap_dimensions(boid->position, dimension_size);
}

void copy_boid(iboid_s *from, iboid_s *to)
{
	copy_vector(from->position, to->position);
	copy_vector(from->velocity, to->velocity);
	to->mass = from->mass;
}

void sum_all_boids_position(boids_s *boids_p, vector_s *sum)
{
	int i;

	init_vector_to_0(sum);
	for (i = 0; i < boids_p->num_boids; i++)
	{
		add_vector(sum, boids_p->the_boids[i]->position);
	}
}
void sum_all_boids_velocity(boids_s *boids_p, vector_s *sum)
{
	int i;

	init_vector_to_0(sum);
	for (i = 0; i < boids_p->num_boids; i++)
	{
		add_vector(sum, boids_p->the_boids[i]->velocity);
	}
}
