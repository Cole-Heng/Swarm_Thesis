#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "vector.h"
#include "objs_struct.h"

objs_s *init_random_objects(int num_objs, int dimension_size, int radius)
{
	int i;
	vector_s speed_vector;
	vector_s position_vector;

	objs_s *objs = allocate_objects(num_objs);

	for (i = 0; i < num_objs; i++)
	{
		/* initialize location */
		rand_positive_vector(&position_vector, dimension_size);
		copy_vector(&position_vector, objs->the_objs[i]->position);

		/* initalize speed assuming 1 */
		init_vector_to_0(&speed_vector);
		copy_vector(&speed_vector, objs->the_objs[i]->velocity);

        /* Set object radius */
        objs->the_objs[i]->r = radius;
	}

	return objs;
}

objs_s *init_objects_with_file(int *num_objs, int *num_frames, parameters_s* parameters, FILE *fp)
{
	int i, j;
	float read[6];
	char string_numbers[8];

	fgets(string_numbers, 8, fp);
	if (string_numbers != NULL)
	{
		*num_objs = atoi(string_numbers);
	}
	else
	{
		printf("Failed to read init file - num_objs\n");
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

	/* allocate the objs */
	objs_s *objs = allocate_objects(*num_objs);

	for (i = 0; i < *num_objs; i++)
	{
#ifdef DIM_3D
		/* assume next 6 values are for position then velocity */
		for (j = 0; j < 6; j++)
		{
			fscanf(fp, "%f", &read[j]);
		}

		set_object_position_xyz(objs->the_objs[i], read[0], read[1], read[2]);
		set_object_velocity_xyz(objs->the_objs[i], read[3], read[4], read[5]);
#else
		/* assume next 4 values are for position then velocity */
		for (j = 0; j < 4; j++)
		{
			fscanf(fp, "%f", &read[j]);
		}

		set_object_position_xy(objs->the_objs[i], read[0], read[1]);
		set_object_velocity_xy(objs->the_objs[i], read[2], read[3]);
#endif
	}

	return objs;
}

/*
 * Allocate the objs
 */
objs_s *allocate_objects(int num_objs)
{
	objs_s *objs;
	int i;

	objs = (objs_s*)malloc(sizeof(objs_s));
	objs->the_objs = (obj_s**)malloc(sizeof(obj_s*)*num_objs);
	objs->num_objs = num_objs;

	for (i = 0; i < num_objs; i++)
	{
		objs->the_objs[i] = allocate_object();
	}

	return objs;
}

/*
 * Free
 */
void free_objectss(objs_s *objs)
{
	int i;

	if (objs == NULL)
	{
		printf("Error freeing objs\n");
		exit(-1);
	}

	for (i = 0; i < objs->num_objs; i++)
	{
		free_object(objs->the_objs[i]);
	}
	free(objs->the_objs);

	free(objs);
}

#ifdef DIM_3D
void set_object_velocity_xyz(obj_s *obj, float x, float y, float z)
{
	obj->velocity->x = x;
	obj->velocity->y = y;
	obj->velocity->z = z;
}

void set_object_position_xyz(obj_s *obj, float x, float y, float z)
{
	obj->position->x = x;
	obj->position->y = y;
	obj->position->z = z;
}
#else
void set_object_velocity_xy(obj_s *obj, float x, float y)
{
	obj->velocity->x = x;
	obj->velocity->y = y;
}

void set_object_position_xy(obj_s *obj, float x, float y)
{
	obj->position->x = x;
	obj->position->y = y;
}
#endif 

/*
 * Allocate the obj
 */
obj_s *allocate_object()
{
	obj_s *obj;

	obj = (obj_s*)malloc(sizeof(obj_s));
	obj->position = allocate_vector();
	obj->velocity = allocate_vector();
	return obj;
}

/*
 * Free
 */
void free_object(obj_s *obj)
{
	if (obj == NULL)
	{
		printf("Error freeing obj\n");
		exit(-1);
	}
	free_vector(obj->position);
	free_vector(obj->velocity);

	free(obj);
}

void copy_object(obj_s *from, obj_s *to)
{
	copy_vector(from->position, to->position);
	copy_vector(from->velocity, to->velocity);
}

void sum_all_objectss_position(objs_s *objs_p, vector_s *sum)
{
	int i;

	init_vector_to_0(sum);
	for (i = 0; i < objs_p->num_objs; i++)
	{
		add_vector(sum, objs_p->the_objs[i]->position);
	}
}
void sum_all_objects_velocity(objs_s *objs_p, vector_s *sum)
{
	int i;

	init_vector_to_0(sum);
	for (i = 0; i < objs_p->num_objs; i++)
	{
		add_vector(sum, objs_p->the_objs[i]->velocity);
	}
}
