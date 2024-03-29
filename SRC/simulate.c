/**
 * @file simulate.c
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @brief Run a Boids simulation.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "types.h"
#include "vector.h"
#include "simulate.h"
#include "boids_struct.h"
#include "osqp/osqp.h"
#include "osqp/glob_opts.h"
#include "osqp/cs.h"


boids_s *neighbours_p;
boids_s *CMs; // Cardinal Marks (wall boids)
boids_s *IDMs; // Isolated Danger Marks (Object boids)
#define CBF_C .1
#define CBF_DS 20 // can be update to be boids_desired_neighbor_distance
FILE *file_sim_stats;
int** heatmap; // heatmap of points that boids visit and explore in the form [y][x]
short full_swarm = FALSE;
int success_frame = -1;

// PROTOTYPES
void rule1(vector_s *vec, iboid_s *boid, boids_s *neighbours, float weight);
void rule2(vector_s *vec, iboid_s *boid, boids_s *neighbours, float desired_seperation, float weight);
void rule3(vector_s *vec, iboid_s *boid, boids_s *neighbours, float weight);

void init_simulate(boids_s* boids_p, parameters_s* parameters, objs_s* objs)
{
	/* allocate data structure for neighbours */
	#ifdef USE_EXTENDED
	CMs = create_cardinal_marks(parameters);
	IDMs = create_isolated_danger_marks(objs);
	neighbours_p = allocate_boids(boids_p->num_boids + CMs->num_boids);
	#else
	neighbours_p = allocate_boids(boids_p->num_boids);
	#endif
	file_sim_stats = (FILE*)fopen("sim_statistics.log", "w");
	heatmap = (int**)malloc(parameters->dimension_size * sizeof(int*));
	for (int i = 0; i < parameters->dimension_size; i++) {
		heatmap[i] = (int*)malloc(parameters->dimension_size * sizeof(int));
	}
}

void free_simulate()
{
	/* allocate data structure for neighbours */
	free_boids(neighbours_p); 
	#ifdef USE_EXTENDED
	free_boids(CMs); 
	free_boids(IDMs); 
	#endif
}

#ifdef USE_EXTENDED
boids_s *create_cardinal_marks(parameters_s* parameters) {
	FILE *out;

	out = (FILE*)fopen("CM_coords.log", "w");

	// seperation ratio for gap between ATONs. They position sep_ratio * boid_radius away from each other
	// 1 = overlapping, 2 = side-by-side, and so on
	int sep_ratio = 2; 

	int num_boids_per_wall = parameters->dimension_size / (sep_ratio * parameters->boid_size_radius);
	boids_s *CMs = allocate_boids(4 * num_boids_per_wall);

	
	int i = 0;
	int j;
	vector_s *temp_vec = allocate_vector();
	temp_vec->x = 1;
	temp_vec->y = 0;
	for (j = 0; j < num_boids_per_wall; j++) { //TL to BL
		CMs->the_boids[j]->position->x = 0;
		CMs->the_boids[j]->position->y = sep_ratio * j * parameters->boid_size_radius;
		copy_vector(temp_vec, CMs->the_boids[j]->velocity);
		CMs->the_boids[j]->life_status = DEAD;
	}
	i = j;

	temp_vec->x = 0;
	temp_vec->y = -1;
	for (j = 0; j < num_boids_per_wall; j++) { //BL tp BR
		CMs->the_boids[j + i]->position->x = sep_ratio * j * parameters->boid_size_radius;
		CMs->the_boids[j + i]->position->y = parameters->dimension_size;
		copy_vector(temp_vec, CMs->the_boids[j + i]->velocity);
		CMs->the_boids[j + i]->life_status = DEAD;
	}
	i = 2 * j;

	temp_vec->x = -1;
	temp_vec->y = 0;
	for (j = 0; j < num_boids_per_wall; j++) { //BR to TR
		CMs->the_boids[j + i]->position->x = parameters->dimension_size;
		CMs->the_boids[j + i]->position->y = parameters->dimension_size - (sep_ratio * j * parameters->boid_size_radius);
		copy_vector(temp_vec, CMs->the_boids[j + i]->velocity);
		CMs->the_boids[j + i]->life_status = DEAD;
	}

	i = 3 * j;

	temp_vec->x = 0;
	temp_vec->y = 1;
	for (j = 0; j < num_boids_per_wall; j++) { //TR to TL
		CMs->the_boids[j + i]->position->x = parameters->dimension_size - (sep_ratio * j * parameters->boid_size_radius);
		CMs->the_boids[j + i]->position->y = 0;
		copy_vector(temp_vec, CMs->the_boids[j + i]->velocity);
		CMs->the_boids[j + i]->life_status = DEAD;
	}

	fprintf(out, "#Markers/wall: %d\n", num_boids_per_wall);
	fprintf(out, "#Markers: %d\n", CMs->num_boids);
	for (int i = 0; i < CMs->num_boids; i++) {
		fprintf(out, "marker[%d] pos[%f, %f] vel[%f, %f]\n", (i), CMs->the_boids[i]->position->x, CMs->the_boids[i]->position->y, CMs->the_boids[i]->velocity->x, CMs->the_boids[i]->velocity->y);
	}

	for (int i = 0; i < CMs->num_boids; i++) {
		CMs->the_boids[i]->life_status = DEAD;
		fprintf(out, "D/A:%d\n", CMs->the_boids[i]->life_status);
	}
	fflush(out);
	/* close output file */
	fclose(out);
	return CMs;
}

