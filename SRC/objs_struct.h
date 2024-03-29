/**
 * @file objs_struct.h
 * @author Cole Hengstebeck (Hengstcm@miamioh.edu)
 * @author Peter Jamieson (Jamiespa@miamioh.edu) 
 * @brief Modifies the fields of an Object structure.
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

objs_s *allocate_objects(int num_objs);
void free_objects(objs_s *objs);

void set_object_velocity(obj_s* obj, float x, float y, float z);
void set_object_position(obj_s* obj, float x, float y, float z);
obj_s *allocate_object();
void free_object(obj_s *obj);

objs_s *init_objects_with_file(int *num_objs, FILE *fp);
objs_s *init_random_objects(int num_objs, int dimension_size, int radius);

#ifdef DIM_3D
void set_object_velocity_xyz(obj_s *obj, float x, float y, float z);
void set_object_position_xyz(obj_s *obj, float x, float y, float z);
#else
void set_object_velocity_xy(obj_s *obj, float x, float y);
void set_object_position_xy(obj_s *obj, float x, float y);
#endif
void set_object_radius(obj_s *obj, int r);
void set_object_waypoint(obj_s *obj, short is_waypoint);

void copy_object(obj_s *from, obj_s *to);

void sum_all_objects_position(objs_s *objs_p, vector_s *sum);
void sum_all_objects_velocity(objs_s *objs_p, vector_s *sum);
