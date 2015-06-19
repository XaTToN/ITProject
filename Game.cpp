#include "Game.h"
#include "Shared.h"

const float PlayerSpeed = 200.f;
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
const float v_origin = 600;
const float h_origin = 1210;

Game::Game() : mWindow(sf::VideoMode(1280, 720), "IT Projektarbeit"), map(PATH_RESSOURCES"maps"OS_SEP"development.vmap")
{
	bg_texture.loadFromFile(PATH_RESSOURCES"sprites"OS_SEP"background.png");
	bg.setTexture(bg_texture);

	player_texture.loadFromFile(PATH_RESSOURCES"characters"OS_SEP"superman.png");
	player.setTexture(player_texture);
	player.scale(3, 3);
	player.setPosition(sf::Vector2f(0, 200));
	is_jumping = false;
	remaining_jump_time = sf::Time::Zero;
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
			physics(TimePerFrame);
		}

		render();
	}
}

void Game::update(sf::Time delta_time)
{
	map.UpdateTilePositions(offset);
}

void Game::render()
{
	mWindow.clear();

	mWindow.draw(bg);

	map.SelectTiles(offset);
	for (std::vector<Tile>::iterator itr = map.tiles.begin(); itr != map.tiles.end(); ++itr)
	{
		mWindow.draw(itr->sprite);
	}


	mWindow.draw(player);
	mWindow.display();
}

void Game::processEvents()
{
	sf::Event event;
	while(mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			//handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key code, bool pressed)
{
	switch (code)
	{
		case sf::Keyboard::Key::Space:
			if (pressed)
			{
				is_jumping = true; 
				remaining_jump_time = sf::milliseconds(300);
			}
	}
}

void Game::physics(sf::Time delta_time)
{
	offset += PlayerSpeed * delta_time.asSeconds() * 2;
	//handle player jumping
	if (is_jumping)
	{
		remaining_jump_time -= delta_time;
		sf::Vector2f movement(0.f, 0.f);
		movement.y -= PlayerSpeed;

		player.move(movement * delta_time.asSeconds());

		if (remaining_jump_time.asSeconds() <= 0)
			is_jumping = false;
	}

	if (!is_jumping && player.getPosition().y < v_origin)
	{
		sf::Vector2f movement(0.f, 0.f);
		movement.y += PlayerSpeed/1.5;

		if (player.getPosition().y + movement.y * delta_time.asSeconds() < v_origin)
			player.move(movement * delta_time.asSeconds());
		else
		{
			mWindow.close();
		}
	}

	sf::FloatRect bpl = player.getGlobalBounds();
	bpl.height /= 1.5;
	bpl.width /= 1.5;

	for (std::vector<Tile>::iterator itr = map.tiles.begin(); itr != map.tiles.end(); ++itr)
	{
		sf::FloatRect bti = itr->sprite.getGlobalBounds();
		bti.height /= 2;
		bti.width /= 2;
		if (bpl.intersects(bti))
		{
			mWindow.close();
		}
	}
}
