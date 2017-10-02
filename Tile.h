#pragma once
#include <vector>
#include <Windows.h>
#include <FL/Fl.h>
#include <FL/Fl_Button.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include "Point.h"
#include <string.h>
#include "GameWindow.h"
#include "projectfunctions.h"

const static int BOMB = -1;
const static int EMPTY = 0;
const static int ONE = 1;
const static int TWO = 2;
const static int THREE = 3;
const static int FOUR = 4;
const static int FIVE = 5;
const static int SIX = 6;
const static int SEVEN = 7;
const static int EIGHT = 8;

const static int PIXELHEIGHT = 15;
const static int PIXELWIDTH = 15;


const static int BOMBPIC = 0;
const static int EMPTYUNCOVEREDPIC = 9;
const static int COVEREDPIC = 10;
const static int FLAGGEDPIC = 11;
const static int INCORRECTPIC = 12;
const static int LOSINGPIC = 13;
const static int QUESTIONPIC = 14;

const static int YDISPLACEMENT = 85;



const static int EASYWIDTH = 9;
const static int EASYHEIGHT = 9;
const static int MEDIUMHEIGHT = 16;
const static int MEDIUMWIDTH = 16;
const static int HARDHEIGHT = 16;
const static int HARDWIDTH = 30;

const static int EASYBOMBS = 10;
const static int MEDIUMBOMBS = 40;
const static int HARDBOMBS = 99;

static std::vector<std::string> pictures{ "images/mine.jpg", "images/one.jpg","images/two.jpg","images/three.jpg","images/four.jpg","images/five.jpg",
											"images/siz.jpg", "images/seven.jpg", "images/eight.jpg", "images/emptyUncoveredTile.jpg", "images/coveredTile.jpg", 
											"images/flaggedMine.jpg", "images/incorrectMine.jpg", "images/losingMine.jpg", "images/question.jpg" };


class Tile : public Fl_Button {
private:
	int x;//x coordinate
	int y;//y coordinate
	int tileType;//this indicates what type of tile it is, if greater than 0 the number indicates how many bombs are adjacent the tile
	std::vector<Fl_JPEG_Image*> images;
	bool covered;//whether the tile is covered or not (might change to clicked at later time)
	bool flagged;//whether the tile has a user placed flag on it
	bool question;
	Point loc;//location of tile in a point object
	int boardWidth;
	int boardHeight;
public:
	int getX();//returns x coordinate
	int getY();//returns y coordinate
	int getTileType();//returns tile type of tile
	Point getPoint();//returns coordinates as a point
	void generateTileType(std::vector<Tile*> surrounding);//if tile is not a bomb, this populates the warning number of tile
	void incTileType();//increments the warning flag due to bomb being adjacent to tile
	int handle(int event);//event handler for each tile
	void populateImageVector();
	void clearAdjacentEmptyTiles();
	bool isCovered();
	bool isFlagged();
	bool isQuestion();


	std::vector<Fl_JPEG_Image*> getImageVector();
	Tile();//default constructor
	Tile(int x, int y);//constructs Tile object with given coordinates x and y, all other fields default
	Tile(int x, int y, int tileType);//constructs Tile object with given coordinates and tile type
	Tile(Point loc);//constructs Tile object with given coordinates within point object
	Tile(Point loc, int tileType);//creates Tile object with given coordinates and tile type
	friend std::ostream& operator<<(std::ostream& os, Tile* t);
};