
void init_simulate(boids_s* boids_p);
void free_simulate();

void simulate_a_frame(boids_s* boids_p, parameters_s* parameters);
void find_neighbours(boids_s *boids_p, iboid_s *current_boid, int radius);
