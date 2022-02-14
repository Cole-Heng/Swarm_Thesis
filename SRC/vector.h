vector_s *allocate_vector();
void free_vector(vector_s *vec);

void rand_positive_vector(vector_s *to, int max);
void rand_pos_or_neg_vector(vector_s *to, int max);
void init_vector_to_0(vector_s *to);

void print_vector(vector_s *vec);

float length_vector(vector_s *a);
float distance_between_vectors(vector_s *a, vector_s *b);
float distance_to_perpendicular(vector_s *a, vector_s *p);

void copy_vector(vector_s *from, vector_s *to);
void normalize_vector(vector_s *to);

void add_vector(vector_s *to, vector_s *with);
void add_vector_new(vector_s *to, vector_s *with_a, vector_s *with_b);
void sub_vector(vector_s *to, vector_s *with);
void sub_vector_new(vector_s *to, vector_s *with_a, vector_s *with_b);
void divide_vector_by_scalar(vector_s *vec, float div_by);
void multiply_vector_by_scalar(vector_s *vec, float mult_by);
void dot_product_new(vector_s *to, vector_s *with_a, vector_s *with_b);

void wrap_dimensions(vector_s *vec, int dimension_size);
