#ifndef POINT_H
#define POINT_H
#include <string>
class Point {
private:
	int x;
	int y;
public:
	Point();
	Point(int newX, int newY);
	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);
	void setXY(int x, int y);
	void swapXY();			// swap x and y
	void multiplyX(int factor);	// multiply x by some factor
	void multiplyY(int factor);	// multiply y by some factor


	std::string toString() const;	// return a string in the form "[x,y]" to represent
};								// the state of the Point instance (for debugging)

#endif