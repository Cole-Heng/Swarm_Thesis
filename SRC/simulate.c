#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "vector.h"
#include "simulate.h"
#include "boids_struct.h"

boids_s *neighbours_p;

// PROTOTYPES
void rule1(vector_s *vec, iboid_s *boid, boids_s *neighbours, float weight);
void rule2(vector_s *vec, iboid_s *boid, boids_s *neighbours, float desired_seperation, float weight);
void rule3(vector_s *vec, iboid_s *boid, boids_s *neighbours, float weight);

void init_simulate(boids_s* boids_p)
{
	/* allocate data structure for neighbours */
	neighbours_p = allocate_boids(boids_p->num_boids);
}
void free_simulate()
{
	/* allocate data structure for neighbours */
	free_boids(neighbours_p); 
}


/*
 * Rule 1: Boids try to fly towards the centre of mass of neighbouring boids.
 * Rule 2: Boids try to keep a small distance away from other objects (including other boids).
 * Rule 3: Boids try to match velocity with near boids.
 */
void simulate_a_frame(boids_s* boids_p, parameters_s* parameters, objs_s* objs_p)
{
	int i;

	vector_s velocity_change_from_rules[3];

	for (i = 0; i < boids_p->num_boids; i++)
	{
		#ifdef ENFORCE_DEATH
			if (boids_p->the_boids[i]->life_status == DEAD)
				continue;
		#endif
		/* find neighbours */
		find_neighbours(boids_p, boids_p->the_boids[i], parameters->neighbour_distance);

		/* Rule 1 - COHESION */
		rule1(&velocity_change_from_rules[0], boids_p->the_boids[i], neighbours_p, parameters->weight_rule1);

		/* Rule 2 - SEPERATE */
		rule2(&velocity_change_from_rules[1], boids_p->the_boids[i], neighbours_p, parameters->neighbour_desired_seperation, parameters->weight_rule2);

		/* Rule 3 - ALLIGN */
		rule3(&velocity_change_from_rules[2], boids_p->the_boids[i], neighbours_p, parameters->weight_rule3);

		/* add velocities for each rule */
		add_vector(boids_p->the_boids[i]->velocity, &velocity_change_from_rules[0]);
		add_vector(boids_p->the_boids[i]->velocity, &velocity_change_from_rules[1]);
		add_vector(boids_p->the_boids[i]->velocity, &velocity_change_from_rules[2]);

		/* normalize to keep to 1 or less */
		normalize_vector(boids_p->the_boids[i]->velocity);

		//printf("NEW VELOCITY boid[%d] - ", i); print_vector(boids_p->the_boids[i]->velocity);

		/* add the speed vector */
		update_boid(boids_p->the_boids[i], parameters->dimension_size);
		
		#ifdef TRACK_DEATH
		check_collision(boids_p->the_boids[i], neighbours_p, parameters, objs_p);
		#endif
	}


}

void rule1(vector_s *vec, iboid_s *boid, boids_s *neighbours, float weight)
{
	/* calculate the neighbours weight */
	sum_all_boids_position(neighbours, vec);
	/* remove yourself */
	sub_vector(vec, boid->position);
	/* remove yourself from the weighting */
	divide_vector_by_scalar(vec, neighbours->num_boids-1);

	/* want to go towards that point */
	sub_vector(vec, boid->position);

	/* make it a unit vector */
	normalize_vector(vec);
	/* weight factor */
	divide_vector_by_scalar(vec, weight);

	//printf("Rule 1 COHESION - "); print_vector(vec);
}

