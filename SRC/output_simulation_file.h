/**
 * @file output_simulation_file.h
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu) 
 * @brief Writes data describing a Boids simulation to a file.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief Initialize the file to output all simulation to and write the headers
 * 
 * @param filename The name of the file to write to
 * @param num_boids The number of Boids in the simulation
 * @param num_frames The number of frames that are simulated
 * @param parameters The simulation parameters
 * @param dim Designates two-dimensional or three-dimensional
 * @param num_objs The number of objects in the simulation
 * @param num_ATONs The number of ATONs in the simulation
 * @return FILE* A file pointer to the output file
 */
FILE* init_output_file_for_simulation(char *filename, int num_boids, int num_frames, parameters_s *parameters, int dim, int num_objs, int num_ATONs);

/**
 * @brief Write a frame of simulation to the output file
 * 
 * @param f_p A pointer to the output file
 * @param boids_p The set of all Boids
 */
void write_frame_to_output_file(FILE *f_p, boids_s *boids_p);

/**
 * @brief Write the information about all the objects to the output file
 * 
 * @param f_p A pointer to the output file
 * @param objs_p The set of all objects in the simulation
 */
void write_objects_to_file(FILE *f_p, objs_s *objs_p);

/**
 * @brief Write the information about all the ATONs to the output file
 * 
 * @param f_P A pointer to the output file
 * @param CMs The set of all cardinal marks
 * @param IDMs The set of all isolated danger marks
 */
void write_atons_to_file(FILE *f_P, boids_s *CMs, boids_s *IDMs);
