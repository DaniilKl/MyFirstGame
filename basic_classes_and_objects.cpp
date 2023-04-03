#define _CRT_SECURE_NO_WARNINGS
//including external libraries
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
//
//including my libraries
#include "other.h"
#include "basic_classes_and_objects.h"
#include "drawings.h"
//
namespace my {
	//basic classes methods & constructors
		//object 
		void object::update_size(float scale) { size *= scale; };
		object::object(float _scale) {
			position = sf::Vector2f(0, 0);
			size = sf::Vector2f(0, 0);
			scale = _scale;
		}
		void object::set_position(sf::Vector2f _position) { position = _position; }
		sf::Vector2f object::get_position() const { return position; }
		sf::Vector2f object::get_size() const { return size; }
		float object::get_scale() const { return scale; }
		//
		//transformable
		void transformable::set_scale(float _scale) {
		update_size(1 / scale);
		scale = _scale;
		update_size(scale);
		}
		void transformable::move(sf::Vector2f _position) { position += _position; }
		void transformable::scale_object(float _scale) {
		update_size(1 / scale);
		scale += _scale;
		update_size(scale);
		}
		//
		//game_object
		void game_object::set_size() {
			size.x = scale * get_max(data_address[2], data_size);
			size.y = scale * get_max(data_address[3], data_size);
		}
		game_object::game_object(const float** _data_address, int _data_size, float _scale, float _outline_size, sf::Vector3f _first_color, sf::Vector3f _second_color) :
			transformable(_scale),
			drawable(_first_color, _second_color)
		{
			data_address = _data_address;
			data_size = _data_size;
			outline_size = _outline_size;
			set_size();
		}
		void game_object::set_data_address(const float** _address) { data_address = _address; }
		void game_object::set_data_size(int _size) { data_size = _size; }
		void game_object::set_outline_size(float _size) { outline_size = _size; }
		const float** game_object::get_data_address() const { return data_address; }
		const int game_object::get_data_size() const { return data_size; }
		float game_object::get_outline_size() const { return outline_size; }
		void game_object::draw_outline(sf::RenderWindow* window) const {
			for (int i = 0; i < data_size; i++) {
				sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f((*(*(data_address + 2) + i) - *(*(data_address)+i) + 2 * outline_size) * scale, (*(*(data_address + 3) + i) - *(*(data_address + 1) + i) + 2 * outline_size) * scale));
				rect->setPosition(sf::Vector2f((*(*(data_address)+i) - outline_size) * scale + position.x, (*(*(data_address + 1) + i) - outline_size) * scale + position.y));
				rect->setFillColor(sf::Color(second_color.x, second_color.y, second_color.z));
				window->draw(*rect);
				delete(rect);
			}
		}
		void game_object::draw(sf::RenderWindow* window) const {
			for (int i = 0; i < data_size; i++) {
				sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f((*(*(data_address + 2) + i) - *(*(data_address)+i)) * scale, (*(*(data_address + 3) + i) - *(*(data_address + 1) + i)) * scale));
				rect->setPosition(sf::Vector2f(*(*(data_address)+i) * scale + position.x, *(*(data_address + 1) + i) * scale + position.y));
				rect->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
				window->draw(*rect);
				delete(rect);
			}
		}
		//
		//window
		void window::set_size(sf::Vector2f _size) { size = _size * scale; }
		window::window(sf::RenderWindow* window, float scale, sf::Vector3f first_color, sf::Vector3f second_color) :
			object(scale),
			drawable(first_color, second_color)
		{
			where_this_window_will_be_drawn = window;
			_window_is_open = true;
			_computing_part_is_sleeping = false;
			position = sf::Vector2f(0, 0);
			set_size(sf::Vector2f(window->getSize().x, window->getSize().y));
		}
		void window::set_window_state(bool state) { _window_is_open = state; }
		bool window::get_window_state()const { return _window_is_open; }
		void window::draw(sf::RenderWindow* window) const {
			window->clear(sf::Color::Blue);
			draw_background();
			draw();
			window->display();
		}
		// 
	//


	//dino classes methods
	void dino_part::draw(sf::RenderWindow* window) const {
		game_object::draw(window);
		sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(10 * scale, 10 * scale));
		rect->setPosition(sf::Vector2f(110 * scale + position.x, 20 * scale + position.y));
		rect->setFillColor(sf::Color(second_color.x, second_color.y, second_color.z));
		window->draw(*rect);
		delete(rect);
	}
	void dino::set_size() { size = dino_parts[0]->get_size(); }
	dino::dino(float _scale, float _outline_size, float _jump_speed, sf::Vector3f _first_color, sf::Vector3f _second_color):
	transformable(_scale)
	{
		stage = 1;
		jump_flag = false;
		first_color = _first_color;
		second_color = _second_color;
		jump_speed = _jump_speed;
		for (int i = 0; i < 3; i++)
			dino_parts[i] = new dino_part(dino_drawings[i], dino_drawings_sizes[i], scale, _outline_size, first_color, second_color);
		set_size();
	}
	void dino::set_stage(int stage_1_to_3) { stage = stage_1_to_3; }
	void dino::set_position(sf::Vector2f position) {
		this->position = position;
		for (int i = 0; i < 3; i++)
			dino_parts[i]->set_position(position);
	}
	void dino::set_start_position() { start_position = position; }
	void dino::set_jump_speed(float _jump_speed) { jump_speed = _jump_speed; }
	void dino::set_first_color(sf::Vector3f color) {
		first_color = color;
		for (int i = 0; i < 3; i++)
			dino_parts[i]->set_first_color(color);
	}
	void dino::set_second_color(sf::Vector3f color) {
		second_color = color;
		for (int i = 0; i < 3; i++)
			this->dino_parts[i]->set_second_color(color);
	}
	sf::Vector2f dino::get_start_position() const { return start_position; }
	int dino::get_stage() const { return stage; }
	float dino::get_jump_speed() const { return jump_speed; }
	const dino_part& dino::get_dino_part(int in_which_stage) const {
		if (in_which_stage == 0)
			return *dino_parts[stage - 1];
		else
			return *dino_parts[in_which_stage - 1];
	}
	bool dino::jump() {
		if (!jump_flag) {
			move(sf::Vector2f(0, -jump_speed));
			if (position.y <= start_position.y - size.y*(2.5))
				jump_flag = true;
		}
		else if (jump_flag){
			move(sf::Vector2f(0, jump_speed));
			if (abs(start_position.y - position.y) < jump_speed) {
				set_position(start_position);
				jump_flag = false;
				return true;
			}
			else if (position.y == start_position.y) {
				jump_flag = false;
				return true;
			}
		}
		return false;
	}
	void dino::move(sf::Vector2f position) {
		this->position += position;
		for (int i = 0; i < 3; i++)
			dino_parts[i]->move(position);
	}
	void dino::scale_object(float scale) {
		this->scale = scale;
		for (int i = 0; i < 3; i++)
			dino_parts[i]->scale_object(scale);
	}
	void dino::animate() {
		if (stage == 3)
			stage = 2;
		else
			stage += 1;
	}
	void dino::draw(sf::RenderWindow* window) const {
		dino_parts[stage - 1]->draw(window);
	}
	void dino::draw_outline(sf::RenderWindow* window) const {
		dino_parts[stage - 1]->draw_outline(window);
	}
	//

	//menu point class methods
	void menu_point::set_size() { size = sf::Vector2f((60 + this->text->getLocalBounds().width) * this->scale, (25 + this->text->getCharacterSize() / 2) * this->scale); }
	void menu_point::draw_background(sf::RenderWindow* window, float y_slip, float increase_size_by, sf::Vector3f color) const {
		sf::RectangleShape* rect[3];
		rect[0] = new sf::RectangleShape(sf::Vector2f((50 + text->getLocalBounds().width + 2 * increase_size_by) * scale, (*(*(data_address + 3)) - *(*(data_address + 1)) + 2 * increase_size_by) * scale));
		rect[1] = new sf::RectangleShape(sf::Vector2f((60 + text->getLocalBounds().width + 2 * increase_size_by) * scale, (20 + text->getCharacterSize() / 2 + 2 * increase_size_by) * scale));
		rect[2] = new sf::RectangleShape(sf::Vector2f((50 + text->getLocalBounds().width + 2 * increase_size_by) * scale, (*(*(data_address + 3) + 2) - *(*(data_address + 1) + 2) + 2 * increase_size_by) * scale));
		rect[0]->setPosition(sf::Vector2f((*(*(data_address)) - increase_size_by) * scale + position.x, (*(*(data_address + 1) + 0) - y_slip - increase_size_by) * scale + position.y));
		rect[1]->setPosition(sf::Vector2f((*(*(data_address)+1) - increase_size_by) * scale + position.x, (*(*(data_address + 1) + 1) - y_slip - increase_size_by) * scale + position.y));
		rect[2]->setPosition(sf::Vector2f((*(*(data_address)+2) - increase_size_by) * scale + position.x, (25 + text->getCharacterSize() / 2 - y_slip - increase_size_by) * scale + position.y));
		for (int i = 0; i < 3; i++) {
			rect[i]->setFillColor(sf::Color(color.x, color.y, color.z));
			window->draw(*(rect[i]));
			delete(rect[i]);
		}
	}
	void menu_point::draw_text(sf::RenderWindow* window, float y_slip, sf::Vector3f color) const {
		float y = text->getCharacterSize();
		text->setPosition(position.x + 30 * scale, position.y - (y / 2 - 15 + y_slip) * scale);
		text->setFillColor(sf::Color(color.x, color.y, color.z));
		text->setScale(sf::Vector2f(scale, scale));
		window->draw(*(text));
	}
	menu_point::menu_point(const float** _data_address, const int _data_size, float _scale, float _outline_size, sf::Vector3f _first_color, sf::Vector3f _second_color, std::string _string, int _characters_size)
		:game_object(_data_address, _data_size, _scale, _outline_size, _first_color, _second_color) {
		text = new sf::Text;
		font = new sf::Font;
		font->loadFromFile("font\\upheavtt.ttf");
		text->setFont(*font);
		text->setString(_string);
		text->setCharacterSize(_characters_size);
		animation = false;
		is_pressed = false;
		set_size();
	}
	void menu_point::set_state(bool _is_pressed) { is_pressed = _is_pressed; }
	int menu_point::get_character_size() const { return text->getCharacterSize(); }
	std::string menu_point::get_string() const { return text->getString(); }
	bool menu_point::get_state() const { return is_pressed; }
	bool menu_point::collision_with_point(sf::Vector2i point_coordinates) const {
		float x_min = get_min(data_address[0], data_size), y_min = get_min(data_address[1], data_size), x_max = 60 + text->getLocalBounds().width, y_max = 25 + text->getCharacterSize() / 2;
		if ((point_coordinates.x >= x_min * get_scale() + get_position().x && point_coordinates.y >= y_min * get_scale() + get_position().y)
			&& (point_coordinates.x <= x_max * get_scale() + get_position().x && point_coordinates.y <= y_max * get_scale() + get_position().y)) {
			return true;
		}
		else
			return false;
	}
	void menu_point::change_animation_flag(bool new_state) { animation = new_state; }
	void menu_point::draw(sf::RenderWindow* window) const {
		if (animation)
			animate(window, 2);
		else
			draw_without_animation(window);
	}
	void menu_point::draw_without_animation(sf::RenderWindow* window) const {
		draw_background(window, 0, 0, first_color);
		draw_text(window, 0, second_color);
	}
	void menu_point::draw_outline(sf::RenderWindow* window) const {
		draw_background(window, 0, outline_size, second_color);
	}
	void menu_point::animate(sf::RenderWindow* window, float step) const {
		draw_background(window, 0, 0, sf::Vector3f(second_color.x / 2, second_color.y / 2, second_color.z / 2));
		draw_background(window, step * 2, 0, sf::Vector3f(second_color.x / 4, second_color.y / 4, second_color.z / 4));
		draw_background(window, step*3, 0, sf::Vector3f(second_color.x * (1 + 8) / 8, second_color.y * (1 + 8) / 8, second_color.z * (1 + 8) / 8));
		draw_background(window, step*4,0, second_color);
		draw_background(window, step*4, 0, first_color);
		draw_text(window, step*4, second_color);
	}
	//

	//sand object methods
	int sand::compute_amount() { return (size.x * 3) / (scale * 100); }
	float sand::generate_horizontal_grains_position(int i) const { return size.x * (i + 1) / amount_of_grains + rand_float_number(0.0f, size.x / amount_of_grains - size_of_grains); }
	float sand::generate_vertical_grains_position() const { return position.y + rand_float_number(0, abs(position.x - position.y - size_of_grains)); }
	void sand::check_position() {
		if (grains[following][0]<-size_of_grains) {
				grains[following][0] = size.x+ rand_float_number(0, size.x / amount_of_grains - size_of_grains);
				grains[following][1] = generate_vertical_grains_position();
				if (following != amount_of_grains-1)
					following++;
				else
					following = 0;
		}
	}
	void sand::generate_grains_position() {
		for (int i = 0; i < amount_of_grains; i++) {
			grains[i][0] = generate_horizontal_grains_position(i);
			grains[i][1] = generate_vertical_grains_position();
		}
	}
	sand::sand(sf::Vector2f window_size, float _size, float _scale, float _speed, sf::Vector2f max_min_positions, sf::Vector3f color) :
	object(_scale)
	{
		size = window_size;
		amount_of_grains = compute_amount();
		size_of_grains = _size;
		speed = _speed;
		position = max_min_positions;
		first_color = color;
		grains = new float*[amount_of_grains];
		for (int i = 0; i < amount_of_grains; i++)
			grains[i] = new float[2];
		generate_grains_position();
		following = 0;
	}
	void sand::update_speed(float _speed) { speed = _speed; }
	void sand::move() {
		for (int i = 0; i < amount_of_grains; i++)
			grains[i][0] -= speed;
		check_position();
	}
	void sand::draw(sf::RenderWindow* window) const {
		sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(size_of_grains, size_of_grains));
		rect->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
		for (int i = 0; i < amount_of_grains; i++) {
			rect->setPosition(grains[i][0], grains[i][1]);
			window->draw(*rect);
		}
		delete(rect);
	}
	//

	//counter methods
	void counter::set_size() { size = sf::Vector2f(current_scores->getLocalBounds().width + 10 + hi->getLocalBounds().width, current_scores->getLocalBounds().height); }
	void counter::update_size(float scale) { set_size(); }
	void counter::update_text_position() { hi->setPosition(current_scores->getLocalBounds().width + 10 + position.x, -hi->getLocalBounds().height + position.y); }
	counter::counter(float scale, unsigned int character_size, int _hi_scores, sf::Vector3f first_color, sf::Vector3f second_color) :
		transformable(scale),
		drawable(first_color, second_color){
		scores = 0;
		hi_scores = _hi_scores;
		scores_string = new std::string(int_to_string(scores));
		hi_scores_string = new std::string("HI: ");
		hi_scores_string->append(int_to_string(hi_scores));
		font = new sf::Font();
		font->loadFromFile("font\\upheavtt.ttf");
		current_scores = new sf::Text("", *font, character_size);
		current_scores->setString(*scores_string);
		current_scores->setScale(scale, scale);
		current_scores->setPosition(0, -current_scores->getLocalBounds().height);
		hi = new sf::Text("", *font, character_size);
		hi->setString(*hi_scores_string);
		hi->setScale(scale, scale);
		hi->setPosition(current_scores->getLocalBounds().width + 10, -hi->getLocalBounds().height);
		set_size();
	}
	void counter::set_position(sf::Vector2f _position) {
		position = _position;
		current_scores->setPosition(position.x, -current_scores->getLocalBounds().height + position.y);
		hi->setPosition(current_scores->getLocalBounds().width + 10 + position.x, -hi->getLocalBounds().height + position.y);
	}
	int counter::get_scores()const { return scores; }
	int counter::get_hi_scores()const { return hi_scores; }
	void counter::move(sf::Vector2f _position) {
		position += _position;
		current_scores->move(_position);
		hi->move(_position);
	}
	void counter::update_hi() {
		if (hi_scores < scores) {
			hi_scores = scores;
			delete(hi_scores_string);
			hi_scores_string = new std::string("HI: ");
			hi_scores_string->append(int_to_string(hi_scores));
			update_size(0);
		}
	}
	void counter::add_to_scores(unsigned int volume) { 
		scores += volume;
		delete(scores_string);
		scores_string = new std::string(int_to_string(scores));
		current_scores->setString(*scores_string);
		update_text_position();
		update_size(0);
	}
	void counter::draw(sf::RenderWindow* window)const {
		current_scores->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
		hi->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
		window->draw(*current_scores);
		window->draw(*hi);
	}
	//

	//pause methods
	pause::pause(float scale, unsigned int character_size, sf::Vector3f first_color, sf::Vector3f second_color) :
		object(scale),
		drawable(first_color, second_color) {
		//font
		font = new sf::Font;
		font->loadFromFile("font\\upheavtt.ttf");
		//texts
		_pause = new sf::Text("PAUSE", *font, character_size);
		unsigned int temp = character_size / 3;
		_continue = new sf::Text(" PRESS \"P\" TO CONTINUE", *font, temp);
	}
	void pause::set_position(sf::Vector2f _position) { 
		position = _position; 
		_pause->setPosition(_position.x - _pause->getLocalBounds().width / 2, _position.y - _pause->getCharacterSize());
		_continue->setPosition(_position.x - _continue->getLocalBounds().width / 2, _position.y + _pause->getCharacterSize() * (1/2 + 1 / 10));
	}
	void pause::draw(sf::RenderWindow* window)const {
		_pause->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
		_continue->setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
		window->draw(*_pause);
		window->draw(*_continue);
	}
	//

	//help methods
	help::help(float scale, unsigned int character_size, sf::Vector3f first_color, sf::Vector3f second_color) :
		object(scale),
		drawable(first_color, second_color) {
		//font
		font = new sf::Font;
		font->loadFromFile("font\\upheavtt.ttf");
		//texts
		text = new sf::Text[6];
		text[0].setString("<Up arrow> - jump");
		text[1].setString("<F1> - open/close help");
		text[2].setString("<P> - pause/resume game");
		text[3].setString("<M> - quit to main menu (without saving)");
		text[4].setString("<Escape> - quick exit (without saving)");
		text[5].setString("Your scores are saved only when you lose!");
		for (int i = 0; i < 6; i++) {
			text[i].setFont(*font);
			text[i].setCharacterSize(character_size);
		}
	}
	void help::set_position(sf::Vector2f _position){
		position = _position;
		text[0].setPosition(_position.x - text[0].getLocalBounds().width / 2, _position.y);
		for (int i = 1; i < 6; i++)
			text[i].setPosition(_position.x - text[i].getLocalBounds().width / 2, position.y + i * text[0].getCharacterSize()/2 + i * 20);
	}
	void help::draw(sf::RenderWindow* window) const {
		for (int i = 0; i < 6; i++) {
			text[i].setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
			window->draw(text[i]);
		}
	}
	//

	//quick exit methods
	void quick_exit::draw_background()const {}
	void quick_exit::draw()const{
		for (int i = 0; i < 2; i++) {
			text[i].setFillColor(sf::Color(first_color.x, first_color.y, first_color.z));
			where_this_window_will_be_drawn->draw(text[i]);
		}
		resume->set_first_color(first_color);
		resume->set_second_color(second_color);
		resume->draw(where_this_window_will_be_drawn);
		_exit->set_first_color(first_color);
		_exit->set_second_color(second_color);
		_exit->draw(where_this_window_will_be_drawn);
	}
	quick_exit::quick_exit(sf::RenderWindow* window, float scale, sf::Vector3f first_color, sf::Vector3f second_color, int text_character_size, int menu_points_character_size) :
		window(window, scale, first_color, second_color) {
		//font
		font = new sf::Font;
		font->loadFromFile("font\\upheavtt.ttf");
		//textes
		text = new sf::Text[2];
		text[0].setString("Are you sure want to quit?");
		text[1].setString("Unless you stay, your progress won't be saved!");
		for (int i = 0; i < 2; i++) {
			text[i].setFont(*font);
			text[i].setCharacterSize(text_character_size);
		}
		//menu_points
		resume = new menu_point(menu_point_drawing, menu_point_drawing_size, scale, 0, first_color, second_color, "resume", menu_points_character_size);
		_exit = new menu_point(menu_point_drawing, menu_point_drawing_size, scale, 0, first_color, second_color, "exit", menu_points_character_size);
	}
	void quick_exit::set_position(sf::Vector2f _position) {
		position = _position;
		text[0].setPosition(position.x - text[0].getLocalBounds().width/2, position.y - text[0].getCharacterSize() / 2);
		text[1].setPosition(position.x - text[1].getLocalBounds().width/2, position.y + 20);
		resume->set_position(sf::Vector2f(position.x - resume->get_size().x - 20, position.y + text[0].getCharacterSize() + 40));
		_exit->set_position(sf::Vector2f(position.x + 20, position.y + text[0].getCharacterSize() + 40));
	}
	menu_point& quick_exit::get_menu_point(int number)const {
		if (number == 1)
			return *resume;
		else if (number == 2)
			return *_exit;
	}
	void quick_exit::draw(sf::RenderWindow* window)const {
		draw();
	}
	//
}