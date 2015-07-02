#ifndef __GAME_H__
#define __GAME_H__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <tuple>

#include <DumpJump/Map/MapMgr.h>

class Game
{
public:
	Game(bool use_gamepad);
	void run();
private:
	void update(sf::Time);
	void render();
	void processGamepad();
	void processEvents();
	void handlePlayerInput(const int&, bool);
	void physics(sf::Time);
	void animator(sf::Time);

private:
	bool use_gp;

	sf::RenderWindow mWindow;
	MapMgr map;

	sf::Sprite player;
	std::vector<std::tuple<sf::Texture, sf::Time>> anim;
	std::vector<std::tuple<sf::Texture, sf::Time>>::iterator a_itr;
	sf::Time anim_t_accumulator;

	float offset;

	bool moving_left;
	bool moving_right;
	bool is_jumping;
	sf::Time remaining_jump_time;

	sf::Texture bg_texture;
	sf::Sprite bg;

	sf::SoundBuffer buffer;
	sf::Sound ambient;
};

#endif //__GAME_H__