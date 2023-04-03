//including external libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <mutex>
//
//including my libraries
#include "other.h"
#include "windows.h"
#include "drawings.h"
//
//defining external const
	//speeds
	#define COLOR_CHANGING_SPEED 1
	#define SPEED 0.5
	//character sizes
	#define HEADER_CHARACTER_SIZE 200
	#define MENU_POINTS_CHARACTER_SIZE 40
	#define PAUSE_CHARACTER_SIZE 200
	#define HELP_CHARACTER_SIZE 50
	#define QUICK_EXIT_TEXT_CHARACTER_SIZE 50
	//others
	#define WINDOWS_SCALE 1
	#define FIRST_COLOR 15
	#define SECOND_COLOR 232
	#define UP_COLOR_CHANGING_BOUND 200
	#define DOWN_COLOR_CHANGING_BOUND 100
	#define DINO_OUTLINE_SIZE 5
	#define GRAINS_SIZE 2
	//
//
namespace my {
	//menu window methods
	void menu_window::draw_background() const {
		sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(this->size.x, this->size.y));
		rect->setFillColor(sf::Color(this->second_color.x, this->second_color.y, this->second_color.z));
		where_this_window_will_be_drawn->draw(*rect);
		delete(rect);
	}
	void menu_window::draw()const {
		where_this_window_will_be_drawn->draw(*header);
		start->draw(where_this_window_will_be_drawn);
		exit->draw(where_this_window_will_be_drawn);
		if (is_file_exist) {
			hi->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
			where_this_window_will_be_drawn->draw(*hi);
		}
	}
	menu_point& menu_window::get_menu_point(int which_one) const {
		if (which_one == 1)
			return *start;
		else if (which_one == 2)
			return *exit;
	}
	void menu_window::computing_part() {
		menu_point* _start = new menu_point;
		menu_point* _exit = new menu_point;
		_mutex.lock();
		*_start = *start;
		*_exit = *exit;
		_mutex.unlock();
		do {
			//collisions
			if (_start->collision_with_point(sf::Mouse::getPosition())) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					_mutex.lock();
					start->set_state(true);
					_window_is_open = false;
					_mutex.unlock();
					break;
				}
				else
					_start->change_animation_flag(true);
			}
			else
				_start->change_animation_flag(false);
			if (_exit->collision_with_point(sf::Mouse::getPosition())) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					_mutex.lock();
					exit->set_state(true);
					_window_is_open = false;
					_mutex.unlock();
					break;
				}
				else
					_exit->change_animation_flag(true);
			}
			else
				_exit->change_animation_flag(false);
			//return data to the main thread
			_mutex.lock();
			*start = *_start;
			*exit = *_exit;
			_mutex.unlock();
		} while (true);
	}
	menu_window::menu_window(sf::RenderWindow* window) :
		window(window, WINDOWS_SCALE, sf::Vector3f(FIRST_COLOR, FIRST_COLOR, FIRST_COLOR), sf::Vector3f(SECOND_COLOR, SECOND_COLOR, SECOND_COLOR)) {
		//font
		font = new sf::Font;
		font->loadFromFile("font\\upheavtt.ttf");
		//header
		header = new sf::Text();
		header->setFont(*font);
		header->setString("DINO");
		header->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
		header->setCharacterSize(HEADER_CHARACTER_SIZE);
		//buttons
		header->setPosition(size.x / 2 - header->getLocalBounds().width / 2, (size.y / 100) * 16);
		start = new menu_point(menu_point_drawing, menu_point_drawing_size, scale, 0, first_color, second_color, "start", MENU_POINTS_CHARACTER_SIZE);
		exit = new menu_point(menu_point_drawing, menu_point_drawing_size, scale, 0, first_color, second_color, "exit", MENU_POINTS_CHARACTER_SIZE);
		start->set_position(sf::Vector2f(size.x / 2 - start->get_size().x / 2, header->getPosition().y + header->getCharacterSize() * header->getScale().y / 2 + size.y * 11 / 100));
		exit->set_position(sf::Vector2f(size.x / 2 - exit->get_size().x / 2, header->getPosition().y + header->getCharacterSize() * header->getScale().y / 2 + start->get_size().y + size.y * 13 / 100));
		//flags
		_struct = new scores_struct;
		//load scores frome file
		hi = new sf::Text;
		is_file_exist = false;
		std::cout << "Start" << std::endl;
		if (read_from_binary_file("hi.dat", "r+b", _struct, sizeof(scores_struct), 0, 0)) {
			is_file_exist = true;
			std::string _string("Hi: ");
			_string.append(int_to_string(_struct->_scores));
			_string.append(", ");
			_string.append(_struct->_string);
			hi->setString(_string);
			hi->setFont(*font);
			hi->setCharacterSize(MENU_POINTS_CHARACTER_SIZE / 2);
			hi->setPosition(size.x / 2 - hi->getLocalBounds().width/2, exit->get_position().y + exit->get_size().y+5);
		}
	}
	bool menu_window::execute_window() {
		where_this_window_will_be_drawn->setFramerateLimit(30);
		where_this_window_will_be_drawn->setMouseCursorVisible(true);
		std::thread computing_thread(&menu_window::computing_part,this);
		computing_thread.detach();
		while (_window_is_open) {
			sf::Event event;
			while (where_this_window_will_be_drawn->pollEvent(event)) { //event loop
				switch (event.type) {
				case sf::Event::EventType::KeyPressed:
					if (event.key.code == sf::Keyboard::Key::Escape)
						return false;
				}
			}
			//drawing
			window::draw(where_this_window_will_be_drawn);
		}
		return get_menu_point(1).get_state();
	}

	//play window methods
	void play_window::set_first_color(sf::Vector3f color) {
		first_color = color;
		sand_pieces->set_first_color(color);
		line->setFillColor(sf::Color(color.x, color.y, color.z));
		trex->set_first_color(color);
		scores_counter->set_first_color(color);
		_pause->set_first_color(color);
		_help->set_first_color(color);
		_exit->set_first_color(color);
		for (int i = 0; i < amount_of_cactuses; i++)
			cactuses[i].set_first_color(color);
	}
	void play_window::set_second_color(sf::Vector3f color) {
		second_color = color;
		trex->set_second_color(color);
		scores_counter->set_second_color(color);
		_exit->set_second_color(color);
		for (int i = 0; i < amount_of_cactuses; i++)
			cactuses[i].set_second_color(color);
	}
	int play_window::compute_amount_of_cactuses() { return size.x / (compute_dino_jump_distance() + 95 * scale + min_addition_distance); }
	float play_window::compute_dino_jump_distance()const { return speed * (2.5 * trex->get_size().x) / trex->get_jump_speed(); }
	void play_window::check_next_cactus_moving_status(int current_cactus_index)const {
		int temp;
		if (current_cactus_index == amount_of_cactuses - 1)
			temp = 0;
		else
			temp = current_cactus_index + 1;
		if ((cactuses[current_cactus_index].get_position().x <= size.x-cactuses[current_cactus_index].get_size().x + compute_dino_jump_distance() + min_addition_distance) && !cactus_moving_permission[temp]) {
			cactuses[temp].set_position(sf::Vector2f(cactuses[current_cactus_index].get_position().x + compute_dino_jump_distance()
				+ speed*0.8 * rand_float_number(min_addition_distance, max_addition_distance), cactuses[temp].get_position().y));
			cactus_moving_permission[temp] = true;
		}
	}
	void play_window::move_cactuses() {
		for (int i = 0; i < amount_of_cactuses; i++) {
			if (cactuses[i].get_position().x <= -cactuses[i].get_size().x) {
				cactuses[i].move(sf::Vector2f(size.x - cactuses[i].get_position().x, 0));
				cactus_moving_permission[i] = false;
			}
			else if (cactus_moving_permission[i] == true) {
				cactuses[i].move(sf::Vector2f(-speed, 0));
				check_next_cactus_moving_status(i);
			}
		}
	}
	bool play_window::check_collision(const dino_part& trex, bool function_for_collision(const game_object&, const game_object&)) const {
		for (int i = 0; i < amount_of_cactuses; i++)
			if (function_for_collision(trex, cactuses[i]))
				return true;
		return false;
	}
	void play_window::computing_part(bool *out_state) {
		sf::Clock sleep_clock;
		menu_point* temp_resume = new menu_point;
		menu_point* temp_exit = new menu_point;
		//colors
		float first_color1 = first_color.x;
		float second_color1 = second_color.x;
		float first_color2 = second_color.x;
		float second_color2 = first_color.x;
		while (_window_is_open) {
			if (exit_flag){
				_mutex.lock();
				is_cursore_visible = true;
				*temp_resume = _exit->get_menu_point(1);
				*temp_exit = _exit->get_menu_point(2);
				_mutex.unlock();
				if (_exit->get_menu_point(1).collision_with_point(sf::Mouse::getPosition())) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						exit_flag = !exit_flag;
						temp_resume->set_state(true);
						_mutex.lock();
						is_cursore_visible = false;
						_mutex.unlock();
					}
					else
						temp_resume->change_animation_flag(true);
				}
				else
					temp_resume->change_animation_flag(false);
				if (_exit->get_menu_point(2).collision_with_point(sf::Mouse::getPosition())) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						_mutex.lock();
						temp_exit->set_state(true);
						*out_state = false;
						_window_is_open = false;
						_mutex.unlock();
						delete(temp_resume);
						delete(temp_exit);
						break;
					}
					else
						temp_exit->change_animation_flag(true);
				}
				else
					temp_exit->change_animation_flag(false);
				_mutex.lock();
				_exit->get_menu_point(1) = *temp_resume;
				_exit->get_menu_point(2) = *temp_exit;
				_mutex.unlock();
			}
			else {
				//pause
				while (pause_flag || help_flag) {
					sleep_clock.restart();
				}
				//animations
				if (sand_animation_clock->getElapsedTime().asMicroseconds() >= 1000.0f) {
					sand_animation_clock->restart();
					_mutex.lock();
					sand_pieces->move();
					move_cactuses();
					_mutex.unlock();
					speed += 0.00001;
					sand_pieces->update_speed(speed);
				}
				if (dino_animation_clock->getElapsedTime().asMicroseconds() >= 150000.0f && dino_jump_flag) {
					dino_animation_clock->restart();
					_mutex.lock();
					trex->animate();
					_mutex.unlock();
				}
				if (scores_add_clock->getElapsedTime().asMicroseconds() > 100000.0f) {
					scores_add_clock->restart();
					_mutex.lock();
					scores_counter->add_to_scores(1);
					_mutex.unlock();
				}
				if (!dino_jump_flag && dino_jump_clock->getElapsedTime().asMicroseconds() >= 15900.0f) {
					dino_jump_clock->restart();
					_mutex.lock();
					trex->set_stage(1);
					dino_jump_flag = trex->jump();
					_mutex.unlock();
				}
				if (scores_counter->get_scores() % (help_counter_for_changing_colors * rand_int_number(DOWN_COLOR_CHANGING_BOUND, UP_COLOR_CHANGING_BOUND)) == 0 && scores_counter->get_scores() != 0) {
					color_change_flag = false;
					if (help_counter_for_changing_colors != 1)
						to_vice_versa(&first_color2, &second_color2);
					help_counter_for_changing_colors++;
				}
				if (!color_change_flag && color_changing_clock->getElapsedTime().asMicroseconds() >= 7000.0f) {
					color_changing_clock->restart();
					color_change_flag = change_color(&first_color1, first_color2, &second_color1, second_color2, COLOR_CHANGING_SPEED);
					_mutex.lock();
					set_first_color(sf::Vector3f(first_color1, first_color1, first_color1));
					set_second_color(sf::Vector3f(second_color1, second_color1, second_color1));
					_mutex.unlock();
				}
				//collisions
				if (check_collision(trex->get_dino_part(0), collision_between_objects)) {
					fail_sound->play();
					_mutex.lock();
					_window_is_open = false;
					_mutex.unlock();
					break;
				}
			}
		}
	}
	void play_window::draw() const {
		if (pause_flag)
			_pause->draw(where_this_window_will_be_drawn);
		else if (help_flag)
			_help->draw(where_this_window_will_be_drawn);
		sand_pieces->draw(where_this_window_will_be_drawn);
		where_this_window_will_be_drawn->draw(*line);
		trex->draw_outline(where_this_window_will_be_drawn);
		for (int i = 0; i < amount_of_cactuses; i++)
			cactuses[i].draw_outline(where_this_window_will_be_drawn);
		trex->draw(where_this_window_will_be_drawn);
		for (int i = 0; i < amount_of_cactuses; i++)
			cactuses[i].draw(where_this_window_will_be_drawn);
		scores_counter->draw(where_this_window_will_be_drawn);
		if (exit_flag)
			_exit->draw(where_this_window_will_be_drawn);
	}
	void play_window::draw_background() const {
		sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(size.x, size.y));
		rect->setFillColor(sf::Color(second_color.x, second_color.y, second_color.z));
		where_this_window_will_be_drawn->draw(*rect);
		delete(rect);
	}
	play_window::play_window(sf::RenderWindow* window) :
		window(window, WINDOWS_SCALE, sf::Vector3f(FIRST_COLOR, FIRST_COLOR, FIRST_COLOR), sf::Vector3f(SECOND_COLOR, SECOND_COLOR, SECOND_COLOR)) {
		//variables
		speed = SPEED;
		help_counter_for_changing_colors = 1;
		//line
		line = new sf::RectangleShape(sf::Vector2f(size.x, 5 * scale * 0.4));
		line->setPosition(0, size.y * 9 / 10);
		line->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
		//dino
		trex = new dino(scale * 0.4, DINO_OUTLINE_SIZE, 14, first_color, second_color);
		trex->set_position(sf::Vector2f(size.x / 10, line->getPosition().y * (1 + 0.05 * scale * 0.4) - trex->get_size().y));
		trex->set_start_position();
		//sand
		sand_pieces = new sand(size, GRAINS_SIZE, scale, SPEED, sf::Vector2f(trex->get_position().y + trex->get_size().y * (1 + 0.2), line->getPosition().y + line->getSize().y), first_color);
		//cactuses
		amount_of_cactuses = compute_amount_of_cactuses();
		cactuses = new cactus[amount_of_cactuses];
		cactus_moving_permission = new bool[amount_of_cactuses];
		for (int i = 0; i < amount_of_cactuses; i++) {
			cactuses[i] = cactus(cactus_drawing1, cactus_drawing1_size, scale * 0.4, DINO_OUTLINE_SIZE, first_color, second_color);
			cactus_moving_permission[i] = false;
			cactuses[i].set_position(sf::Vector2f(size.x, trex->get_position().y + trex->get_size().y - cactuses[i].get_size().y));
		}
		cactus_moving_permission[0] = true;
		min_addition_distance = 9*trex->get_size().x;
		max_addition_distance = 18 * trex->get_size().x;
		//read from file 
		_struct = new scores_struct;
		int _hi_scores;
		if (read_from_binary_file("hi.dat", "r+b", _struct, sizeof(scores_struct), 0, 0))
			_hi_scores = _struct->_scores;
		else
			_hi_scores = 0;
		std::cout << _hi_scores;
		//scores counter
		scores_counter = new counter(scale, 20, _hi_scores, first_color, second_color);
		scores_counter->set_position(sf::Vector2f(size.x / 10, size.y / 8));
		//pause
		_pause = new pause(scale, PAUSE_CHARACTER_SIZE, first_color, second_color);
		_pause->set_position(sf::Vector2f(size.x / 2, size.y / 3));
		//help
		_help = new help(scale, HELP_CHARACTER_SIZE, first_color, second_color);
		_help->set_position(sf::Vector2f(size.x / 2, size.y / 5));
		//quick exit 
		_exit = new quick_exit(window, scale, first_color, second_color, QUICK_EXIT_TEXT_CHARACTER_SIZE, MENU_POINTS_CHARACTER_SIZE);
		_exit->set_position(sf::Vector2f(size.x / 2, size.y / 3));
		//sounds
			//buffers
			jump_sound_buffer = new sf::SoundBuffer;
			jump_sound_buffer->loadFromFile("audio\\jump.ogg");
			fail_sound_buffer = new sf::SoundBuffer;
			fail_sound_buffer->loadFromFile("audio\\fail.ogg");
			//
			jump_sound = new sf::Sound;
			jump_sound->setBuffer(*jump_sound_buffer);
			fail_sound = new sf::Sound;
			fail_sound->setBuffer(*fail_sound_buffer);
		//clocks
		dino_animation_clock = new sf::Clock;
		dino_jump_clock = new sf::Clock;
		sand_animation_clock = new sf::Clock;
		scores_add_clock = new sf::Clock;
		color_changing_clock = new sf::Clock;
		//flags
		dino_jump_flag = true;
		color_change_flag = true;
		pause_flag = false;
		help_flag = false;
		exit_flag = false;
		is_cursore_visible = false;
	}
	bool play_window::execute_window() {
		bool out_state = true;
		where_this_window_will_be_drawn->setFramerateLimit(0);
		//threads
		std::thread computing_thread(&play_window::computing_part, this, &out_state);
		computing_thread.detach();
		while (_window_is_open) {
			where_this_window_will_be_drawn->setMouseCursorVisible(is_cursore_visible);
			sf::Event event;
			while (where_this_window_will_be_drawn->pollEvent(event)) { //event loop
				switch (event.type) {
				case sf::Event::EventType::KeyPressed:
					if (event.key.code == sf::Keyboard::Key::Escape) {
						exit_flag = !exit_flag;
						pause_flag = false;
						help_flag = false;
					}
					if (event.key.code == sf::Keyboard::Key::M) {
						_mutex.lock();
						_window_is_open = false; 
						_mutex.unlock();
						out_state = true;
					}
					if (event.key.code == sf::Keyboard::Key::Up && dino_jump_flag && !pause_flag && !help_flag && !exit_flag) {
						jump_sound->play();
						_mutex.lock();
						dino_jump_flag = false;
						_mutex.unlock();
					}
					if (event.key.code == sf::Keyboard::Key::Left)
						where_this_window_will_be_drawn->setFramerateLimit(30);
					if (event.key.code == sf::Keyboard::Key::Right)
						where_this_window_will_be_drawn->setFramerateLimit(0);
					if (event.key.code == sf::Keyboard::Key::P && !help_flag && !exit_flag) {
						_mutex.lock();
						pause_flag=!pause_flag;
						_mutex.unlock();
					}
					if (event.key.code == sf::Keyboard::Key::F1 && !pause_flag && !exit_flag) {
						_mutex.lock();
						help_flag = !help_flag;
						_mutex.unlock();
					}
				}
			}
			//drawing
			window::draw(where_this_window_will_be_drawn);
		}
		if (scores_counter->get_scores() >= _struct->_scores) {
			std::string temp_string(get_current_date_and_time());
			_struct->_scores = scores_counter->get_scores();
			for (int i = 24; i >= 0; i--)
				_struct->_string[i] = temp_string[i];
			write_to_binary_file("hi.dat", "w+b", _struct, sizeof(scores_struct), 1, 0, 0);
		}
		return out_state;
	}
	//
}