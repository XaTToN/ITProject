#include <DumpJump/Game/Game.h>
#include <DumpJump/Shared.h>
#include <DumpJump/Gamepad/GamepadLayout.h>

const float PlayerSpeed = 200.f;
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
const float v_origin = 650;
const float h_origin = 1210;

Game::Game(bool use_gamepad) : use_gp(use_gamepad), mWindow(sf::VideoMode(1280, 720), "DumpJump"), map(PATH_RESSOURCES"maps"OS_SEP"development.vmap")
{
	bg_texture.loadFromFile(PATH_RESSOURCES"sprites"OS_SEP"background.png");
	bg.setTexture(bg_texture);

	for (int frame = 1; frame <= 5; ++frame)
	{
		sf::Texture txt;
		txt.loadFromFile(PATH_RESSOURCES"characters"OS_SEP"superman"OS_SEP + std::to_string(frame) + ".png");
		sf::Time t = sf::seconds(1.f / 15.f);
		anim.push_back(std::make_tuple(txt, t));
	}

	a_itr = anim.begin();

	player.setTexture(std::get<0>(anim.at(0)));
	player.scale(3, 3);
	player.setPosition(sf::Vector2f(0, 200));
	is_jumping = false;
	remaining_jump_time = sf::Time::Zero;

	buffer.loadFromFile(PATH_RESSOURCES"sounds"OS_SEP"ambient.ogg");
	ambient.setBuffer(buffer);
	ambient.setLoop(true);
	ambient.play();

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

			if (use_gp)
				processGamepad();

			processEvents();
			update(TimePerFrame);
			physics(TimePerFrame);
			animator(TimePerFrame);
		}

		render();
	}
}

void Game::update(sf::Time delta_time)
{
	map.UpdateTilePositions(offset);
	map.SelectTiles(offset);

	sf::Vector2f movement(0.f, 0.f);

	if (moving_left && player.getPosition().x > 0)
		movement.x -= PlayerSpeed;
	if (moving_right && player.getPosition().x < 1200)
		movement.x += PlayerSpeed;

	player.move(movement * delta_time.asSeconds());
}

void Game::render()
{
	mWindow.clear();

	mWindow.draw(bg);

	for (std::vector<Tile>::iterator itr = map.selected_tiles.begin(); itr != map.selected_tiles.end(); ++itr)
	{
		mWindow.draw(itr->sprite);
	}


	mWindow.draw(player);
	mWindow.display();
}

void Game::processGamepad()
{
	for (unsigned int itr = 0; itr < GamepadButtons::BTN_COUNT; ++itr)
	{
		if (sf::Joystick::isButtonPressed(0, itr))
			handlePlayerInput(itr, true);
		else
			handlePlayerInput(itr, false);
	}

	if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 90)
	{
		handlePlayerInput(sf::Keyboard::Left, false);
		handlePlayerInput(sf::Keyboard::Right, true);
	}
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -90)
	{
		handlePlayerInput(sf::Keyboard::Left, true);
		handlePlayerInput(sf::Keyboard::Right, false);
	}
	else
	{
		handlePlayerInput(sf::Keyboard::Left, false);
		handlePlayerInput(sf::Keyboard::Right, false);
	}
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
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::handlePlayerInput(const int& code, bool pressed)
{
	switch (code)
	{
		case GamepadButtons::BTN_A:
		case GamepadButtons::BTN_B:
		case GamepadButtons::BTN_X:
		case GamepadButtons::BTN_Y:
		case sf::Keyboard::Space:
			if (pressed)
			{
				is_jumping = true; 
				remaining_jump_time = sf::milliseconds(300);
			}
			break;
		case sf::Keyboard::Right:
			if (pressed)
				moving_right = true;
			else
				moving_right = false;
			break;
		case sf::Keyboard::Left:
			if (pressed)
				moving_left = true;
			else
				moving_left = false;
			break;
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

	for (std::vector<Tile>::iterator itr = map.selected_tiles.begin(); itr != map.selected_tiles.end(); ++itr)
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

void Game::animator(sf::Time delta_time)
{
	anim_t_accumulator += delta_time;

	if (anim_t_accumulator > std::get<1>(*a_itr))
	{
		++a_itr;
		if (a_itr == anim.end())
			a_itr = anim.begin();

		player.setTexture(std::get<0>(*a_itr));
		anim_t_accumulator = sf::Time::Zero;
	}
}