boids_s *create_isolated_danger_marks(objs_s* objs) {
	FILE *out;

	out = (FILE*)fopen("IDM_coords.log", "w");
	
	int waypoint_count = 0;
	for (int i = 0; i < objs->num_objs; i++) {
		if (objs->the_objs[i]->is_waypoint == TRUE) {
			waypoint_count++;
		}
	}
	boids_s *IDMs = allocate_boids(4 * (objs->num_objs - waypoint_count));

	fprintf(out, "#Markers: %d\n", IDMs->num_boids);

	vector_s *temp_vec = allocate_vector();
	temp_vec->x = 0;
	int j = 0;
	for (int i = 0; i < objs->num_objs; i++) { //Set North IDM
		if (objs->the_objs[i]->is_waypoint == TRUE){
			continue; // Don't add marks to waypoints
		}
		temp_vec->y = -1 * objs->the_objs[i]->radius;
		add_vector_new(IDMs->the_boids[j]->position, objs->the_objs[i]->position, temp_vec);
		IDMs->the_boids[j]->velocity->y = -1;
		IDMs->the_boids[j]->life_status = DEAD;
		fprintf(out, "marker[%d] N on obj[%d] pos[%f, %f] vel[%f, %f]\n", (j), (i), IDMs->the_boids[j]->position->x, IDMs->the_boids[j]->position->y, IDMs->the_boids[j]->velocity->x, IDMs->the_boids[j]->velocity->y);
		j++;
	}

	for (int i = 0; i < objs->num_objs; i++) { //Set South IDM
		if (objs->the_objs[i]->is_waypoint == TRUE){
			continue; // Don't add marks to waypoints
		}
		temp_vec->y = objs->the_objs[i]->radius;
		add_vector_new(IDMs->the_boids[j]->position, objs->the_objs[i]->position, temp_vec);
		IDMs->the_boids[j]->velocity->y = 1;
		IDMs->the_boids[j]->life_status = DEAD;
		fprintf(out, "marker[%d] S on obj[%d] pos[%f, %f] vel[%f, %f]\n", (j), (i), IDMs->the_boids[j]->position->x, IDMs->the_boids[j]->position->y, IDMs->the_boids[j]->velocity->x, IDMs->the_boids[j]->velocity->y);
		j++;
	}

	temp_vec->y = 0;
	for (int i = 0; i < objs->num_objs; i++) { //Set East IDM
		if (objs->the_objs[i]->is_waypoint == TRUE){
			continue; // Don't add marks to waypoints
		}
		temp_vec->x = objs->the_objs[i]->radius;
		add_vector_new(IDMs->the_boids[j]->position, objs->the_objs[i]->position, temp_vec);
		IDMs->the_boids[j]->velocity->x = 1;
		IDMs->the_boids[j]->life_status = DEAD;
		fprintf(out, "marker[%d] E on obj[%d] pos[%f, %f] vel[%f, %f]\n", (j), (i), IDMs->the_boids[j]->position->x, IDMs->the_boids[j]->position->y, IDMs->the_boids[j]->velocity->x, IDMs->the_boids[j]->velocity->y);
		j++;
	}

	for (int i = 0; i < objs->num_objs; i++) { //Set West IDM
		if (objs->the_objs[i]->is_waypoint == TRUE){
			continue; // Don't add marks to waypoints
		}
		temp_vec->x = -1 * objs->the_objs[i]->radius;
		add_vector_new(IDMs->the_boids[j]->position, objs->the_objs[i]->position, temp_vec);
		IDMs->the_boids[j]->velocity->x = -1;
		IDMs->the_boids[j]->life_status = DEAD;
		fprintf(out, "marker[%d] W on obj[%d] pos[%f, %f] vel[%f, %f]\n", (j), (i), IDMs->the_boids[j]->position->x, IDMs->the_boids[j]->position->y, IDMs->the_boids[j]->velocity->x, IDMs->the_boids[j]->velocity->y);
		j++;
	}

	for (int i = 0; i < IDMs->num_boids; i++) {
		IDMs->the_boids[i]->life_status = DEAD;
		fprintf(out, "D/A:%d\n", IDMs->the_boids[i]->life_status);
	}
	fflush(out);
	/* close output file */
	fclose(out);
	return IDMs;
}
#endif

