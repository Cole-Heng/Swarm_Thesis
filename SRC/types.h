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
#define BOID_RADIUS 5
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

#ifndef TYPES_H
#define TYPES_H


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
};


/* The set of boids */
typedef struct boids_s_s boids_s;
struct boids_s_s
{
	iboid_s **the_boids;
	int num_boids;
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
#endif // TYPES_H

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