void rule2(vector_s *vec, iboid_s *boid, boids_s *neighbours, float desired_seperation, float weight)
{
	int i;

	init_vector_to_0(vec);

	if (desired_seperation != -1)
	{
		for (i = 0; i < neighbours->num_boids; i++)
		{
			float d = distance_between_vectors(boid->position, neighbours->the_boids[i]->position);
			if (d < desired_seperation)
			{
				/* boid - neighbour */
				vector_s away;
				sub_vector_new(&away, boid->position, neighbours->the_boids[i]->position);
	
				add_vector(vec, &away);
			}
		}
	}
	else
	{
		for (i = 0; i < neighbours->num_boids; i++)
		{
			/* boid - neighbour */
			vector_s away;
			sub_vector_new(&away, boid->position, neighbours->the_boids[i]->position);
	
			add_vector(vec, &away);
		}
	}

	/* make it a unit vector */
	normalize_vector(vec);
	/* weight factor */
	divide_vector_by_scalar(vec, weight);

	//printf("Rule 2 SEPERATE - "); print_vector(vec);
}

void rule3(vector_s *vec, iboid_s *boid, boids_s *neighbours, float weight)
{
	/* calculate the neighbours weight */
	sum_all_boids_velocity(neighbours, vec);
	/* remove yourself */
	sub_vector(vec, boid->velocity);
	/* remove yourself from the weighting */
	divide_vector_by_scalar(vec, neighbours->num_boids-1);

	/* make it a unit vector */
	normalize_vector(vec);
	/* weight factor */
	divide_vector_by_scalar(vec, weight);

	//printf("Rule 3 ALLIGNMENT - "); print_vector(vec);
}

void find_neighbours(boids_s *boids_p, iboid_s *current_boid, int radius)
{
	int i;

	if (radius == -1)
	{
		neighbours_p->num_boids = boids_p->num_boids;
		for (i = 0; i < boids_p->num_boids; i++)
		{
			copy_boid(boids_p->the_boids[i], neighbours_p->the_boids[i]);
		}
	}
	else
	{
		neighbours_p->num_boids = 0;
		for (i = 0; i < boids_p->num_boids; i++)
		{
			if (current_boid->id == boids_p->the_boids[i]->id){
				continue; // current_boid and the boid from the list are the same object, so skip
			}
			if (distance_between_vectors(boids_p->the_boids[i]->position, current_boid->position) <= radius)
			{
				copy_boid(boids_p->the_boids[i], neighbours_p->the_boids[neighbours_p->num_boids]);
				neighbours_p->num_boids++;
			}
		}
	}
}

#ifdef TRACK_DEATH
void check_collision(iboid_s *current_boid, boids_s *neighbours, parameters_s* parameters, objs_s* objects){
	if (check_boid_collision(current_boid, neighbours, parameters->boid_size_radius)) {
		current_boid->life_status = DEAD;
	} 
	else if (check_wall_collision(current_boid, parameters->boid_size_radius, parameters->dimension_size)) {
		current_boid->life_status = DEAD;
	}
	else if (check_object_collision(current_boid, parameters->boid_size_radius, objects)){
		current_boid->life_status = DEAD;
	}
}

short check_boid_collision(iboid_s *current_boid, boids_s *neighbours, int boid_size_radius){
	for (int i = 0; i < neighbours->num_boids; i++){
		if (distance_between_vectors(current_boid->position, neighbours->the_boids[i]->position) <= 2 * boid_size_radius) {
			// neighbours->the_boids[i]->life_status = DEAD; //Need this?
			return TRUE;
		}
	}
	return FALSE;
}

short check_wall_collision(iboid_s *current_boid, int boid_size_radius, int dimension_size) {
	if ((current_boid->position->x - boid_size_radius) <= 0) {
		return TRUE;
	} 
	else if ((current_boid->position->x + boid_size_radius) >= dimension_size) {
		return TRUE;
	} 
	else if ((current_boid->position->y - boid_size_radius) <= 0) {
		return TRUE;
	} 
	else if ((current_boid->position->y + boid_size_radius) >= dimension_size) {
		return TRUE;
	}
	return FALSE;
}

short check_object_collision(iboid_s *current_boid, int boid_size_radius, objs_s* objects) {
	for (int i = 0; i < objects->num_objs; i++){
		if (distance_between_vectors(current_boid->position, objects->the_objs[i]->position) <= (boid_size_radius + objects->the_objs[i]->r)) {
			return TRUE;
		}
	}
	return FALSE;
}
#endif

