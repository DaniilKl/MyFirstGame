#include <SFML/Window.hpp>
#include "windows.h"
#include "other.h"
int main()
{
	my::is_console_visible(false);
	sf::RenderWindow window(sf::VideoMode(), "Dino", sf::Style::Fullscreen);
	//window settings
	window.setMouseCursorVisible(true);
	window.setVerticalSyncEnabled(true);
	//
	my::menu_window* menu;
	my::play_window* playground;
	while (true) {
		menu = new my::menu_window(&window);
		if (!menu->execute_window())
			break;
		delete(menu);
		playground = new my::play_window(&window);
		if (!playground->execute_window())
			break;
		delete(playground);
	}
	window.close();
	return 0;
}