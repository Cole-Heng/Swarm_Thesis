/**
 * @file boids_struct.h
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu) 
 * @brief Modifies the fields of a Boid struct. This includes ATONs.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

boids_s *allocate_boids(int num_boids);
void free_boids(boids_s *boids);

void set_boid_velocity(iboid_s* boid, float x, float y, float z);
void set_boid_position(iboid_s* boid, float x, float y, float z);
iboid_s *allocate_boid();
void free_boid(iboid_s *boid);
ghost_boid_s *allocate_ghost();


boids_s *init_boids_with_file(int *num_boids, int *num_frames, parameters_s* parameters, FILE *fp);
boids_s *init_random_boids(int num_boids, int dimension_size);

#ifdef DIM_3D
void set_boid_velocity_xyz(iboid_s *boid, float x, float y, float z);
void set_boid_position_xyz(iboid_s *boid, float x, float y, float z);
#else
void set_boid_velocity_xy(iboid_s *boid, float x, float y);
void set_boid_position_xy(iboid_s *boid, float x, float y);
#endif
void set_boid_leader(iboid_s *boid, short is_leader);
void assign_ghost_boid(iboid_s *boid);

void update_boid(iboid_s *boid, int dimension_size);
void copy_boid(iboid_s *from, iboid_s *to);

void sum_all_boids_position(boids_s *boids_p, vector_s *sum);
void sum_all_boids_velocity(boids_s *boids_p, vector_s *sum);
