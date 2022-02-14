#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "vector.h"

/*
 * Allocate a vector
 */
vector_s *allocate_vector()
{
	vector_s *vec;

	vec = (vector_s*)malloc(sizeof(vector_s));
	vec->x = 0;
	vec->y = 0;
#ifdef DIM_3D
	vec->z = 0;
#endif

	return vec;
}

/*
 * Free a vector
 */
void free_vector(vector_s *vec)
{
	if (vec == NULL)
	{
		printf("Error freeing vector\n");
		exit(-1);
	}
	free(vec);
}

void init_vector_to_0(vector_s *to)
{
	to->x = 0;
	to->y = 0;
#ifdef DIM_3D
	to->z = 0;
#endif
}

void rand_positive_vector(vector_s *to, int max)
{
	to->x = rand()%max;
	to->y = rand()%max;
#ifdef DIM_3D
	to->z = rand()%max;
#endif
}

void rand_pos_or_neg_vector(vector_s *to, int max)
{
	to->x = (rand()%2 == 0) ? rand()%max : -1*rand()%max;
	to->y = (rand()%2 == 0) ? rand()%max : -1*rand()%max;
#ifdef DIM_3D
	to->z = (rand()%2 == 0) ? rand()%max : -1*rand()%max;
#endif
}

void copy_vector(vector_s *from, vector_s *to)
{
	to->x = from->x;
	to->y = from->y;
#ifdef DIM_3D
	to->z = from->z;
#endif
}

void add_vector(vector_s *to, vector_s *with)
{
	to->x = to->x + with->x;
	to->y = to->y + with->y;
#ifdef DIM_3D
	to->z = to->z + with->z;
#endif
}

void add_vector_new(vector_s *to, vector_s *with_a, vector_s *with_b)
{
	to->x = with_a->x + with_b->x;
	to->y = with_a->y + with_b->y;
#ifdef DIM_3D
	to->z = with_a->z + with_b->z;
#endif
}

void sub_vector(vector_s *to, vector_s *with)
{
	to->x = to->x - with->x;
	to->y = to->y - with->y;
#ifdef DIM_3D
	to->z = to->z - with->z;
#endif
}

void sub_vector_new(vector_s *to, vector_s *with_a, vector_s *with_b)
{
	to->x = with_a->x - with_b->x;
	to->y = with_a->y - with_b->y;
#ifdef DIM_3D
	to->z = with_a->z - with_b->z;
#endif
}

float length_vector(vector_s *a)
{
	float total;

#ifdef DIM_2D
	total = sqrt(a->x * a->x + a->y * a->y); 
#else
	total = sqrt(a->x * a->x + a->y * a->y + a->z * a->z); 
#endif

	return total;
}

float distance_between_vectors(vector_s *a, vector_s *b)
{
	float total;
	vector_s sub;

	sub_vector_new(&sub, a, b);
	total = length_vector(&sub);

	return total;
}

void normalize_vector(vector_s *to)
{
	float length = length_vector(to);

	if (length != 0)
	{
		to->x = to->x / length;
		to->y = to->y / length;
#ifdef DIM_3D
		to->z = to->z / length;
#endif
	}
}

void wrap_dimensions(vector_s *vec, int dimension_size)
{
#ifdef WRAP_TRUE
	if (vec->x > dimension_size)
	{
		vec->x -= dimension_size;
	}
	else if (vec->x < 0)
	{
		vec->x += dimension_size;
	}

	if (vec->y > dimension_size)
	{
		vec->y -= dimension_size;
	}
	else if (vec->y < 0)
	{
		vec->y += dimension_size;
	}
#ifdef DIM_3D
	if (vec->z > dimension_size)
	{
		vec->z -= dimension_size;
	}
	else if (vec->z < 0)
	{
		vec->z += dimension_size;
	}
#endif
#else 
	if (vec->x > dimension_size)
	{
		vec->x = dimension_size;
	}
	else if (vec->x < 0)
	{
		vec->x = 0;
	}

	if (vec->y > dimension_size)
	{
		vec->y = dimension_size;
	}
	else if (vec->y < 0)
	{
		vec->y = 0;
	}
#ifdef DIM_3D
	if (vec->z > dimension_size)
	{
		vec->z = dimension_size;
	}
	else if (vec->z < 0)
	{
		vec->z = 0;
	}
#endif
#endif
}

void multiply_vector_by_scalar(vector_s *vec, float mult_by)
{
	vec->x = vec->x * mult_by;
	vec->y = vec->y * mult_by;
#ifdef DIM_3D
	vec->z = vec->z * mult_by;
#endif
}


void divide_vector_by_scalar(vector_s *vec, float div_by)
{
	if (div_by == 0)
		return;

	vec->x = vec->x / div_by;
	vec->y = vec->y / div_by;
#ifdef DIM_3D
	vec->z = vec->z / div_by;
#endif
}

void print_vector(vector_s *vec)
{
#ifdef DIM_3D
	printf("Vector: (%f, %f, %f)\n", vec->x, vec->y, vec->z);
#else
	printf("Vector: (%f, %f)\n", vec->x, vec->y);
#endif
}
