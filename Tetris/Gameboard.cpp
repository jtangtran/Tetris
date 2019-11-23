// The Gameboard class encapsulates the functionality of the Tetris game board.
// It is essentially a grid of blocks, and we will implement it with a 2D array.
//
// The Gameboard class has no concept of what a tetromino is!
// This is intentional abstraction and makes the gameboard’s implementation as 
// simple as possible. The gameboard class simply manages a 2D array of content
// (integers). Each integer of content can represent either an empty spot, or a
// color.  Gameboard member functions operate on that content (particularly on
// rows) to facilitate operations that happen on a Tetris gameboard (checking for
// a complete row, filling a row, copying a row, collapsing rows, etc…).
//
// Actual gameplay is handled by a separate class (which makes use of the gameboard).
// We will handle this in a future lab. To get a sense of the bigger picture and how 
// the gameboard is used : a GridTetromino (a special Tetromino that has knowledge of
// its location on a co-ordinate system) will eventually reach a location on the 
// gameboard where it cannot move down any further (because the points that it would
// occupy already contain content).  At this point, the information a Tetromino 
// contains about its blocks (location and color) will be copied to the gameboard grid.
// We use the gameboard to determine what points are already occupied as a result of 
// previous tetrominoes being “locked”, and what the contents are.It deals strictly in
// terms of[x][y] locations, and content(color, or EMPTY_BLOCK)
// 
// Displaying the board(via text output) should show empty spots, and areas where blocks
// of color(content) have been copied("locked") onto the board from tetrominos that have
// already been placed(either intentionally or not).
//
// - The game board is represented by a 2D array of integers(the grid).
// - The array contains content(integers) which represent either :
//    - an EMPTY_BLOCK(-1),
//    - a color from the Tetromino::TetColor enum.
// - This 2D array is oriented with[0][0] at the top left and[MAX_X][MAX_Y] at the bottom
//     right. Why ? It makes it easier to draw the grid on the screen later because this 
//     is the same way things are drawn on a screen co-ordinate system(where pixel 0, 0 is
//     considered top left).
//
// - *** IMPORTANT ***
//   Note that a 2D array normally has a[row][col] orientation (row-major ordering).  Since this
//     grid is used as a co-ordinate system, an[x][y] orientation is used where x (representing 
//     the column index) comes first, and y (representing the row index) comes second. This is 
//     the opposite of what you'd normally expect of a 2D array.  If you were to print out the 2D
//     array using a [row][column] orientation, you would find that it is horizontal instead
//     of vertical and you will likely exceed the bounds of the arrays!  If you simply think in
//     terms of an x, y co-ordinate system, you should be fine. Use 2 for loops to initialize
//     the array. Using an initializer list would be very confusing (because of the 
//     orientation change).
//
//  [expected .cpp size: ~ 150 lines]

#include <vector>
#include "Point.h"
#include "Gameboard.h"
#include <iomanip>
#include <iostream>

	// constructor - empty() the grid
Gameboard::Gameboard() { 
	//empties the grid initially
	empty();
}
	// return the content at a given point
int Gameboard::getContent(Point pt) const {
	return grid[pt.getX()][pt.getY()];
}
	// return the content at an x,y grid loc
int Gameboard::getContent(int x, int y) const {
	return grid[x][y];
}

	// set the content at a given point
void Gameboard::setContent(Point pt, int content) {
	grid[pt.getX()][pt.getY()] = content;
}
	// set the content at an x,y grid loc
void Gameboard::setContent(int x, int y, int content) {
	grid[x][y] = content;
}

	// set the content for an array of grid locs
void Gameboard::setContent(std::vector<Point> locs, int content) {
	for (int i = 0; i < locs.size(); i++) {
		grid[locs[i].getX()][locs[i].getY()] = content;
	}
}

	// return true if the content at ALL (valid) points is empty
	//   *** IMPORTANT NOTE: invalid x,y values can be passed to this method.
	//   Invalid meaning: outside the bounds of the grid.
	//   * ONLY TEST VALID POINTS (disregard the others - and ensure you
	//   don't use them to index into the grid).  Testing invalid points
	//   would likely result in an out of bounds error or segmentation fault!
	//   If no points are valid, return true
