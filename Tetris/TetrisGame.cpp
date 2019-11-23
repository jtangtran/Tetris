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

#include "Gameboard.h"
#include "TetrisGame.h"
#include "GridTetromino.h"
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <iostream>



	// constructor
	//   assign pointers,
	//   load font from file: fonts/RedOctober.ttf
	//   setup scoreText
	//   reset the game
TetrisGame::TetrisGame(sf::RenderWindow* pWindow, sf::Sprite* pBlockSprite, Point gameboardOffset, Point nextShapeOffset) {
	
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
	{
		assert(false && "Missing font: RedOctober.ttf");
	};
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(435, 325);

	this->pWindow = pWindow;
	this->pBlockSprite = pBlockSprite;
	this->gameboardOffset = gameboardOffset;
	this->nextShapeOffset = nextShapeOffset;

	GridTetromino random = GridTetromino();
	//random.setShape(Tetromino::getRandomShape());
	//random.setGridLoc(board.getSpawnLoc());
	reset();
	//board.setContent(Gameboard::MAX_X / 2, Gameboard::MAX_Y / 2, 1);
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
	{
		assert(false && "Missing font: RedOctober.ttf");
	};
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(435, 325);

	}


	// destructor, set pointers to null
TetrisGame::~TetrisGame() {
	pWindow = nullptr;
	pBlockSprite = nullptr;
	}

	// draw anything to do with the game,
	// includes board, currentShape, nextShape, score
void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	pWindow->draw(scoreText);
	}

	// Event and game loop processing
	// handles keypress events (up, left, right, down, space)
void TetrisGame::onKeyPressed(sf::Event event) {
	switch (event.key.code) {
	case (sf::Keyboard::Left): 
			attemptMove(currentShape, -1, 0);
			break;
	case (sf::Keyboard::Right):
			attemptMove(currentShape, 1, 0);
			break;
	case (sf::Keyboard::Down):
			attemptMove(currentShape, 0, 1);
			if (!attemptMove(currentShape, 0, 1)) {
				shapePlacedSinceLastGameLoop = true;
			}
			break;
	case(sf::Keyboard::Up): 
			attemptRotate(currentShape);
			break;
	case(sf::Keyboard::Space):
		drop(currentShape);
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
		break;
	}
	
}
/*Let’s fill in the rest of processGameLoop() and make it so the game knows what to do once a shape has been placed. 
Check shapePlacedSinceLastGameLoop.  If true, we want to attempt to spawn the next shape.  Do so. 
If spawnNewShape() fails, it means that the gameboard was full and that spawning a new shape resulted in an illegal placement –
in which case, simply call reset() to start the game over. 
If it  succeeds, then we need to:*/
	// called every game loop to handle ticks & tetromino placement (locking)
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick += secondsSinceLastLoop;
	if (secondsSinceLastTick > secsPerTick) {
		tick();
		secondsSinceLastTick -= secsPerTick;
	}
	if (shapePlacedSinceLastGameLoop) {
		bool success = spawnNextShape();
		pickNextShape();
		int completedRows = board.removeCompletedRows();
		score += completedRows;
		updateScoreDisplay();

		//sboard.
		if (!success) {
			reset();
		}
	}
	shapePlacedSinceLastGameLoop = false;
	}

	// A tick() forces the currentShape to move (if there were no tick,
	// the currentShape would float in position forever). This should
	// call attemptMove() on the currentShape.  If not successful, lock() 
	// the currentShape (it can move no further), and record the fact that a
	// shape was placed (using shapePlacedSinceLastGameLoop)
