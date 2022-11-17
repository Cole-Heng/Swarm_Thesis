/**
 * @file simulate.h
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu) 
 * @brief Run a Boids simulation.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief Initialize the simulation by creating ATONS and allocating memory for
 * neighbors
 *
 * @param boids_p The set of all Boids
 * @param parameters The simulation parameters
 * @param objs The set of all objects
 */
void init_simulate(boids_s* boids_p, parameters_s* parameters, objs_s* objs);

/**
 * @brief Free the memory for all structures used by the simulation
 * 
 */
void free_simulate();

/**
 * @brief Simulate a single frame by finding neighbors, applying the 3 rules,
 * updating position, and detecting collision
 *
 * @param boids_p The set of all boids
 * @param parameters The simulation parameters
 * @param objs_p The set of all objects
 * @param frame_num The current frame number the simulation is on
 */
void simulate_a_frame(boids_s* boids_p, parameters_s* parameters, objs_s* objs_p, int frame_num);

void CBF_solution(iboid_s *current_boid);

/**
 * @brief Determine which boids from the whole set are within a Boid's field of
 * view
 *
 * @param boids_p The set of all Boids
 * @param current_boid The Boid to find the neighbors of
 * @param radius The radius around a boid that makes up its field of view
 */
void find_neighbours(boids_s *boids_p, iboid_s *current_boid, int radius);

/**
 * @brief Update the position of a Ghost Boid to atch the Boid it is assigned to
 * and update the velocity to point to the waypoint
 *
 * @param boid The Boid that owns the Ghost Boid to be updated
 * @param objects The set of all objects
 */
void update_ghost(iboid_s *boid, objs_s* objects);

/** 
 * These functions are only if the simulation is set to track Boid deaths
 */
#ifdef TRACK_DEATH

/**
 * @brief Determine if a Boid has collided with the wall, an obstacle, or
 * another boid
 *
 * @param boid The boid to run the checks on
 * @param neighbours The set of the Boid's neighbor
 * @param parameters The simulation parameters
 * @param objects The set of all objects
 */
void check_collision(iboid_s *boid, boids_s *neighbours, parameters_s* parameters, objs_s* objects);

/**
 * @brief Check if a Boid has collided with another boid
 * 
 * @param boid The Boid to check
 * @param neighbours The set of neighbors around the Boid
 * @param boid_size_radius The size of the boids from the simulation parameters
 * @return short 1 if there is a collision, 0 otherwise
 */
short check_boid_collision(iboid_s *boid, boids_s *neighbours, int boid_size_radius);

/**
 * @brief Check if a Boid has collided with the wall
 * 
 * @param current_boid The Boid to check
 * @param boid_size_radius The size of the boids from the simulation parameters
 * @param dimension_size The dimensions of the simulation window
 * @return short 1 if there is a collision, 0 otherwise
 */
short check_wall_collision(iboid_s *current_boid, int boid_size_radius, int dimension_size);

/**
 * @brief Check if a Boid has collided with an object
 * 
 * @param current_boid The Boid to check
 * @param boid_size_radius The size of the boids from the simulation parameters
 * @param objects The set of all objects
 * @return short 1 if there is a collision, 0 otherwise
 */
short check_object_collision(iboid_s *current_boid, int boid_size_radius, objs_s* objects);
#endif

/**
 * @brief Create cardinal marks, the ATONs that line the walls
 * 
 * @param parameters The simulation parameters
 * @return boids_s* The set of all Ghost Boids that are cardinal marks
 */
boids_s *create_cardinal_marks(parameters_s* parameters);

/**
 * @brief Create isolated danger marks, the ATONs that surround all obstacles
 * @param objs The set of all objects
 * @return boids_s* The set of all Ghost Boids that are isolated danger marks
 */
boids_s *create_isolated_danger_marks(objs_s* objs);

/**
 * @brief Get the memory pointer for the set of cardinal marks
 * 
 * @return boids_s* The pointer to the cardinal marks
 */
boids_s *get_CMs_pointer();

/**
 * @brief Get the memory pointer for the set of isolated danger marks
 * 
 * @return boids_s* The pointer to the isolated danger marks
 */
boids_s *get_IDMs_pointer();

/**
 * These functions are for tracking the size of swarms. In othe words, checking
 * how many boids exist in a connected graph where each Boid is a node in the graph
 */
#ifdef TRACK_SWARM

/**
 * @brief Calculated the number of nodes are in the connected graph. In other
 * words, the number of Boids in a swarm
 * 
 * @param linked The set of all boids that are connected to each other by being
 * within neighbor distances
 * @param boids_p The set of all boids
 * @param radius The field of view of the Boids from the simulation parameters
 * @return int The number of Boids in the swarm
 */
int nodes_in_swarm(boids_s *linked, boids_s *boids_p, int radius);

/**
 * @brief Set the visited status of all boids to unvisited
 * 
 * @param swarm The set of all Boids
 */
void set_all_unvisted(boids_s *swarm);

/**
 * @brief Find all Boids that are linked together by being within neighbor
 * distance. In other words, all the Boids that form a connected graph
 * 
 * @param out The set of Boids that are linked together
 * @param boids_p The set of all boids
 * @param current_boid The boid to start finding links to
 * @param radius The field of view of the Boids from the simulation parameters
 */
void find_links(boids_s *out, boids_s *boids_p, iboid_s *current_boid, int radius);
#endif

/**
 * @brief Write final statistics of the simulation to the terminal
 * 
 * @param boids_p The set of all Boids
 */
void output_simulation_final_stats(boids_s *boids_p);