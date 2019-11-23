#include <string>
#include "Point.h"
#include <iostream>
#include <sstream>

	Point::Point() {
		x = 0;
		y = 0;
	}

	Point::Point(int newX, int newY) {
		x = newX;
		y = newY;
	}

	int Point::getX() const {
		return x;
	}

	int Point::getY() const{
		return y;
	}

	void Point::setX(int x) {
		this->x = x;
	}

	void Point::setY(int y) {
		this->y = y;
	}

	void Point::setXY(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void Point::swapXY() {
		int temp;
		temp = x;
		x = y;
		y = temp;
	}

	void Point::multiplyX(int factor) {
		x = x * factor;
	}

	void Point::multiplyY(int factor) {
		y = y * factor;
	}

	// return a string in the form "[x,y]" to represent
	// the state of the Point instance (for debugging)
	 std::string Point::toString() const {
		 std::stringstream s;
		 s << " [ " << x << ", " << y << "]";
		return s.str();
	}