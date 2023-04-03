#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
//including external libraries
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <Windows.h>
//
//including my libraries
#include "basic_classes_and_objects.h"
#include "other.h"
//
namespace my {
	//Functions for generating random numbers
	/*template <typename type>
	type rand_number(type min, type max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> num(min, max);
		return num(gen);
	}
	template <typename type>
	type rand_number(type min, type ommit, type max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		while (1) {
			std::uniform_int_distribution<> num(min, max);
			type number = num(gen);
			if (number != ommit)
				return number;
		}
	}*/
	float rand_float_number(float min, float max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> num(min, max);
		return num(gen);
	}
	float rand_float_number(float min, float ommit, float max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		while (1) {
			std::uniform_int_distribution<> num(min, max);
			float number = num(gen);
			if (number != ommit)
				return number;
		}
	}
	int rand_int_number(int min, int max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> num(min, max);
		return num(gen);
	}
	int rand_int_number(int min, int ommit, int max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		while (true) {
			std::uniform_int_distribution<> num(min, max);
			int number = num(gen);
			if (number != ommit)
				return number;
		}
	}
	//

	//Functions for interaction with files
	void write_to_binary_file(FILE* file, void* data, int size_of_data, int how_many, int position_to_start, int relative_position) {
		fseek(file, position_to_start * size_of_data, relative_position);
		/*std::cout << "Point file position before writing: " << ftell(file) << std::endl;*/
		for (int i = 0; i < how_many; i++)
			fwrite(data, size_of_data, 1, file);
		/*std::cout <<"Point file position: " << ftell(file)<<std::endl;*/
	}
	bool write_to_binary_file(const char* file_name, const char* open_mode, void* data, int size_of_data, int how_many, int position_to_start, int relative_position) {
		FILE* file;
		file = fopen(file_name, open_mode);
		if (file == NULL)
			return false;
		long temp = position_to_start * size_of_data;
		fseek(file, temp, relative_position);
		/*std::cout << "Point file position before writing: " << ftell(file) << std::endl;*/
		for (int i = 0; i < how_many; i++)
			fwrite(data, size_of_data, 1, file);
		/*std::cout <<"Point file position: " << ftell(file)<<std::endl;*/
		fclose(file);
		return true;
	}
	bool read_from_binary_file(const char* file_name, const char* open_mode, void* bufer, int size_of_data, int position_to_start, int relative_position) {
		FILE* file;
		file = fopen(file_name, open_mode);
		if (file == NULL)
			return false;
		long temp = position_to_start * size_of_data;
		fseek(file, temp, relative_position);
		/*std::cout << "Point file position before reading: " << ftell(file) << std::endl;*/
		fread(bufer, size_of_data, 1, file);
		/*std::cout << "Point file position: " << ftell(file) << std::endl;*/
		fclose(file);
		return true;
	}
	//
	
	//looking for min or max
	float get_min(const float* table, const int size) {
		float min = *table;
		for (int i = 0; i < size; i++)
			if (*(table + i) < min)
				min = *(table + i);
		return min;
	}
	float get_max(const float* table, const int size) {
		float max = *table;
		for (int i = 0; i < size; i++)
			if (*(table + i) > max)
				max = *(table + i);
		return max;
	}
	//

