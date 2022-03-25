

FILE* init_output_file_for_simulation(char *filename, int num_boids, int num_frames, parameters_s *parameters, int dim, int num_objs, int num_ATONs);
void write_frame_to_output_file(FILE *f_p, boids_s *boids_p);
void write_objects_to_file(FILE *f_p, objs_s *objs_p);
void write_atons_to_file(FILE *f_P, boids_s *CMs, boids_s *IDMs);