bool Gameboard::areLocsEmpty(std::vector<Point> locs) const {
	for (int i = 0; i < locs.size(); i++) {
		// if invalid, continue is like break but goes to the next part of the loop
		if ((locs[i].getX() >= MAX_X || locs[i].getX() < 0) || (locs[i].getY() >= MAX_Y || locs[i].getY() < 0)) {
			continue;
		}
		else { // valid
			//checks if it is not empty 
			if (grid[locs[i].getX()][locs[i].getY()] != EMPTY_BLOCK) {
				return false;
			}
		}
	}
	return true;
}

	// removes all completed rows from the board
	//   use getCompletedRowIndices() and removeRows() 
	//   return the # of completed rows removed
int Gameboard::removeCompletedRows() {
	//assigning a new vector with the completed row indices
	std::vector<int> completed = getCompletedRowIndices();
	//remove the rows
	removeRows(completed);
	//return the number of removed completed rows
	return completed.size();
}

	// fill the board with EMPTY_BLOCK 
	//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
	void Gameboard::empty() {
		//iterating through the rowIndex, y is the rowIndex
		for (int y = 0; y < MAX_Y; y++) {
				//fill the row with empty block  
				fillRow(y, EMPTY_BLOCK);
			}
	}

	// getter for the spawnLoc for new blocks 
Point Gameboard::getSpawnLoc() const {
	return spawnLoc;
}

	// print the grid contents to the console (for debugging purposes)
	//   use std::setw(2) to space the contents out (#include <iomanip>).
	void Gameboard::printToConsole() const {
		for (int y = 0; y < MAX_Y; y++) {
			for (int x = 0; x < MAX_X; x++) {
				std::cout << std::setw(2); //spaces out the grid contents
				std::cout << grid[x][y];
			}
			std::cout << std::endl;
		}
		
	}

	// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
	bool Gameboard::isRowCompleted(int rowIndex) const {
		for (int x = 0; x < MAX_X; x++) {
			//checking if the grid x y coordinate is empty or not
			if (grid[x][rowIndex] == EMPTY_BLOCK) {
				return false;
			}
		}
		return true;
	}

	//	scan the board for completed rows.
	//   Iterate through grid rows and use isRowCompleted(rowIndex)
	//   return a vector of completed row indices.

	std::vector<int> Gameboard::getCompletedRowIndices() const {
		std::vector<int> completed; //declare a vector
		//scanning the grid rows and checking if they are complete row indices
		for (int y = 0; y < MAX_Y; y++) {
			//checking if the row is completed
			if (isRowCompleted(y)) {
				//adding to the vector 
				completed.push_back(y); 
			}
		}
		return completed;
	}

	// In gameplay, when a full row is completed (filled with content)
	// it gets "removed".  To be exact, the row itself is not removed
	// but the content from the row above it is copied into it.
	// This continues all the way up the grid until the first row
	// is copied into the second row.  Finally, the first row is 
	// filled with EMPTY_BLOCK
	// given a row index:
	//   1) Starting at rowIndex, copy each row above the removed
	//     row "one-row-downwards" in the grid.
	//     (loop from y=rowIndex down to 0, and copyRowIntoRow(y-1, y)).
	//   2) call fillRow() on the first row (and place EMPTY_BLOCKs in it).

	void Gameboard::removeRow(int rowIndex) {
		for (int y = rowIndex; y >= 0; y--) {
			copyRowIntoRow(y - 1, y);
		}
		//calling fillRow on the first row, hence 0
		//special case for the first row because there is no y -1 
		fillRow(0, EMPTY_BLOCK);
	}

	// given a vector of row indices, remove them 
	//   (iterate through the vector and and call removeRow()
	//   on each row index). 
	void Gameboard::removeRows(std::vector<int> rowIndices) {
		//iterating through the vector 
		for(auto &y: rowIndices){
			// & so we don't have to copy 
			//removing the rows of the vector
			removeRow(y);
		}
		/*for (int y = 0; y < rowIndices.size(); y++) {
			//removing the rows of the vector
			removeRow(rowIndices[y]);
		}*/ //same thing 
	}

	// fill a given grid row with specified content
	void Gameboard::fillRow(int rowIndex, int content) {
		for (int x = 0; x < MAX_X; x++) {
			//filling the row with specified content
			grid[x][rowIndex] = content;	
		}
	}

	// copy a source row's contents into a target row.
	void Gameboard::copyRowIntoRow(int sourceRowIndex, int targetRowIndex) {
		for (int x = 0; x < MAX_X; x++) {
			grid[x][targetRowIndex] = grid[x][sourceRowIndex];
		}
	}