/*
 * Rule 1: Boids try to fly towards the centre of mass of neighbouring boids.
 * Rule 2: Boids try to keep a small distance away from other objects (including other boids).
 * Rule 3: Boids try to match velocity with near boids.
 */
void simulate_a_frame(boids_s* boids_p, parameters_s* parameters, objs_s* objs_p, int frame_num)
{
	vector_s velocity_change_from_rules[3];

	for (int i = 0; i < boids_p->num_boids; i++)
	{
		#ifdef ENFORCE_DEATH
			if (boids_p->the_boids[i]->life_status == DEAD){
				//add back to allow boids to see and add to heatmap when dead
				// if (heatmap[(int)floor(boids_p->the_boids[i]->position->y)][(int)floor(boids_p->the_boids[i]->position->x)] < 7){
				// 	heatmap[(int)floor(boids_p->the_boids[i]->position->y)][(int)floor(boids_p->the_boids[i]->position->x)]++;
				// }
				continue;
			}
		#endif

		#ifdef USE_EXTENDED
		/* Point ghost boid towards waypoint and position on boid*/
		if (boids_p->the_boids[i]->is_leader) 
		{
			update_ghost(boids_p->the_boids[i], objs_p);
		}
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

		#ifdef USE_CBF
		CBF_solution(boids_p->the_boids[i]);
		#endif

		/* add the speed vector */
		update_boid(boids_p->the_boids[i], parameters->dimension_size);

		// add one to the location heatmap. Round boid coordinates down to an
		// int
		heatmap[(int)floor(boids_p->the_boids[i]->position->y)][(int)floor(boids_p->the_boids[i]->position->x)]++;
		#ifdef TRACK_DEATH
		check_collision(boids_p->the_boids[i], neighbours_p, parameters, objs_p, frame_num);
		#endif
	}

	if (success_frame == -1) {
		// Check if all alive boids have reached the waypoint. If so, mark what
		// frame this was achieved.
		success_frame = frame_num;
		for (int i = 0; i < boids_p->num_boids; i++)
		{
			if (boids_p->the_boids[i]->life_status == ALIVE && boids_p->the_boids[i]->success == FALSE) {
				success_frame = -1;
				break;
			}
		}
	}

	#ifdef TRACK_SWARM
		boids_s *links = allocate_boids(boids_p->num_boids);
		set_all_unvisted(boids_p);
		find_links(links, boids_p, boids_p->the_boids[0], parameters->neighbour_distance);
		boids_p->the_boids[0]->was_visited = TRUE;
		int num_in_swarm = nodes_in_swarm(links, boids_p, parameters->neighbour_distance) + 1;
		if (full_swarm == TRUE && num_in_swarm != boids_p->num_boids){ // swarm broke
			fprintf(file_sim_stats, "swarm_broken: %d\n", frame_num);
			fflush(file_sim_stats);
			full_swarm = FALSE;
		} else if (full_swarm == FALSE && num_in_swarm == boids_p->num_boids){ //swarm achieved
			fprintf(file_sim_stats, "swarm_achieved: %d\n", frame_num);
			fflush(file_sim_stats);
			full_swarm = TRUE;
		}
		free_boids(links);
	#endif


}

