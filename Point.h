#pragma once
#include <vector>
class Point {
private:
	int x;
	int y;
public:
	int getX();
	int getY();
	bool isValid(int boardWidth, int boardHeight);
	void setX(int newX);
	void setY(int newY);
	Point();
	Point(int x, int y);
	bool operator==(const Point other);
};