/**
 * @file vector.h
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu) 
 * @brief Some specific vector arithmetic and manipulations.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief Allocate memory for a vector
 * 
 * @return vector_s* 
 */
vector_s *allocate_vector();

/**
 * @brief Free the memory of a vector
 * 
 * @param vec 
 */
void free_vector(vector_s *vec);


/**
 * @brief Set the fields of a vector to random positive values
 * 
 * @param to The vector to set the values of
 * @param max Maximum value the fields can be set to
 */
void rand_positive_vector(vector_s *to, int max);

/**
 * @brief Set the fields of a vector to random positive or negative values
 * 
 * @param to The vector to set the values of
 * @param max Maximum value the fields can be set to
 */
void rand_pos_or_neg_vector(vector_s *to, int max);

/**
 * @brief Set the fields of a vector to zero
 * 
 * @param to The vector to set the values of
 */
void init_vector_to_0(vector_s *to);


/**
 * @brief Print the fields of a vector to the terminal
 * 
 * @param vec The vector to print
 */
void print_vector(vector_s *vec);


/**
 * @brief Calculate the length/magnitude of a vector from the origin
 * 
 * @param a The vector to calculate
 * @return float The length/magnitude
 */
float length_vector(vector_s *a);

/**
 * @brief Calculate the distance between two vectors.
 * 
 * @param a The first vector
 * @param b The second vector
 * @return float The distance between the two vectors
 */
float distance_between_vectors(vector_s *a, vector_s *b);

/**
 * @brief Determine if two vectors are equal
 * 
 * @param a The first vector
 * @param b The second vector
 * @return short 1 if they are equal, 0 if they are different
 */
short vectors_equal(vector_s *a, vector_s *b);


/**
 * @brief Copy the fields from one vector to another
 * 
 * @param from The vector to copy from
 * @param to The vector to copy to
 */
void copy_vector(vector_s *from, vector_s *to);

/**
 * @brief Normalize the fields of a vector
 * 
 * @param a The vector to normalize
 */
void normalize_vector(vector_s *a);


/**
 * @brief Add the fields of one vector to another vector and store it into one vector
 * 
 * @param to The vector to be added to
 * @param with The vector to be added with
 */
void add_vector(vector_s *to, vector_s *with);

/**
 * @brief Add the fields of two vectors together and store it in a third vector
 * 
 * @param to The vector to store the fields
 * @param with_a The first vector to be added
 * @param with_b The second vector to be added
 */
void add_vector_new(vector_s *to, vector_s *with_a, vector_s *with_b);

/**
 * @brief Subtract the fields of one vector to another vector and store it into one vector
 * 
 * @param to The vector to be subtracted from and store the new field
 * @param with The vector to be subtracted with
 */
void sub_vector(vector_s *to, vector_s *with);

/**
 * @brief Subtract the fields of two vectors together and store it in a third vector
 * 
 * @param to The vector to store the fields
 * @param with_a The first vector to be subtracted from
 * @param with_b The second vector to be subtracted with
 */
void sub_vector_new(vector_s *to, vector_s *with_a, vector_s *with_b);

/**
 * @brief Divide the fields of a vector by a scalar
 * 
 * @param vec The vector to be divided
 * @param div_by The scalar to divide with
 */
void divide_vector_by_scalar(vector_s *vec, float div_by);

/**
 * @brief Multiply the fields of a vector by a scalar
 * 
 * @param vec The vector to be multiplied
 * @param div_by The scalar to multiply with
 */
void multiply_vector_by_scalar(vector_s *vec, float mult_by);

/**
 * @brief If WRAP_TRUE is defined in types.h, This function wraps a vector from
 * one side of the simulation window to the other
 *
 * @param vec The vector to be wrapped
 * @param dimension_size The dimensions of the simulation
 */
void wrap_dimensions(vector_s *vec, int dimension_size);
