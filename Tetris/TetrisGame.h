// This class encapsulates the tetris game and its drawing routines, gameplay, & control logic.
// This class was designed so with the idea of potentially instantiating 2 of them
// and have them run side by side (player vs player).
// So, anything you would need for an individual tetris game has been included here.
// Anything you might use between games (like the background, or the sprite used for 
// rendering a tetromino block) was left in main.cpp
// 
// This class is responsible for:
//   - setting up the board,
//	 - drawing game elements to the screen
//   - spawning tetrominoes,
//   - handling user input,
//   - moving and placing tetrominoes 
//
//  [expected .cpp size: ~ 275 lines]

#ifndef TETRISGAME_H
#define TETRISGAME_H

#include "Gameboard.h"
#include "GridTetromino.h"
#include <SFML/Graphics.hpp>


class TetrisGame
{
public:
	// STATIC CONSTANTS
	static const int BLOCK_WIDTH = 32;			// pixel width of a tetris block
	static const int BLOCK_HEIGHT = 32;			// pixel height of a tetris block

	// MEMBER FUNCTIONS

	// constructor
	//   assign pointers,
	//   load font from file: fonts/RedOctober.ttf
	//   setup scoreText
	//   reset the game
	TetrisGame(sf::RenderWindow *pWindow, sf::Sprite *pBlockSprite, Point gameboardOffset, Point nextShapeOffset);	 


	// destructor, set pointers to null
	~TetrisGame();								
				
	// draw anything to do with the game,
	// includes board, currentShape, nextShape, score
	void draw();								

	// Event and game loop processing
	// handles keypress events (up, left, right, down, space)
	void onKeyPressed(sf::Event event);

	// called every game loop to handle ticks & tetromino placement (locking)
	void processGameLoop(float secondsSinceLastLoop);

	// A tick() forces the currentShape to move (if there were no tick,
	// the currentShape would float in position forever). This should
	// call attemptMove() on the currentShape.  If not successful, lock() 
	// the currentShape (it can move no further), and record the fact that a
	// shape was placed (using shapePlacedSinceLastGameLoop)
	void tick();

private:
	// reset everything for a new game (use existing functions) 
	//  - setScore to 0
	//  - determineSecondsPerTick(),
	//  - clear the gameboard,
	//  - pick & spawn next shape
	//  - pick next shape again
	void reset();

	// assign nextShape.setShape a new random shape  
	void pickNextShape();

	
	// copy the nextShape into the currentShape and set 
	//   its loc to be the gameboard's spawn loc.
	//	 - return true/false based on isPositionLegal()
	bool spawnNextShape();																	



	// test if a rotation is legal on the tetromino, 
	//   if so, rotate it.
	//  To do this:
	//	 1) create a (local) temporary copy of the tetromino
	//	 2) rotate it (shape.rotateCW())
	//	 3) test if temp rotatio was legal (isPositionLegal()), 
	//      if so - rotate the original tetromino.
	//	 4) return true/false to indicate successful movement
	bool attemptRotate(GridTetromino &shape);

   
	// test if a move is legal on the tetromino, if so, move it.
	//  To do this:
	//	 1) create a (local) temporary copy of the current shape
	//	 2) move it (temp.move())
	//	 3) test if temp move was legal (isPositionLegal(),
	//      if so - move the original.
	//	 4) return true/false to indicate successful movement
	bool attemptMove(GridTetromino &shape, int x, int y);
												

	// drops the tetromino vertically as far as it can 
	//   legally go.  Use attemptMove(). This can be done in 1 line.
	void drop(GridTetromino &shape);

	// copy the contents of the tetromino's mapped block locs to the grid.
	//	 1) get current blockshape locs via tetromino.getBlockLocsMappedToGrid()
	//	 2) iterate on the mapped block locs and copy the contents (color) 
	//      of each to the grid (via gameboard.setGridContent()) 
	void lock(const GridTetromino &shape);
	