#ifdef USE_CBF
void CBF_solution(iboid_s *current_boid) {
	// Boid has no neighbors, so skip CBF calculation
	if (neighbours_p->num_boids == 0) {
		return;
	}
	int num_neigh = neighbours_p->num_boids;
	c_float P_x[2] = {1.0, 1.0};
    c_int   P_nnz  = 2;
    c_int   P_r[2] = { 0, 1};
    c_int   P_c[3] = { 0, 1, 2};
    c_float q[2]   = { -1 * current_boid->velocity->x, -1 * current_boid->velocity->y};
    c_float A_x[num_neigh * 2];
    c_int   A_nnz  = num_neigh * 2;
    c_int   A_r[num_neigh * 2];
    c_int   A_c[3] = {0, num_neigh, num_neigh * 2};
    c_float l[num_neigh];
	c_float u[num_neigh];
	c_int n = 2;
    c_int m = neighbours_p->num_boids;

	for (int ii = 0; ii < num_neigh; ii++) {
		A_x[ii] = (current_boid->position->x - neighbours_p->the_boids[ii]->position->x);
		A_x[ii + num_neigh] = (current_boid->position->y - neighbours_p->the_boids[ii]->position->y);
		A_r[ii] = ii;
		A_r[ii + num_neigh] = ii;

		float h = (pow(current_boid->position->x - neighbours_p->the_boids[ii]->position->x, 2) + 
				   pow(current_boid->position->y - neighbours_p->the_boids[ii]->position->y, 2) - 
				   (CBF_DS * CBF_DS));
		l[ii] = -1 * CBF_C * h;

		u[ii] = INT_MAX; // We don't have a true upper limit, so make it max
	}

	// Exitflag
	c_int exitflag = 0;

  	// Workspace structures
  	OSQPWorkspace *work;
  	OSQPSettings  *settings = (OSQPSettings *)c_malloc(sizeof(OSQPSettings));
  	OSQPData      *data     = (OSQPData *)c_malloc(sizeof(OSQPData));

  	// Populate data
  	if (data) {
  	  data->n = n;
  	  data->m = m;
  	  data->P = csc_matrix(data->n, data->n, P_nnz, P_x, P_r, P_c);
  	  data->q = q;
  	  data->A = csc_matrix(data->m, data->n, A_nnz, A_x, A_r, A_c);
  	  data->l = l;
  	  data->u = u;
  	}

	// Define solver settings as default
  	if (settings) osqp_set_default_settings(settings);
	settings->verbose = 0;
	settings->polish = 1;
	settings->eps_abs = 1E-12;
	settings->eps_rel = 1E-12;
	settings->eps_prim_inf = 1E-12;
	settings->eps_dual_inf = 1E-12;

  	// Setup workspace
  	exitflag = osqp_setup(&work, data, settings);
	if (exitflag != 0) {
		printf("ERROR: Failed OSQP setup with flag %d\n", (int)exitflag);
	}

  	// Solve Problem
  	osqp_solve(work);
	
	current_boid->velocity->x = work->solution->x[0];
	current_boid->velocity->y = work->solution->x[1];
	normalize_vector(current_boid->velocity);

	// free all osqp structures/memory
	osqp_cleanup(work);
	if (data) {
        if (data->A) c_free(data->A);
        if (data->P) c_free(data->P);
        c_free(data);
    }
	c_free(settings);\
}
#endif

