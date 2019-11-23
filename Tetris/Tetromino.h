#ifndef TETROMINO_H
#define TETROMINO_H

#include "Point.h"
#include <vector>

enum class TetColor {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE_LIGHT,
	BLUE_DARK,
	PURPLE
};
enum class TetShape {
	SHAPE_S,
	SHAPE_Z,
	SHAPE_L,
	SHAPE_J,
	SHAPE_O,
	SHAPE_I,
	SHAPE_T,
	SHAPECOUNT
};
class Tetromino {
private:
	TetColor color;
	TetShape shape;

protected:
	std::vector<Point> blockLocs;

public:
	Tetromino();
	void setShape(TetShape shape);
	void rotateCW();
	TetColor getColor() const;
	TetShape getShape() const;
	void printToConsole() const;
	friend class TestSuite;// (allows TestSuite access to private members for testing)
	static TetShape getRandomShape();

};
#endif