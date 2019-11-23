
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "TetrisGame.h"
#include "TestSuite.h"


int main()
{
	// run some sanity tests on our classes to ensure they're working as expected.
	assert(TestSuite::runTestSuite());

	sf::Sprite blockSprite;			// the tetromino block sprite
	sf::Texture blockTexture;		// the tetromino block texture
	sf::Sprite backgroundSprite;	// the background sprite
	sf::Texture backgroundTexture;	// the background texture

	// load images
	backgroundTexture.loadFromFile("images/background.png");// load the background sprite
	backgroundSprite.setTexture(backgroundTexture);

	blockTexture.loadFromFile("images/tiles.png");			// load the tetris block sprite
	blockSprite.setTexture(blockTexture);

	srand((unsigned int)time(0));	// seed our randomizer

	// create the game window
	sf::RenderWindow window(sf::VideoMode(640, 800), "Tetris Game Window");

	// set up a tetris game
	TetrisGame game(&window, &blockSprite, Point(54, 125), Point(490, 210));


	sf::Clock clock;	// set up a clock so we can determine seconds per game loop

	// the main game loop
	while (window.isOpen())
	{
		// how long since the last loop (fraction of a second)
		float gameLoopSeconds = clock.getElapsedTime().asSeconds();
		clock.restart();

		// handle any window or keyboard events that have occured since the last game loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				game.onKeyPressed(event);	// handle key press
			}
		}

		game.processGameLoop(gameLoopSeconds);	// handle tetris game logic in here.


		window.clear(sf::Color::White);		// clear the entire window
		window.draw(backgroundSprite);	// draw the background (onto the window) 
		game.draw();					// draw the game (onto the window)
		window.display();				// re-display the entire window
	}
	return 0;
}
