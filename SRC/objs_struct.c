/**
 * @file objs_struct.h
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu) 
 * @brief Modifies the fields of an Object structure.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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
        set_object_radius(objs->the_objs[i], radius);
		set_object_waypoint(objs->the_objs[i], FALSE);
	}

	return objs;
}

objs_s *init_objects_with_file(int *num_objs, FILE *fp)
{
	int i, j;
	float read[4];
	char string_numbers[8];

	fgets(string_numbers, 8, fp); // Skip first read, it always gives "0"

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


	/* allocate the objs */
	objs_s *objs = allocate_objects(*num_objs);

	for (i = 0; i < *num_objs; i++)
	{
		/* assume next 4 values are for position x,y, radius, is_waypoint*/
		for (j = 0; j < 4; j++)
		{
			fscanf(fp, "%f", &read[j]);
		}

		set_object_position_xy(objs->the_objs[i], read[0], read[1]);
		set_object_velocity_xy(objs->the_objs[i], 0, 0);
		set_object_radius(objs->the_objs[i], read[2]);
		set_object_waypoint(objs->the_objs[i], read[3]);
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
void set_object_radius(obj_s *obj, int radius)
{
	obj->radius = radius;
}

void set_object_waypoint(obj_s *obj, short is_waypoint)
{
	obj->is_waypoint = is_waypoint;
}


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
