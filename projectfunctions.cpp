#pragma once
#include "Tile.h"
#include "projectfunctions.h"

using namespace std;

void projectfunctions::printBoard(vector < vector < Tile* > >& board, int boardWidth, int boardHeight) {
	for (int j = 0; j < boardHeight; j++) {
		for (int i = 0; i < boardWidth; i++) {
			cout << board.at(j).at(i);
		}
		cout << endl;
	}
}

//adds empty surrounding tiles to point 'p' to vector suroundingEmpty, used for clearing adjacent empty tiles when an empty tile is clicked
void projectfunctions::getSurroundingEmptyTiles(vector < vector < Tile* > >& board, Point pos, int boardWidth, int boardHeight, vector<Tile*>& surroundingEmpty) {
	vector< Tile* > surrounding;
	getSurroundingTiles(pos, boardWidth, boardHeight, board, surrounding);//uses existing method getSurroundingTiles to get all tiles around point pos
	for (Tile* t : surrounding) {//iterates thru all surrounding tiles
		if (t->getTileType() == EMPTY)// || t->getTileType() == ONE)//if the surrounding tile 't' has a tiletype of EMPTY
			surroundingEmpty.push_back(t);//adds empty tile to vector to be returned
	}
}


//adds the warning flags around the bombs on the board
void projectfunctions::populateWarnings(vector < vector < Tile* > >& board, int boardWidth, int boardHeight) {
	for (int j = 0; j < boardHeight; j++) {//y
		for (int i = 0; i < boardWidth; i++) {//x
			if (board.at(j).at(i)->getTileType() != -1) {//if the tile at (i,j) is not a bomb
				vector<Tile*> around;
				getSurroundingTiles(board.at(j).at(i)->getPoint(), boardWidth, boardHeight, board, around);//gets all tiles surrounding the tile at (i,j)
				board.at(j).at(i)->generateTileType(around);//calls the tile at (i,j)'s generateTileType function, which increments the warning number for each adjacent bomb
			}
		}
	}
}


//generates random locations for the bombs given the number of bombs and the board dimensions
vector<Point> projectfunctions::randomizeBombLocs(int numBombs, int boardWidth, int boardHeight) {
	vector<Point> bomblocs;
	//this for loop generates a vector of random points for the bombs to be placed at
	for (int i = 0; i < numBombs; i++) {
		int bombx = rand() % boardWidth;//gets a random x val for bomb to be placed
		int bomby = rand() % boardHeight;//gets a random y val for bomb to be placed
		Point tempPoint(bombx, bomby); //possible point for a bomb
		if (bombx > boardWidth || bomby > boardHeight || bombx < 0 || bomby < 0 || (bombx == bomby && bombx == 0)) {
			i--;
			continue;
		}
		if (bomblocs.empty()) //if its the first time the loop has run the location is automatically added to the vector
			bomblocs.push_back(tempPoint);
		else {
			bool validNewLoc = true;
			for (Point p : bomblocs) {
				if (p == tempPoint) {//if a point in bombloc is the same as the random point
					i--;			//decreases 'i' so loop will run again
					validNewLoc = false; //sets value to false so duplicate loc wont be added to vector
				}
			}
			if (validNewLoc) //if the point tempPoint is a valid location for a bomb (there is no bomb already occupying that location)
				bomblocs.push_back(tempPoint);//adds random bomb location to vector bombloc
		}
	}
	return bomblocs;
}

//returns a vector of the tiles surrounding the point on the board given the x and y coordinates
void projectfunctions::getSurroundingTiles(int x, int y, int boardWidth, int boardHeight, vector < vector < Tile* > >& board, vector <Tile*>& surrounding) {
	getSurroundingTiles(Point(x, y), boardWidth, boardHeight, board, surrounding);
}

//returns a vector of the tiles surrounding the point on the board given the point location
void projectfunctions::getSurroundingTiles(Point pos, int boardWidth, int boardHeight, vector < vector < Tile* > >& board, vector<Tile*>& surrounding) {
	for (int j = -1; j < 2; j++) {//goes from -1 to 1 to get tile above, next to or under (in the y direction)
		for (int i = -1; i < 2; i++) {//goes from -1 to 1 to get tile before, over/under and next to (in the x direction)
			Point possiblePoint(pos.getX() + i, pos.getY() + j); //adds either -1, 0 or 1 to x and y vals to get 3x3 square around tile
			if (possiblePoint.isValid(boardWidth, boardHeight) && !(i == j && j == 0)) { // checks to see if point is in range of board, not checking at the point where the tile is at
				surrounding.push_back(board.at(possiblePoint.getY()).at(possiblePoint.getX())); //adds tile at position of point to vector to be returned
			}
		}
	}
}

//tile callback, actually not used at all in program
void projectfunctions::tileCallback(Fl_Widget* flw, void* w) {
	Tile* t = static_cast<Tile*>(flw);
	GameWindow* wind = static_cast<GameWindow*>(t->parent());
}

//callback for progress bar
void projectfunctions::progressCallback(Fl_Widget* f, void* w) {
	GameWindow* gw = static_cast<GameWindow*>(w);
	Fl_Progress* prog = static_cast<Fl_Progress*>(f);
	int nf = gw->getNumFlags();
	int nb = gw->getNumBombs();
	prog->value(nf);
	Fl::check();
}

//callback for timer
void projectfunctions::timerCallback(void* w) {
	GameWindow* gw = static_cast<GameWindow*>(w);
	gw->incCount();
	char buffer[6];
	int n = sprintf_s(buffer, "%d", gw->getCount());
	gw->timerButton->label(buffer);
	gw->timerButton->redraw();
	Fl::check();
	Fl::repeat_timeout(1, timerCallback, gw);
}

//callback for menu 
void projectfunctions::menuCallback(Fl_Widget* f, void* w) {
	Fl_Window* win = static_cast<Fl_Window*>(w);
	GameWindow* gw = static_cast<GameWindow*>(win);
	Fl_Menu_Bar* menu = static_cast<Fl_Menu_Bar*>(f);
	char item[80];
	menu->item_pathname(item, sizeof(item) - 1);
	if (strcmp(item, "Game/NewGame") == 0) {
		win->~Fl_Window();
	}
	else if (strcmp(item, "Game/Difficulty/Easy") == 0) {
		gw->setBB(EASYBOMBS);
		gw->setBH(EASYHEIGHT);
		gw->setBW(EASYWIDTH);
		win->~Fl_Window();
	}
	else if (strcmp(item, "Game/Difficulty/Intermediate") == 0) {
		cout << "int" << endl;
		gw->setBB(MEDIUMBOMBS);
		gw->setBH(MEDIUMHEIGHT);
		gw->setBW(MEDIUMWIDTH);
		win->~Fl_Window();
	}
	else if (strcmp(item, "Game/Difficulty/Expert") == 0) {
		gw->setBB(HARDBOMBS);
		gw->setBH(HARDHEIGHT);
		gw->setBW(HARDWIDTH);
		win->~Fl_Window();
	}
}