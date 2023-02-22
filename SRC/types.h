/**
 * @file types.h
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu)
 * @brief Type and structure defenitions and settings for Boids simulations
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef ALIVE
#define ALIVE 1
#endif
#ifndef DEAD
#define DEAD 0
#endif

#ifndef BOID_RADIUS
#define BOID_RADIUS 5 //physical size of a boid
#endif

#ifndef GHOST_INFLUENCE
#define GHOST_INFLUENCE 1.0 //a float percentage
#endif

// Pick the dimensions
#define DIM_2D
//#define DIM_3D
//

// Comment out to exclude screen wrapping and to allow boids to move when dead
//#define WRAP_TRUE

#define TRACK_DEATH
#ifdef TRACK_DEATH
#define ENFORCE_DEATH // WARNING TRACK_DEATH must be defined to enforce death
#endif

#define SHARE_GOAL //leaders share leader status with boids they are near to

// #define TRACK_SWARM // record if the swarm fully connects/breaks

#define AVOID_SUCCESS // define if boids should avoid other boids that have reached the waypoint 

//#define USE_EXTENDED

#ifdef USE_EXTENDED // CBF builds on and depends on Boids_Extended
//#define USE_CBF // define if a CBF solution should be calculated and used, or if boids solution alone should be used
#endif

#ifndef TYPES_H
#define TYPES_H

/**
 * @brief Return the maximum of two integers
 * 
 * @param a 
 * @param b 
 * @return int 
 */
static inline int imax(int a, int b) {
	return (a >= b) ? a : b;
}

/**
 * @brief Return the minimum of two integers
 * 
 * @param a 
 * @param b 
 * @return int 
 */
static inline int imin(int a, int b) {
	return (a <= b) ? a : b;
}

/* A boid */
typedef struct vector_s_s vector_s;
struct vector_s_s
{
	float x;
	float y;
#ifdef DIM_3D
	float z;
#endif
};

/* parameter simulation */
typedef struct parameters_s_s parameters_s;
struct parameters_s_s
{
	int dimension_size;
	int neighbour_distance;
	int neighbour_desired_seperation;
	float weight_rule1;
	float weight_rule2;
	float weight_rule3;
	int boid_size_radius;
	int object_radius;
};


/* A ghost boid */
typedef struct ghost_boid_s_s ghost_boid_s;
struct ghost_boid_s_s
{
	unsigned int id;
	vector_s *position;
	vector_s *velocity;
};

/* A boid */
typedef struct iboid_s_s iboid_s;
struct iboid_s_s
{
	unsigned int id;
	vector_s *position;
	vector_s *velocity;
	float mass; // might be needed if we allow collisions
	#ifdef TRACK_DEATH
		short life_status; // alive or dead depending on collision
	#endif
	short is_leader;
	ghost_boid_s *ghost_boid;
	short was_visited;
	short success;
};


/* The set of boids */
typedef struct boids_s_s boids_s;
struct boids_s_s
{
	iboid_s **the_boids;
	int num_boids; // all boids, including ghosts
	int num_real_boids; // only real boids, not ghosts
};

struct shape
{
	int num_vertices;
	vector_s points[];
};

typedef struct quad_s quad;
struct quad_s
{
	vector_s points[4];
};

/* An object */
typedef struct obj_s_s obj_s;
struct obj_s_s
{
	vector_s *position;
	vector_s *velocity;
	int radius;
	short is_waypoint;
};


/* The set of objects */
typedef struct objs_s_s objs_s;
struct objs_s_s
{
	obj_s **the_objs;
	int num_objs;
};
#endif // TYPES_H