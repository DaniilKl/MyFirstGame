#pragma once
#ifndef OTHER_H
#define OTHER_H
//including external libraries
#include <SFML/Graphics.hpp>
//
//including my libraries
#include "basic_classes_and_objects.h"
//
namespace my {
	//Functions for generating random numbers
	//template <typename type>
	//type rand_number(type min, type max);
	//template <typename type>
	//type rand_number(type min, type ommit, type max);
	float rand_float_number(float min, float max);
	float rand_float_number(float min, float ommit, float max);
	int rand_int_number(int min, int max);
	int rand_int_number(int min, int ommit, int max);
	//

	//Functions for interaction with files
	bool write_to_binary_file(const char* file_name, const char* open_mode, void* data, int size_of_data, int how_many, int position_to_start, int relative_position);
	bool read_from_binary_file(const char* file_name, const char* open_mode, void* bufer, int size_of_data, int position_to_start, int relative_position);
	void write_to_binary_file(FILE* file, void* data, int size_of_data, int how_many, int position_to_start, int relative_position);
	//

	//looking for min or max
	float get_min(const float* table, const int size);
	float get_max(const float* table, const int size);
	//
	
	//collision functions
	bool correlations_between_objects_x(float x_1, sf::Vector2f x_min_max_2, const my::game_object& object1, const my::game_object& object2);
	bool correlations_between_objects_y(float y_1, sf::Vector2f y_min_max_2, const my::game_object& object1, const my::game_object& object2);
	bool collision_object_with_point(sf::Vector2i point_coordinates, const my::game_object& object);
	bool collision_between_objects(const my::game_object& object1, const my::game_object& object2);
	//

	//functions for changing colors
	bool change_color_simple(float* color_storage, const float color, float speed);
	bool change_color(float* color_storage1, const float color1, float* color_storage2, const float color2, float speed);
	//
	
	//console functions
	void is_console_visible(bool _bool);
	// 
	//others
	std::string get_current_date_and_time();
	void to_vice_versa(float* a, float* b);
	std::string int_to_string(int nummber);
	//
}
#endif