

FILE* init_output_file_for_simulation(char *filename, int num_boids, int num_frames, parameters_s *parameters, int dim);
void write_frame_to_output_file(FILE *f_p, boids_s *boids_p);
