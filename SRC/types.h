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
// #define WRAP_TRUE
#define TRACK_DEATH

#ifdef TRACK_DEATH
#define ENFORCE_DEATH // WARNING TRACK_DEATH must be defined to enforce death
#endif

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
};

/* A boid */
typedef struct iboid_s_s iboid_s;
struct iboid_s_s
{
	vector_s *position;
	vector_s *velocity;
	float mass; // might be needed if we allow collisions
	#ifdef TRACK_DEATH
		short life_status; // alive or dead depending on collision
	#endif
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
