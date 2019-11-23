#include "Tetromino.h"
#include <vector>
#include <iostream>

Tetromino::Tetromino() {
	setShape(TetShape::SHAPE_T);

}
void Tetromino::setShape(TetShape shape) {
	// set the shape
	//  - clear any blockLocs set previously
	//  - set the blockLocs for the shape
	//  - set the color for the shape

	this->shape = shape;
	blockLocs.clear();
	switch (shape) {
	case TetShape::SHAPE_I:
		color = TetColor::BLUE_LIGHT;
		blockLocs = { Point(0,-1), Point(0,0), Point(0,1), Point(0,2) };
		break;
	case TetShape::SHAPE_J:
		color = TetColor::RED;
		blockLocs = { Point(0,0), Point(0,-1), Point(-1,-1), Point(0,1) };
		break;
	case TetShape::SHAPE_T:
		blockLocs = { Point(0,0), Point(1,0), Point(0,-1), Point(-1,0) };
		color = TetColor::PURPLE;
		break;
	case TetShape::SHAPE_L:
		blockLocs = { Point(0,0), Point(0,1), Point(0,-1), Point(1,-1) };
		color = TetColor::ORANGE;
		break;
	case TetShape::SHAPE_O:
		blockLocs = { Point(0,1), Point(1,1), Point(0,0), Point(1,0) };
		color = TetColor::YELLOW;
		break;
	case TetShape::SHAPE_S:
		blockLocs = { Point(0,0), Point(1,1), Point(0,1), Point(-1,1) };
		color = TetColor::BLUE_DARK;
		break;
	case TetShape::SHAPE_Z:
		blockLocs = { Point(0,0), Point(0,1), Point(1,0), Point(-1,1) };
		color = TetColor::GREEN;
		break;
	} // end of switch statement

}
// rotate the shape 90 degrees around [0,0] (clockwise)
// to do this:
//  - iterate through blockLocs
//  - rotate each Point 90 degrees around [0,0]
// hint: rotate the point [1,2] clockwise around [0,0] and note
// how the x,y values change. There are 2 functions in the Point
// class that can be used to accomplish a rotation.	
void Tetromino::rotateCW() {
	for (int i = 0; i < blockLocs.size(); i++) {
		blockLocs[i].swapXY();
		blockLocs[i].multiplyY(-1);
	}
}
TetColor Tetromino::getColor() const {
	return color;
}
TetShape Tetromino::getShape() const {
	return shape;
}
/*Have this function return a random TetShape.  
Hint: if you add an enumeration to the TetShape enums (in the last spot) called TetShapeCount –
you can then use it with the rand() function (using the mod operator) to get a random shape  
(You will have to cast TetShapeCount to an int, and then cast the result back to a TetShape before returning). 
The randomizer has already been seeded in main.cpp so you are already set up to make calls to rand().*/
TetShape Tetromino::getRandomShape() {
	int shape = rand() % static_cast<int>(TetShape::SHAPECOUNT);
	TetShape shapePrint = static_cast<TetShape>(shape);
	return shapePrint;
}
// print a grid to display the current shape
// to do this:
// print out a �grid� of text to represent a co-ordinate
// system.  Start at top left [-3,3] go to bottom right [3,-3]
// (use nested for loops)
//  for each [x,y] point, loop through the blockLocs and if
//  the point exists in the list, print an 'x' instead of a '.'
// You should end up with something like this:
// (results will vary depending on shape and rotation, eg: this
//  one shows a T shape rotated clockwise once)
//  .......
//  .......
//  ...x...
//  ..xx...
//  ...x...
//  .......
void Tetromino::printToConsole() const {
	for (int y = 3; y >= -3; --y) {
		for (int x = -3; x <= 3; ++x) {
			bool match{ false };
			for (int k = 0; k < blockLocs.size(); k++) {
				if (blockLocs[k].getX() == x && blockLocs[k].getY() == y) {
					match = true;
					break;
				}
			}
			if (match == true) {
				std::cout << "x ";
			}
			else {
				std::cout << ". ";
			}
		}
		std::cout << "\n";
	}
}
