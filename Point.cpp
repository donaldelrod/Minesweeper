#include "Point.h"

int Point::getX() {
	return x;
}

int Point::getY() {
	return y;
}

bool Point::isValid(int boardWidth, int boardHeight) {
	if (x < 0 || x >= boardWidth)
		return false;
	else if (y<0 || y>= boardHeight)
		return false;
	else return true;
}

void Point::setY(int newY) {
	y = newY;
}

void Point::setX(int newX) {
	x = newX;
}

Point::Point() {
	x = 0;
	y = 0;
}

Point::Point(int x, int y) : x(x), y(y) {}

bool Point::operator==(const Point other) {
	if (this->x == other.x && this->y == other.y)
		return true;
	else return false;
}