void rule1(vector_s *vec, iboid_s *boid, boids_s *neighbours, float weight)
{
	/* calculate the neighbours weight */
	sum_all_real_boids_position(neighbours, vec);
	/* remove yourself */
	sub_vector(vec, boid->position);
	/* remove yourself from the weighting */
	divide_vector_by_scalar(vec, neighbours->num_real_boids-1);

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
	/* add ghost boid if leader */
	if (boid->is_leader)
	{
		vector_s* temp_vec = allocate_vector();
		copy_vector(boid->ghost_boid->velocity, temp_vec);
		multiply_vector_by_scalar(temp_vec, imax(1, neighbours->num_real_boids * GHOST_INFLUENCE));
		add_vector(vec, temp_vec);
	}
	/* remove yourself from the weighting, add one if ghost boid is present*/
	divide_vector_by_scalar(vec, (neighbours->num_boids-1 + (boid->is_leader)));

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
		neighbours_p->num_real_boids = 0;
		// Compare to boids
		for (i = 0; i < boids_p->num_boids; i++)
		{
			if (current_boid->id == boids_p->the_boids[i]->id){
				continue; // current_boid and the boid from the list are the same object, so skip
			}
			#ifndef AVOID_SUCCESS
				if (boids_p->the_boids[i]->success == TRUE) {
					continue;
				}
			#endif
			if (distance_between_vectors(boids_p->the_boids[i]->position, current_boid->position) <= radius)
			{
				copy_boid(boids_p->the_boids[i], neighbours_p->the_boids[neighbours_p->num_boids]);
				neighbours_p->num_boids++;
				neighbours_p->num_real_boids++;
				#ifdef SHARE_GOAL
				if(current_boid->is_leader == TRUE && boids_p->the_boids[i]->life_status == ALIVE && boids_p->the_boids[i]->is_leader == FALSE) {
					set_boid_leader(boids_p->the_boids[i], TRUE);
					// printf("mod boid #%d at p[%f, %f] leader: %d\n", boids_p->the_boids[i]->id, boids_p->the_boids[i]->position->x, boids_p->the_boids[i]->position->y, boids_p->the_boids[i]->is_leader);
				}
				#endif
			}
		}

		#ifdef USE_EXTENDED
		// Compare to Cardinal Marks (CMs)
		for (i = 0; i < CMs->num_boids; i++)
		{
			if (distance_between_vectors(CMs->the_boids[i]->position, current_boid->position) <= radius)
			{
				copy_boid(CMs->the_boids[i], neighbours_p->the_boids[neighbours_p->num_boids]);
				neighbours_p->num_boids++;
			}
		}

		// Compare to Isolated Danger Marks (IDMs)
		for (i = 0; i < IDMs->num_boids; i++)
		{
			if (distance_between_vectors(IDMs->the_boids[i]->position, current_boid->position) <= radius)
			{
				copy_boid(IDMs->the_boids[i], neighbours_p->the_boids[neighbours_p->num_boids]);
				neighbours_p->num_boids++;
			}
		}
		#endif
	}
}

