// The GridTetromino extends the basic Tetromino
// Functionality added:
//  - The concept of the tetromino's location on the gameboard/grid. (gridLoc)
//  - The ability to change a tetromino's location
//  - The ability to retrieve a vector of tetromino block locations mapped to the gridLoc.
//
//  [expected .cpp size: ~ 40 lines]

#ifndef GRIDTETROMINO_H
#define GRIDTETROMINO_H

#include "Tetromino.h"

class GridTetromino : public Tetromino
{
public:
	GridTetromino();				// constructor, initialize gridLoc to 0,0

	Point getGridLoc() const;		// return the tetromino's grid/gameboard loc (x,y)
	void setGridLoc(int x, int y);	// sets the tetromino's grid/gameboard loc using x,y
	void setGridLoc(const Point& pt);		// sets the tetromino's grid/gameboard loc using a Point

	// transpose the gridLoc of this shape
	//	(1,0) represents a move to the right (x+1)
	//	(-1,0) represents a move to the left (x-1)
	//	(0,1) represents a move down (y+1)
	void move(int xOffset, int yOffset);

	// build and return a vector of Points to represent our inherited
	// blockLocs vector mapped to the gridLoc of this object instance.
	// eg: if we have a Point [x,y] in our vector,
	// and our gridLoc is [5,6] the mapped Point would be [5+x,6+y].
	std::vector<Point> getBlockLocsMappedToGrid() const;


	// MEMBER VARIABLES
private:
	Point gridLoc;	// the [x,y] location of this tetromino on the grid/gameboard. 
					// This loc changes each time the tetromino moves.


};

#endif /* GRIDTETROMINO_H */