	//collision functions
	bool correlations_between_objects_x(float x_1, sf::Vector2f x_min_max_2, const my::game_object& object1, const my::game_object& object2) {
		if (x_1 * object1.get_scale() + object1.get_position().x >= x_min_max_2.x * object2.get_scale() + object2.get_position().x && x_1 * object1.get_scale() + object1.get_position().x <= x_min_max_2.y * object2.get_scale() + object2.get_position().x)
			return true;
		else
			return false;
	}
	bool correlations_between_objects_y(float y_1, sf::Vector2f y_min_max_2, const my::game_object& object1, const my::game_object& object2) {
		if (y_1 * object1.get_scale() + object1.get_position().y >= y_min_max_2.x * object2.get_scale() + object2.get_position().y && y_1 * object1.get_scale() + object1.get_position().y <= y_min_max_2.y * object2.get_scale() + object2.get_position().y)
			return true;
		else
			return false;
	}
	bool collision_object_with_point(sf::Vector2i point_coordinates, const my::game_object& object) {
		const float** data = object.get_data_address();
		const int data_size = object.get_data_size();
		float x_min = get_min(data[0], data_size), y_min = get_min(data[1], data_size), x_max = get_max(data[2], data_size), y_max = get_max(data[3], data_size);
		if ((point_coordinates.x >= x_min * object.get_scale() + object.get_position().x && point_coordinates.y >= y_min * object.get_scale() + object.get_position().y)
			&& (point_coordinates.x <= x_max * object.get_scale() + object.get_position().x && point_coordinates.y <= y_max * object.get_scale() + object.get_position().y)) {
			for (int i = 0; i < data_size; i++)
				if ((point_coordinates.x >= *(*(data)+i) * object.get_scale() + object.get_position().x && point_coordinates.y >= *(*(data + 1) + i) * object.get_scale() + object.get_position().y)
					&& (point_coordinates.x <= *(*(data + 2) + i) * object.get_scale() + object.get_position().x && point_coordinates.y <= *(*(data + 3) + i) * object.get_scale() + object.get_position().y))
					return true;
			return false;
		}
		else
			return false;
	}
	bool collision_between_objects(const my::game_object& object1,const my::game_object& object2) {
		const float** data1 = object1.get_data_address();
		const float** data2 = object2.get_data_address();
		const int data1_size = object1.get_data_size(), data2_size = object2.get_data_size();
		float x_min1 = get_min(data1[0], data1_size), y_min1 = get_min(data1[1], data1_size), x_max1 = get_max(data1[2], data1_size), y_max1 = get_max(data1[3], data1_size);
		float x_min2 = get_min(data2[0], data2_size), y_min2 = get_min(data2[1], data2_size), x_max2 = get_max(data2[2], data2_size), y_max2 = get_max(data2[3], data2_size);
		if (((x_min1 * object1.get_scale() + object1.get_position().x >= x_min2 * object2.get_scale() + object2.get_position().x && x_min1 * object1.get_scale() + object1.get_position().x <= x_max2 * object2.get_scale() + object2.get_position().x)
			|| (x_max1 * object1.get_scale() + object1.get_position().x >= x_min2 * object2.get_scale() + object2.get_position().x && x_max1 * object1.get_scale() + object1.get_position().x <= x_max2 * object2.get_scale() + object2.get_position().x))
			&& ((y_min1 * object1.get_scale() + object1.get_position().y >= y_min2 * object2.get_scale() + object2.get_position().y && y_min1 * object1.get_scale() + object1.get_position().y <= y_max2 * object2.get_scale() + object2.get_position().y)
				|| (y_max1 * object1.get_scale() + object1.get_position().y >= y_min2 * object2.get_scale() + object2.get_position().y && y_max1 * object1.get_scale() + object1.get_position().y <= y_max2 * object2.get_scale() + object2.get_position().y))) {
			for (int i = 0; i < data1_size; i++)
				for (int j = 0; j < data2_size; j++)
					if (((*(*(data1)+i) * object1.get_scale() + object1.get_position().x >= *(*(data2)+j) * object2.get_scale() + object2.get_position().x && *(*(data1)+i) * object1.get_scale() + object1.get_position().x <= *(*(data2 + 2) + j) * object2.get_scale() + object2.get_position().x)
						|| (*(*(data1 + 2) + i) * object1.get_scale() + object1.get_position().x >= *(*(data2)+j) * object2.get_scale() + object2.get_position().x && *(*(data1 + 2) + i) * object1.get_scale() + object1.get_position().x <= *(*(data2 + 2) + j) * object2.get_scale() + object2.get_position().x))
						&& ((*(*(data1 + 1) + i) * object1.get_scale() + object1.get_position().y >= *(*(data2 + 1) + j) * object2.get_scale() + object2.get_position().y && *(*(data1 + 1) + i) * object1.get_scale() + object1.get_position().y <= *(*(data2 + 3) + j) * object2.get_scale() + object2.get_position().y)
							|| (*(*(data1 + 3) + i) * object1.get_scale() + object1.get_position().y >= *(*(data2 + 1) + j) * object2.get_scale() + object2.get_position().y && *(*(data1 + 3) + i) * object1.get_scale() + object1.get_position().y <= *(*(data2 + 3) + j) * object2.get_scale() + object2.get_position().y)))
						return true;
			return false;
		}
		else if (((x_min2 * object2.get_scale() + object2.get_position().x >= x_min1 * object1.get_scale() + object1.get_position().x && x_min2 * object2.get_scale() + object2.get_position().x <= x_max1 * object1.get_scale() + object1.get_position().x)
			|| (x_max2 * object2.get_scale() + object2.get_position().x >= x_min1 * object1.get_scale() + object1.get_position().x && x_max2 * object2.get_scale() + object2.get_position().x <= x_max1 * object1.get_scale() + object1.get_position().x))
			&& ((y_min2 * object2.get_scale() + object2.get_position().y >= y_min1 * object1.get_scale() + object1.get_position().y && y_min2 * object2.get_scale() + object2.get_position().y <= y_max1 * object1.get_scale() + object1.get_position().y)
				|| (y_max2 * object2.get_scale() + object2.get_position().y >= y_min1 * object1.get_scale() + object1.get_position().y && y_max2 * object2.get_scale() + object2.get_position().y <= y_max1 * object1.get_scale() + object1.get_position().y))) {
			for (int i = 0; i < data2_size; i++)
				for (int j = 0; j < data1_size; j++)
					if (((*(*(data2)+i) * object2.get_scale() + object2.get_position().x >= *(*(data1)+j) * object1.get_scale() + object1.get_position().x && *(*(data2)+i) * object2.get_scale() + object2.get_position().x <= *(*(data1 + 2) + j) * object1.get_scale() + object1.get_position().x)
						|| (*(*(data2 + 2) + i) * object2.get_scale() + object2.get_position().x >= *(*(data1)+j) * object1.get_scale() + object1.get_position().x && *(*(data2 + 2) + i) * object2.get_scale() + object2.get_position().x <= *(*(data1 + 2) + j) * object1.get_scale() + object1.get_position().x))
						&& ((*(*(data2 + 1) + i) * object2.get_scale() + object2.get_position().y >= *(*(data1 + 1) + j) * object1.get_scale() + object1.get_position().y && *(*(data2 + 1) + i) * object2.get_scale() + object2.get_position().y <= *(*(data1 + 3) + j) * object1.get_scale() + object1.get_position().y)
							|| (*(*(data2 + 3) + i) * object2.get_scale() + object2.get_position().y >= *(*(data1 + 1) + j) * object1.get_scale() + object1.get_position().y && *(*(data2 + 3) + i) * object2.get_scale() + object2.get_position().y <= *(*(data1 + 3) + j) * object1.get_scale() + object1.get_position().y)))
						return true;
			return false;
		}
		else
			return false;
	}
	//

