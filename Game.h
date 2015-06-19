#ifndef __GAME_H__
#define __GAME_H__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "MapMgr.h"

class Game
{
public:
	Game();
	void run();
private:
	void update(sf::Time);
	void render();
	void processEvents();
	void handlePlayerInput(sf::Keyboard::Key, bool);
	void physics(sf::Time);
private:
	sf::RenderWindow mWindow;
	MapMgr map;

	sf::Texture player_texture;
	sf::Sprite player;
	float offset;

	bool is_jumping;
	sf::Time remaining_jump_time;

	sf::Texture bg_texture;
	sf::Sprite bg;
};

#endif //__GAME_H__