	// Graphics methods ==============================================
	
	// draw a tetris block sprite on the canvas		
	// x,y are meant to be block offsets (not pixels), which means you
	// will need to multiply them by BLOCK_WIDTH & BLOCK_HEIGHT
	//  for help: www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
	//	 1) set the block color using pBlockSprite->setTextureRect()
	//   2) set the block loc using pBlockSprite->setPosition()   
	//	 3) draw the block using pWindow.draw()
	// (pointers to window and sprite were passed into the constructor)
	void drawBlock(int x, int y, TetColor color, Point origin);
										
	// draw the gameboard blocks on the window
	//   iterate through each row & col, use drawBlock() to 
	//   draw a block if it it isn't empty.
	void drawGameboard();
	
	// draw a tetromino on the window
	//	 iterate through each mapped loc & drawBlock() for each.
	//   the origin determines a 'base point' from which to calculate block offsets
	//   If the Tetromino is on the gameboard: use gameboardOffset (otherwise you 
	//   can specify another point as the origin - for the nextShape)
	void drawTetromino(GridTetromino tetromino, Point origin);
	
	// update the score display
	// form a string "score: ##" to display the current score
	// user scoreText.setString() to display it.
	void updateScoreDisplay();

	// State & gameplay/logic methods ================================

	// return true if shape is within borders (isShapeWithinBorders())
	//	 and does NOT intersect locked blocks (!doesShapeIntersectLockedBlocks())
	bool isPositionLegal(const GridTetromino &shape);					
		
	// return true if the shape is within the left, right,
	//	 and lower border of the grid. (false otherwise)
	//   All of a shape's blocks must be on the gameboard to be within borders
	bool isShapeWithinBorders(const GridTetromino &shape);

	// return true if the shape passed in intersects with content on the gameboard.
	//   Use Gameboard's areLocsEmpty() for this, and pass it the shape's mapped locs.
	bool doesShapeIntersectLockedBlocks(const GridTetromino &shape);

	// set secsPerTick 
	//   - basic: use MAX_SECS_PER_TICK
	//   - advanced: base it on score (higher score results in lower secsPerTick)
	void determineSecsPerTick();

	// MEMBER VARIABLES

	// State members ---------------------------------------------
	int score = 0;				// the current game score.
    Gameboard board;			// the gameboard (grid) to represent where all the blocks are.
    GridTetromino nextShape;	// the tetromino shape that is "on deck".
    GridTetromino currentShape;	// the tetromino that is currently falling.

	// Graphics members ------------------------------------------
	Point gameboardOffset = {0,0};	// pixel XY offset of the gameboard on the screen
	Point nextShapeOffset = {0,0};	// pixel XY offset to the nextShape
	sf::Sprite *pBlockSprite;		// a pointer to the sprite used for all the blocks.
	sf::RenderWindow *pWindow;		// a pointer to the window that we are drawing on.

	sf::Font scoreFont;				// SFML font for displaying the score.
	sf::Text scoreText;				// SFML text object for displaying the score
	
								
	// Time members ----------------------------------------------
	// Note: a "tick" is the amount of time it takes a block to fall one line.

	const double MAX_SECS_PER_TICK = 0.75;		// start off with a slow (max) tick rate. (seconds per game tick)
	const double MIN_SECS_PER_TICK = 0.20;		// this is the fastest tick pace (seconds per game tick).
	double secsPerTick = MAX_SECS_PER_TICK;		// the number of seconds per tick (changes depending on score)
											
	double secondsSinceLastTick = 0.0;			// update this every game loop until it is >= secsPerTick,
												// we then know to trigger a tick.  Reduce this var (by a tick) & repeat.
	bool shapePlacedSinceLastGameLoop = false;	// Tracks whether we have placed (locked) a shape on
												// the gameboard in the current gameloop
};

#endif /* TETRISGAME_H */