	//functions for changing colors
	bool change_color_simple(float* color_storage, const float color, float speed) {
		float _speed = speed;
		if (float dif = abs(color - *color_storage) < _speed)
			_speed = dif;
		if (*color_storage < color)
			*color_storage += _speed;
		else if (*color_storage > color)
			*color_storage -= _speed;
		if (*color_storage == color)
			return true;
		else
			return false;
	}
	bool change_color(float* color_storage1, const float color1, float* color_storage2, const float color2, float speed) {
		bool _1, _2;
		_1 = change_color_simple(color_storage1, color1, speed);
		_2 = change_color_simple(color_storage2, color2, speed);
		if (_1 == true && _2 == true)
			return true;
		else
			return false;
	}
	//

	//console functions
	void is_console_visible(bool _bool){
		if (_bool)
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		else
			ShowWindow(GetConsoleWindow(), SW_HIDE);
	}
	//

	//others
	std::string get_current_date_and_time() {
		std::time_t result = std::time(nullptr);
		return std::ctime(&result);
	}
	void to_vice_versa(float* a, float* b) {
		float temp = *a;
		*a = *b;
		*b = temp;
	}
	std::string int_to_string(int number) {
		int _number = number;
		char _char;
		int it = 0;
		std::string string1, string2;
		while (true) {
			_char = _number % 10 + 48;
			_number /= 10;
			string1.push_back(_char);
			if (_number == 0) {
				while (it != 0) {
					string2.push_back(string1[it--]);
				}
				string2.push_back(string1[0]);
				return string2;
			}
			it++;
		}
	}
	//
}