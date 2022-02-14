boids_s *allocate_boids(int num_boids);
void free_boids(boids_s *boids);

void set_boid_velocity(iboid_s* boid, float x, float y, float z);
void set_boid_position(iboid_s* boid, float x, float y, float z);
iboid_s *allocate_boid();
void free_boid(iboid_s *boid);

boids_s *init_boids_with_file(int *num_boids, int *num_frames, parameters_s* parameters, FILE *fp);
boids_s *init_random_boids(int num_boids, int dimension_size);

#ifdef DIM_3D
void set_boid_velocity_xyz(iboid_s *boid, float x, float y, float z);
void set_boid_position_xyz(iboid_s *boid, float x, float y, float z);
#else
void set_boid_velocity_xy(iboid_s *boid, float x, float y);
void set_boid_position_xy(iboid_s *boid, float x, float y);
#endif

void update_boid(iboid_s *boid, int dimension_size);
void copy_boid(iboid_s *from, iboid_s *to);

void sum_all_boids_position(boids_s *boids_p, vector_s *sum);
void sum_all_boids_velocity(boids_s *boids_p, vector_s *sum);
