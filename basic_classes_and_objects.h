#pragma once
#ifndef BASIC_CLASSES_AND_OBJECTS_H
#define BASIC_CLASSES_AND_OBJECTS_H
//including external libraries
#include <SFML/Graphics.hpp>
#include <mutex>
//
namespace my {
	//structs
	typedef struct {
		char _string[25];
		int _scores;
	}scores_struct;

	//basic classes
	class object {
	protected:
		sf::Vector2f position;
		sf::Vector2f size;
		float scale;
		//dynamic methods
		virtual void update_size(float scale);
	public:
		//constructors
		object() {};
		object(float scale);
		//setters
		virtual void set_position(sf::Vector2f position);
		//getters
		virtual sf::Vector2f get_position() const;
		virtual sf::Vector2f get_size() const;
		virtual float get_scale() const;
		//distructor
		virtual ~object() {}
	};
	class transformable :public object {
	public:
		//constructors
		transformable() {}
		transformable(float scale) :object(scale) {}
		//setters
		virtual void set_scale(float scale);
		//dynamic functions
		virtual void move(sf::Vector2f position);
		virtual void scale_object(float scale);
		//distructor
		virtual ~transformable() {}
	};
	class drawable {
	protected:
		sf::Vector3f first_color, second_color;
	public:
		//constructors
		drawable() {}
		drawable(sf::Vector3f first_color, sf::Vector3f second_color): first_color(first_color), second_color(second_color){}
		//setters
		virtual void set_first_color(sf::Vector3f color) { first_color = color; }
		virtual void set_second_color(sf::Vector3f color) { second_color = color; }
		//getters
		virtual sf::Vector3f get_first_color()const { return first_color; }
		virtual sf::Vector3f get_second_color()const { return second_color; }
		//visual methods
		virtual void draw(sf::RenderWindow* window)const = 0;
		//distructor
		virtual ~drawable() {}
	};
	class game_object : public transformable, public drawable {
	protected:
		const float** data_address;
		int data_size;
		float outline_size;
		//dynamic methods
		virtual void set_size();
	public:
		//constructors
		game_object() {}
		game_object(const float** data_address, int data_size, float scale, float outline_size, sf::Vector3f first_color, sf::Vector3f second_color);
		//setters
		virtual void set_data_address(const float** address);
		virtual void set_data_size(int size);
		virtual void set_outline_size(float size);
		//getters
		virtual const float** get_data_address() const;
		virtual const int get_data_size() const;
		virtual float get_outline_size() const;
		//visual methods
		virtual void draw(sf::RenderWindow* window) const;
		virtual void draw_outline(sf::RenderWindow* window) const;
		//distructor
		virtual ~game_object() {}
	};
	class window :public object, public drawable {
	protected:
		std::mutex _mutex;
		sf::RenderWindow* where_this_window_will_be_drawn;
		bool _window_is_open, _computing_part_is_sleeping;
		//setters
		virtual void set_size(sf::Vector2f size);
		//visual methods
		virtual void draw_background()const = 0;
		virtual void draw()const = 0;
	public:
		//constructors
		window() {}
		window(sf::RenderWindow* window, float scale, sf::Vector3f first_color, sf::Vector3f second_color);
		//setters
		void set_window_state(bool state);
		//getters
		bool get_window_state()const;
		//viual methods
		virtual void draw(sf::RenderWindow* window) const;
		//distructor
		virtual ~window() {}
	};
	//
	
