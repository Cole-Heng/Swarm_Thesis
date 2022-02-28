
void init_simulate(boids_s* boids_p);
void free_simulate();

void simulate_a_frame(boids_s* boids_p, parameters_s* parameters, objs_s* objs_p);
void find_neighbours(boids_s *boids_p, iboid_s *current_boid, int radius);
void update_ghost(iboid_s *boid, objs_s* objects);

#ifdef TRACK_DEATH
void check_collision(iboid_s *boid, boids_s *neighbours, parameters_s* parameters, objs_s* objects);
short check_boid_collision(iboid_s *boid, boids_s *neighbours, int boid_size_radius);
short check_wall_collision(iboid_s *current_boid, int boid_size_radius, int dimension_size);
short check_object_collision(iboid_s *current_boid, int boid_size_radius, objs_s* objects);
#endif
