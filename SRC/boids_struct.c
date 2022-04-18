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
	float read[7];
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
		printf("Failed to read init file - dimension_size\n");
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
		printf("Failed to read init file - neighbor_distance\n");
		exit(-1);
	}

	fgets(string_numbers, 8, fp);
	if (string_numbers != NULL)
	{
		parameters->neighbour_desired_seperation = atoi(string_numbers);
	}
	else
	{
		printf("Failed to read init file - desired_seperation\n");
		exit(-1);
	}

	fscanf(fp, "%f", &parameters->weight_rule1);
	fscanf(fp, "%f", &parameters->weight_rule2);
	fscanf(fp, "%f", &parameters->weight_rule3);

	/* allocate the boids */
	boids_s *boids = allocate_boids(*num_boids);
	vector_s temp_vec;
	for (i = 0; i < *num_boids; i++)
	{
#ifdef DIM_3D
		/* assume next 7 values are for position then velocity then leader status*/
		for (j = 0; j < 7; j++)
		{
			fscanf(fp, "%f", &read[j]);
		}

		set_boid_position_xyz(boids->the_boids[i], read[0], read[1], read[2]);
		set_boid_velocity_xyz(boids->the_boids[i], read[3], read[4], read[5]);
		set_boid_leader(boids->the_boids[i], read[6]);
#else
		/* assume next 5 values are for position then velocity then leader status */
		for (j = 0; j < 5; j++)
		{
			fscanf(fp, "%f", &read[j]);
		}

		set_boid_position_xy(boids->the_boids[i], read[0], read[1]);
		if(read[2] == 0 && read[3] == 0)
		{
			rand_pos_or_neg_vector(&temp_vec, parameters->dimension_size);
			normalize_vector(&temp_vec); // all speed is 1 presently
			copy_vector(&temp_vec, boids->the_boids[i]->velocity);
		} else {
			set_boid_velocity_xy(boids->the_boids[i], read[2], read[3]);
		}
		if (read[4]) { // boid is a leader
			set_boid_leader(boids->the_boids[i], read[4]);
			assign_ghost_boid(boids->the_boids[i]);
		}
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

void set_boid_leader(iboid_s *boid, short is_leader) 
{
	if(boid->is_leader == FALSE && is_leader == TRUE){
		assign_ghost_boid(boid);
	}
	boid->is_leader = is_leader;
	//printf("moded boid #%d at p[%f, %f] leader: %d\n", boid->id, boid->position->x, boid->position->y, boid->is_leader);
}

void assign_ghost_boid(iboid_s *boid) 
{
	boid->ghost_boid = allocate_ghost();
	boid->ghost_boid->id = boid->id;
	copy_vector(boid->position, boid->ghost_boid->position);
	init_vector_to_0(boid->ghost_boid->velocity);
}

ghost_boid_s *allocate_ghost()
{
	ghost_boid_s *ghost;

	ghost = (ghost_boid_s*)malloc(sizeof(ghost_boid_s));
	ghost->position = allocate_vector();
	ghost->velocity = allocate_vector();
	return ghost;
}

/*
 * Allocate the boid
 */
iboid_s *allocate_boid()
{
	iboid_s *boid;

	boid = (iboid_s*)malloc(sizeof(iboid_s));
	boid->position = allocate_vector();
	boid->velocity = allocate_vector();
	boid->is_leader = FALSE;
	boid->mass = 0;
	#ifdef TRACK_DEATH
		boid->life_status = ALIVE;
	#endif
	boid->was_visited = FALSE;
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
	to->was_visited = from->was_visited;
	to->id = from->id;
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