#ifdef TRACK_DEATH
void check_collision(iboid_s *current_boid, boids_s *neighbours, parameters_s* parameters, objs_s* objects, int frame_num){
	if (check_boid_collision(current_boid, neighbours, parameters->boid_size_radius)) {
		//printf("Death occured in frame %d\n", frame_num);
		current_boid->life_status = DEAD;
	} 
	else if (check_wall_collision(current_boid, parameters->boid_size_radius, parameters->dimension_size)) {
		current_boid->life_status = DEAD;
		//printf("Death occured in frame %d\n", frame_num);
	}
	else if (check_object_collision(current_boid, parameters->boid_size_radius, objects)){
		current_boid->life_status = DEAD;
		//printf("Death occured in frame %d\n", frame_num);
	}
}

short check_boid_collision(iboid_s *current_boid, boids_s *neighbours, int boid_size_radius){
	for (int i = 0; i < neighbours->num_real_boids; i++){
		if (distance_between_vectors(current_boid->position, neighbours->the_boids[i]->position) <= 2 * boid_size_radius) {
			//printf("boid died at (%f, %f) by hitting boid at (%f, %f)\n", current_boid->position->x, current_boid->position->y, neighbours->the_boids[i]->position->x, neighbours->the_boids[i]->position->y);
			neighbours->the_boids[i]->life_status = DEAD; 
			return TRUE;
		}
		// #ifdef SHARE_GOAL
		// else if(current_boid->is_leader == TRUE && neighbours->the_boids[i]->life_status == ALIVE && neighbours->the_boids[i]->is_leader == FALSE) {
		// 	set_boid_leader(neighbours->the_boids[i], TRUE);
		// 	printf("mod boid #%d at p[%f, %f] leader: %d\n", neighbours->the_boids[i]->id, neighbours->the_boids[i]->position->x, neighbours->the_boids[i]->position->y, neighbours->the_boids[i]->is_leader);
		// }
		// #endif
	}
	return FALSE;
}

short check_wall_collision(iboid_s *current_boid, int boid_size_radius, int dimension_size) {
	if ((current_boid->position->x - boid_size_radius) <= 0) {
		//printf("boid died at (%f, %f) by hitting left wall\n", current_boid->position->x, current_boid->position->y);
		return TRUE;
	} 
	else if ((current_boid->position->x + boid_size_radius) >= dimension_size) {
		//printf("boid died at (%f, %f) by hitting right wall\n", current_boid->position->x, current_boid->position->y);
		return TRUE;
	} 
	else if ((current_boid->position->y - boid_size_radius) <= 0) {
		//printf("boid died at (%f, %f) by hitting top wall\n", current_boid->position->x, current_boid->position->y);
		return TRUE;
	} 
	else if ((current_boid->position->y + boid_size_radius) >= dimension_size) {
		//printf("boid died at (%f, %f) by hitting bottom wall\n", current_boid->position->x, current_boid->position->y);
		return TRUE;
	}
	return FALSE;
}

short check_object_collision(iboid_s *current_boid, int boid_size_radius, objs_s* objects) {
	for (int i = 0; i < objects->num_objs; i++){
		if (distance_between_vectors(current_boid->position, objects->the_objs[i]->position) <= (boid_size_radius + objects->the_objs[i]->radius)) {
			if (objects->the_objs[i]->is_waypoint == TRUE) {
				current_boid->success = TRUE;
				continue;
			}
			//printf("boid died at (%f, %f) by hitting object at (%f, %f)\n", current_boid->position->x, current_boid->position->y, objects->the_objs[i]->position->x, objects->the_objs[i]->position->y);
			return TRUE; 
		}
	}
	return FALSE;
}
#endif

#ifdef USE_EXTENDED
void update_ghost(iboid_s *boid, objs_s* objects)
{
	/* Update position to be on boid */
	copy_vector(boid->position, boid->ghost_boid->position);

	/* Update Velocity to be a unit vector pointing to waypoint */
	for (int i = 0; i < objects->num_objs; i++)
	{
		if (objects->the_objs[i]->is_waypoint)
		{
			vector_s *temp_vec = allocate_vector();
			sub_vector_new(temp_vec, objects->the_objs[i]->position, boid->ghost_boid->position);
			normalize_vector(temp_vec);
			copy_vector(temp_vec, boid->ghost_boid->velocity);
			break;
		}
	}
}