	//cactus class
	class cactus :public game_object {
	public:
		//constructors
		cactus() {}
		cactus(const float** data_address, const int data_size, float scale, float outline_size, sf::Vector3f first_color, sf::Vector3f second_color) :
			game_object(data_address, data_size, scale, outline_size, first_color, second_color) {}
		//distructor
		virtual ~cactus() {}
	};
	//dino classes
	class dino_part :public game_object {
	public:
		//constructors
		dino_part(const float** data_address, const int data_size, float scale, float outline_size, sf::Vector3f first_color, sf::Vector3f second_color) :
			game_object(data_address, data_size, scale, outline_size, first_color, second_color) {}
		//visual methods
		virtual void draw(sf::RenderWindow* window) const;
		//distructor
		virtual ~dino_part() {}
	};
	class dino : public transformable, public drawable {
	private:
		int stage;
		float jump_speed;
		dino_part* dino_parts[3];
		sf::Vector2f start_position;
		bool jump_flag;
		//dynamic methods
		void set_size();
	public:
		//constructors
		dino() {}
		dino(float scale, float outline_size, float jump_speed, sf::Vector3f first_color, sf::Vector3f second_color);
		//setters
		void set_stage(int stage_1_to_3);
		virtual void set_position(sf::Vector2f position);
		void set_start_position();
		void set_jump_speed(float jump_speed);
		virtual void set_first_color(sf::Vector3f color);
		virtual void set_second_color(sf::Vector3f color);
		//getters
		sf::Vector2f get_start_position() const;
		int get_stage() const;
		float get_jump_speed() const;
		const dino_part& get_dino_part(int in_which_stage) const;
		//dynamic methods
		bool jump();
		virtual void move(sf::Vector2f position);
		virtual void scale_object(float scale);
		void animate();
		//visual methods
		virtual void draw(sf::RenderWindow* window) const;
		virtual	void draw_outline(sf::RenderWindow* window) const;
		//distructor
		virtual ~dino() {}
	};
	//menu point class
	class menu_point : public game_object {
	private:
		sf::Text* text;
		sf::Font* font;
		bool animation, is_pressed;
		//dynamic methods
		void set_size();
		//visual methods
		void draw_background(sf::RenderWindow* window, float y_slip, float increase_size_by, sf::Vector3f color) const;
		void draw_text(sf::RenderWindow* window, float y_slip, sf::Vector3f color) const;
	public:
		//constructors
		menu_point() {}
		menu_point(const float** data_address, const int data_size, float scale, float outline_size, sf::Vector3f first_color, sf::Vector3f second_color, std::string string, int characters_size);
		//setters
		void set_state(bool is_pressed);
		//getters
		int get_character_size() const;
		std::string get_string() const;
		bool get_state() const;
		//dynamic methods
		bool collision_with_point(sf::Vector2i point_coordinates) const;
		void change_animation_flag(bool new_state);
		//visual methods
		virtual void draw(sf::RenderWindow* window) const;
		void draw_without_animation(sf::RenderWindow* window) const;
		virtual void draw_outline(sf::RenderWindow* window) const;
		void animate(sf::RenderWindow* window, float step) const;
		//distructor
		virtual ~menu_point() {}
	};
	//sand class
	class sand: public object , public drawable{
	private:
		int amount_of_grains, following;
		float size_of_grains, speed;
		float** grains;
		int compute_amount();
		float generate_horizontal_grains_position(int i) const;
		float generate_vertical_grains_position() const;
		void check_position();
		void generate_grains_position();
	public:
		//constructors
		sand() {}
		sand(sf::Vector2f window_size, float size, float scale, float speed, sf::Vector2f max_min_positions, sf::Vector3f color);
		//dynamic methods
		void update_speed(float speed);
		virtual void move();
		//visual methods
		virtual void draw(sf::RenderWindow* window) const;
		//distructor
		virtual ~sand() {}
	};
	//scores counter class
	class counter : public transformable, public drawable {
	private:
		int scores, hi_scores;
		std::string *scores_string, *hi_scores_string;
		sf::Font* font;
		sf::Text* current_scores, * hi;
		//dynamic methods
		void set_size();
		virtual void update_size(float scale);
		void update_text_position();
	public:
		//constructors
		counter() {}
		counter(float scale, unsigned int character_size, int hi_scores, sf::Vector3f first_color, sf::Vector3f second_color);
		//setters
		void set_hi_scores(int scores);
		virtual void set_position(sf::Vector2f position);
		//getters
		int get_scores() const;
		int get_hi_scores()const;
		//dynamic methods
		virtual void move(sf::Vector2f position);
		void update_hi();
		void add_to_scores(unsigned int volume);
		//visual methods
		virtual void draw(sf::RenderWindow* window) const;
		//distructor
		virtual ~counter() {}
	};
	//pause
	class pause : public object, public drawable {
	private:
		sf::Font* font;
		sf::Text* _pause, * _continue;
	public:
		//constructors
		pause() {}
		pause(float scale, unsigned int character_size, sf::Vector3f first_color, sf::Vector3f second_color);
		//setters
		virtual void set_position(sf::Vector2f position);
		//visual functions
		virtual void draw(sf::RenderWindow* window) const;
		//distructor
		virtual ~pause() {}
	};
	//help
	class help : public object, public drawable {
	private:
		sf::Font* font;
		sf::Text* text;
	public:
		//constructors
		help() {}
		help(float scale, unsigned int character_size, sf::Vector3f first_color, sf::Vector3f second_color);
		//setters
		virtual void set_position(sf::Vector2f position);
		//visual functions
		virtual void draw(sf::RenderWindow* window) const;
		//distructorq
		virtual ~help() {}
	};
	//quick exit
	class quick_exit : public window {
	private:
		sf::Font* font;
		sf::Text* text;
		menu_point* resume, * _exit;
		//visual methods
		virtual void draw_background()const;
		virtual void draw()const;
	public:
		//constructors
		quick_exit() {}
		quick_exit(sf::RenderWindow* window, float scale, sf::Vector3f first_color, sf::Vector3f second_color, int text_character_size, int menu_points_character_size);
		//setters
		virtual void set_position(sf::Vector2f position);
		//getters
		menu_point& get_menu_point(int which_1_or_2) const;
		//visual methods
		virtual void draw(sf::RenderWindow* window) const;
		//distructors
		virtual ~quick_exit() {}
	};
}
#endif
