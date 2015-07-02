#include <SFML/Graphics.hpp>

#include <DumpJump/Game/Game.h>

int main()
{
	sf::Joystick::update();
	bool gamepad = false;
	if (sf::Joystick::isConnected(0))
		gamepad = true;

	Game game(gamepad);
	game.run();
}