void TetrisGame::tick() {
	if (attemptMove(currentShape, 0, 1)) {
		
	}
	else {
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}

	// reset everything for a new game (use existing functions) 
	//  - setScore to 0
	//  - determineSecondsPerTick(),
	//  - clear the gameboard,
	//  - pick & spawn next shape
	//  - pick next shape again
void TetrisGame::reset() {
	score = 0;
	updateScoreDisplay(); 
	determineSecsPerTick();
  	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
	}

	// assign nextShape.setShape a new random shape  
void TetrisGame::pickNextShape() {
	nextShape.setShape(Tetromino::getRandomShape());
	}


	// copy the nextShape into the currentShape and set 
	//   its loc to be the gameboard's spawn loc.
	//	 - return true/false based on isPositionLegal()
	bool TetrisGame::spawnNextShape() { 
		currentShape.setShape(nextShape.getShape());
		currentShape.setGridLoc(board.getSpawnLoc());
		if (isPositionLegal(currentShape)) {
			return true;
		}
		return false; 
	}

	// test if a rotation is legal on the tetromino, 
	//   if so, rotate it.
	//  To do this:
	//	 1) create a (local) temporary copy of the tetromino
	//	 2) rotate it (shape.rotateCW())
	//	 3) test if temp rotatio was legal (isPositionLegal()), 
	//      if so - rotate the original tetromino.
	//	 4) return true/false to indicate successful movement
	bool TetrisGame::attemptRotate(GridTetromino& shape) { 
		GridTetromino temp = shape;
		temp.rotateCW();
		bool test = isPositionLegal(temp);
		if (test) {
			shape.rotateCW();
			return true;
		}
		return false;
	}


	// test if a move is legal on the tetromino, if so, move it.
	//  To do this:
	//	 1) create a (local) temporary copy of the current shape
	//	 2) move it (temp.move())
	//	 3) test if temp move was legal (isPositionLegal(),
	//      if so - move the original.
	//	 4) return true/false to indicate successful movement
	bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
		//return true;
		GridTetromino temp = shape;
		temp.move(x, y);
		if (isPositionLegal(temp)) {
			shape.move(x, y);
			return true;
		}
		//shapePlacedSinceLastGameLoop = true;
		return false;
	}

	// drops the tetromino vertically as far as it can 
	//   legally go.  Use attemptMove(). This can be done in 1 line.
	void TetrisGame::drop(GridTetromino& shape) {
		while (attemptMove(shape, 0, 1)) {}
	}

	// copy the contents of the tetromino's mapped block locs to the grid.
	//	 1) get current blockshape locs via tetromino.getBlockLocsMappedToGrid()
	//	 2) iterate on the mapped block locs and copy the contents (color) 
	//      of each to the grid (via gameboard.setGridContent()) 
	void TetrisGame::lock(const GridTetromino& shape) {
		//GridTetromino contents = shape.getBlockLocsMappedToGrid();
		std::vector<Point> locs = shape.getBlockLocsMappedToGrid();
		for (Point& pt : locs) {
			board.setContent(pt.getX(), pt.getY(), static_cast<int>(shape.getColor()));
		}
	}

	// Graphics methods ==============================================

	// draw a tetris block sprite on the canvas		
	// x,y are meant to be block offsets (not pixels), which means you
	// will need to multiply them by BLOCK_WIDTH & BLOCK_HEIGHT
	//  for help: www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
	//	 1) set the block color using pBlockSprite->setTextureRect()
	//   2) set the block loc using pBlockSprite->setPosition()   
	//	 3) draw the block using pWindow.draw()
	// (pointers to window and sprite were passed into the constructor)
	void TetrisGame::drawBlock(int x, int y, TetColor color, Point origin) {
		pBlockSprite->setTextureRect(sf::IntRect(static_cast<int>(color)*BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		pBlockSprite->setPosition((origin.getX() +(BLOCK_WIDTH*x)), (origin.getY()+ (BLOCK_HEIGHT*y)));
		pWindow->draw(*pBlockSprite);
	}

	// draw the gameboard blocks on the window
	//   iterate through each row & col, use drawBlock() to 
	//   draw a block if it it isn't empty.
	void TetrisGame::drawGameboard() {
		for (int x = 0; x < Gameboard::MAX_X; x++) {
			for (int y = 0; y < Gameboard::MAX_Y; y++) {
				if (board.getContent(x, y) != Gameboard::EMPTY_BLOCK) {
					drawBlock(x,y,static_cast<TetColor>(board.getContent(x,y)), gameboardOffset);
				}
			}
		}
	}

	//   draw a tetromino on the window
	//	 iterate through each mapped loc & drawBlock() for each.
	//   the origin determines a 'base point' from which to calculate block offsets
	//   If the Tetromino is on the gameboard: use gameboardOffset (otherwise you 
	//   can specify another point as the origin - for the nextShape)
	void TetrisGame::drawTetromino(GridTetromino tetromino, Point origin) {
		std::vector<Point> locs = tetromino.getBlockLocsMappedToGrid();
		for (auto &pt : locs) {
			drawBlock(pt.getX(), pt.getY(), tetromino.getColor(), origin);
		}
	}

	// update the score display
	// form a string "score: ##" to display the current score
	// user scoreText.setString() to display it.
	void TetrisGame::updateScoreDisplay() {
		//std::cout << "score: " << score;
		std::string scoreT = "score: ";
		scoreT += std::to_string(score);
		scoreText.setString(scoreT);

	}

	// State & gameplay/logic methods ================================

	// return true if shape is within borders (isShapeWithinBorders())
	//	 and does NOT intersect locked blocks (!doesShapeIntersectLockedBlocks())
	bool TetrisGame::isPositionLegal(const GridTetromino& shape) {
		if (isShapeWithinBorders(shape) && !doesShapeIntersectLockedBlocks(shape)) {
			return true;
		}
		return false;
	}

	// return true if the shape is within the left, right,
	//	 and lower border of the grid. (false otherwise)
	//   All of a shape's blocks must be on the gameboard to be within borders
	bool TetrisGame::isShapeWithinBorders(const GridTetromino& shape) { 
		//looping through 
		std::vector<Point> locs = shape.getBlockLocsMappedToGrid();
		//for (auto &points : locs) {
		//	std::cout << "[" << points.getX() << "," << points.getY() << "] ";
		//}

		for (Point &point : locs) {
			//checks for invalid left, right, s
			if (point.getX() >= board.MAX_X || point.getX() < 0 || point.getY() >= board.MAX_Y ) {
				//std::cout << "isWithinBorders:false\n";
				return false;
			}
		}
		//std::cout << "isWithinBorders:true\n";
		return true;
	}

	// return true if the shape passed in intersects with content on the gameboard.
	//   Use Gameboard's areLocsEmpty() for this, and pass it the shape's mapped locs.
	bool TetrisGame::doesShapeIntersectLockedBlocks(const GridTetromino& shape) { 
		return board.areLocsEmpty(shape.getBlockLocsMappedToGrid()) == false;
		
	}

	// set secsPerTick 
	//   - basic: use MAX_SECS_PER_TICK
	//   - advanced: base it on score (higher score results in lower secsPerTick)
	void TetrisGame::determineSecsPerTick() {
		secsPerTick = MAX_SECS_PER_TICK;
	}