boids_s *get_CMs_pointer() {
	return CMs;
}
boids_s *get_IDMs_pointer() {
	return IDMs;
}
#endif

#ifdef TRACK_SWARM
void set_all_unvisted(boids_s *boids_p) {
	for (int i = 0; i < boids_p->num_boids; i++) {
		boids_p->the_boids[i]->was_visited = FALSE;
	}
}

int nodes_in_swarm(boids_s *linked, boids_s *boids_p, int radius) {
	int count = 0;
	for (int i = 0; i < linked->num_boids; i++) {
		// if (linked->the_boids[i]->was_visited){
		// 	break;
		// }
		linked->the_boids[i]->was_visited = TRUE;
		count++;
		boids_s *links = allocate_boids(boids_p->num_boids);
		find_links(links, boids_p, linked->the_boids[i], radius);
		count += nodes_in_swarm(links, boids_p, radius);
		free_boids(links);
	}
	return count;
}

void find_links(boids_s *out, boids_s *boids_p, iboid_s *current_boid, int radius){
	int i;

	if (radius == -1)
	{
		out->num_boids = boids_p->num_boids;
		for (i = 0; i < boids_p->num_boids; i++)
		{
			copy_boid(boids_p->the_boids[i], out->the_boids[i]);
		}
	}
	else
	{
		out->num_boids = 0;
		// Compare to boids
		for (i = 0; i < boids_p->num_boids; i++)
		{
			if (current_boid->id == boids_p->the_boids[i]->id){
				continue; // current_boid and the boid from the list are the same object, so skip
			}
			if (distance_between_vectors(boids_p->the_boids[i]->position, current_boid->position) <= radius)
			{
				if (boids_p->the_boids[i]->was_visited == FALSE) {
					boids_p->the_boids[i]->was_visited = TRUE;
					copy_boid(boids_p->the_boids[i], out->the_boids[out->num_boids]);
					out->num_boids++;
				}
			}
		}
	}
}
#endif

void output_simulation_final_stats(boids_s *boids_p, int dim, double elapsed) {
	int success_and_alive = 0;
	int success_and_dead = 0;
	int fail_and_alive = 0;
	int fail_and_dead = 0;
	for (int i = 0; i < boids_p->num_boids; i++) {
		if (boids_p->the_boids[i]->life_status == ALIVE) {
			if (boids_p->the_boids[i]->success == TRUE) {
				success_and_alive++;
			} else {
				fail_and_alive++;
			}
		} else {
			if (boids_p->the_boids[i]->success == TRUE) {
				success_and_dead++;
			} else {
				fail_and_dead++;
			}
		}
	}
	fprintf(file_sim_stats, "success_and_alive: %d\n", success_and_alive);
	fprintf(file_sim_stats, "success_and_dead: %d\n", success_and_dead);
	fprintf(file_sim_stats, "fail_and_alive: %d\n", fail_and_alive);
	fprintf(file_sim_stats, "fail_and_dead: %d\n", fail_and_dead);
	fprintf(file_sim_stats, "success_frame: %d\n", success_frame);
	fprintf(file_sim_stats, "elapsed_time: %f\n", elapsed);
	fprintf(file_sim_stats, "deaths: \n");
	for (int i = 0; i < boids_p->num_boids; i++) {
		if (boids_p->the_boids[i]->life_status == DEAD) {
			fprintf(file_sim_stats, "%f %f\n", boids_p->the_boids[i]->position->x, boids_p->the_boids[i]->position->y);
		}
	}
	fprintf(file_sim_stats, "Heatmap:\n");
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			fprintf(file_sim_stats, "%d", heatmap[i][j]);
			if (j != dim - 1) {
				fprintf(file_sim_stats, ",");
			}
		}
		fprintf(file_sim_stats, "\n");
	}

	fflush(file_sim_stats);
	fclose(file_sim_stats);
}