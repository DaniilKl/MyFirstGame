#pragma once
#ifndef WINDOWS_H
#define WINDOWS_H
//including external libraries
#include <SFML/Audio.hpp>
#include <iostream>
#include <mutex>
//
//including my libraries
#include "basic_classes_and_objects.h"
//
namespace my {
	//main menu window
	class menu_window : public window {
	private:
		//objects
		sf::Text* header;
		sf::Font* font;
		menu_point* start;
		menu_point* exit;
		//textes
		sf::Text* hi;
		//flags
		bool is_file_exist;
		//others
		scores_struct* _struct;
		//getters
		menu_point& get_menu_point(int which_one) const;
		//dynamic methods
		void computing_part();
		//visual methods
		virtual void draw_background() const;
		virtual void draw() const;
	public:
		//constructor
		menu_window(sf::RenderWindow* window);
		//dynamic methods
		virtual bool execute_window();
		//distructor
		virtual ~menu_window() {}
	};
	// 
	
	//play window
	class play_window : public window {
	private:
		//objects
		dino* trex;
		cactus* cactuses;
		sf::RectangleShape* line;
		sand* sand_pieces;
		counter* scores_counter;
		pause* _pause;
		help* _help;
		quick_exit* _exit;
		//sounds
			//buffers
			sf::SoundBuffer* jump_sound_buffer;
			sf::SoundBuffer* fail_sound_buffer;
			//
			sf::Sound* jump_sound;
			sf::Sound* fail_sound;
		//clocks
		sf::Clock* dino_animation_clock;
		sf::Clock* dino_jump_clock;
		sf::Clock* sand_animation_clock;
		sf::Clock* scores_add_clock;
		sf::Clock* color_changing_clock;
		//flags
		bool* cactus_moving_permission;
		bool color_change_flag, dino_jump_flag, pause_flag, help_flag, exit_flag, is_cursore_visible, is_file_exist;
		//others
		scores_struct* _struct;
		float min_addition_distance, max_addition_distance,speed;
		int amount_of_cactuses;
		unsigned int help_counter_for_changing_colors;
		//protected methods
			//setters
			virtual void set_first_color(sf::Vector3f color);
			virtual void set_second_color(sf::Vector3f color);
			//dynamic methods
			int compute_amount_of_cactuses();
			float compute_dino_jump_distance()const;
			void check_next_cactus_moving_status(int current_cactus_index)const;
			void move_cactuses();
			bool check_collision(const dino_part& trex, bool function_for_collision(const game_object&, const game_object&)) const;
			virtual void computing_part(bool *out_state);
			//visual methods
			virtual void draw() const;
			virtual void draw_background() const;
		//
	public:
		//constructors
		play_window() {}
		play_window(sf::RenderWindow* window);
		//dynamic methods
		bool execute_window();
		//distructor
		virtual ~play_window() {}
	};
}
#endif