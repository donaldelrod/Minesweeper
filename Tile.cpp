#include "Tile.h"
#include <iostream>
#include <string>
#include "projectfunctions.h"

using namespace std;

//returns x coordinate
int Tile::getX() {
	return x;
}

//returns y coordinate
int Tile::getY() {
	return y;
}

//returns tile type of tile
int Tile::getTileType() {
	return tileType;
}

//returns coordinates as a point
Point Tile::getPoint() {
	return loc;
}

//increments the warning flag due to bomb being adjacent to tile
void Tile::incTileType() {
	tileType++;
}

//event handler for each tile
int Tile::handle(int event) {

	GameWindow* p = static_cast<GameWindow*>(this->parent());


	//if the game hasnt started playing
	if (!p->isPlaying())
		return 1;


	//if the timer hasnt started and a tile is clicked
	if (!p->timerStarted() && event == FL_PUSH) {
		p->startTimer();
		p->statusButton->label("in progress");
		Fl::add_timeout(1, projectfunctions::timerCallback, p);
	}


	if (event == FL_PUSH) {
		
		if (p->isPlaying() == false && p->isCleared()) {
			p->resetCount();
		}

		int numb = p->getNumBombs();
		int numf = p->getNumFlags();


		if (Fl::event_button() == FL_RIGHT_MOUSE) {//if right click

			//if already uncovered
			if (this->covered == false) {}

			//if not flagged and there arent more flags on the board than there are bombs
			else if (this->flagged == false && numf<numb) {
				this->flagged = true;
				this->clear();
				this->image(images.at(FLAGGEDPIC));
				this->redraw();
				this->do_callback();
				p->incFlag();
				p->flagprogress->do_callback();
			}

			//if the tile is flagged
			else if (this->flagged && this->question == false){
				this->question = true;
				this->clear();
				this->image(images.at(QUESTIONPIC));
				p->decFlag();
				p->flagprogress->do_callback();
				this->redraw();
				this->do_callback();
			}

			//if tile is in quesion state
			else if (this->flagged && this->question) {
				this->flagged = false;
				this->question = false;
				this->clear();
				this->image(images.at(COVEREDPIC));
				this->redraw();
				this->do_callback();
			}
			
			
			
		}
		//end right click code
		
		
		//if the Tile is flagged or the Tile has already been uncovered
		else if (this->flagged || this->covered==false) {
		}//does nothing if flag is uncovered or flagged

		
		 //if the Tile is covered and its an empty tile
		else if (this->covered && this->tileType == 0) {
			this->clear();
			this->image(images.at(EMPTYUNCOVEREDPIC));
			this->clearAdjacentEmptyTiles();
			this->redraw();
			this->do_callback();
		}

		//if the Tile is covered and its a warning tile
		else if (this->covered && this->tileType > 0 && this->flagged == false) {
			this->covered = false;//uncovers the Tile
			this->clear();
			this->image(images.at(this->tileType));
			this->redraw();
			this->do_callback();
		}


		//if tile is uncovered and is a bomb and isnt flagged
		else if (this->covered && this->tileType == -1 && this->flagged == false) {
			for (int j = 0; j < p->getBoardHeight(); j++) {
				for (int i = 0; i < p->getBoardWidth(); i++) {
					Tile* t = p->getB().at(j).at(i);
					
					if (t->tileType == -1) {
						t->covered = false;
						t->flagged = false;
						t->clear();
						t->image(images.at(BOMBPIC));
						t->redraw();
						
					}
					else if (t->flagged && t->tileType != -1) {
						t->covered = false;
						t->flagged = false;
						t->clear();
						t->image(images.at(INCORRECTPIC));
						t->redraw();
					}
				}
			}
			this->covered = false;
			this->clear();
			this->image(images.at(LOSINGPIC));
			this->redraw();
			p->stopPlaying();
		}


		//this part checks to see if the user won, only happens when the number of placed flags equals number of bombs
		int numCorrectFlags = 0;
		numf = p->getNumFlags();
		if (numf == numb) {
			for (int j = 0; j < p->getBoardHeight(); j++) {
				for (int i = 0; i < p->getBoardWidth(); i++) {
					Tile* t = p->getB().at(j).at(i);
					if (t->isFlagged() && !t->isQuestion() && t->getTileType() == -1) {
						numCorrectFlags++;
					}
				}
			}

		}
		if (numCorrectFlags == numb) {
			p->stopPlaying();
			p->statusButton->label("You Win!!");
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		return 1;
	}
	else return Fl_Button::handle(event);
}

//if tile is not a bomb, this populates the warning number of tile
void Tile::generateTileType(vector<Tile*> surrounding) {
	for (Tile* around : surrounding) {//iterates thru all surrounding tiles
		if (around->getTileType() == BOMB) {//if a surrounding tile is a bomb
			tileType++;//it increments its tileType, which for non-bomb tiles starts at 0
		}
	}
}

//returns vector of tile images
vector<Fl_JPEG_Image*> Tile::getImageVector() {
	return images;
}


//returns if the question flag is true
bool Tile::isQuestion() {
	return question;
}

//cascades all(*or i guess most) empty adjacent tiles
void Tile::clearAdjacentEmptyTiles() {
	vector<Tile*> emptyAdjacent;
	GameWindow* gw = static_cast<GameWindow*>(this->parent());
	projectfunctions::getSurroundingEmptyTiles(gw->getB(), loc, gw->getBoardWidth(), gw->getBoardHeight(), emptyAdjacent);
	if (emptyAdjacent.size() < 3)
		return;
	this->clear();
	this->covered = false;
	this->image(images.at( (this->tileType == 0 ? EMPTYUNCOVEREDPIC : this->tileType)));
	this->redraw();
	for (Tile* t : emptyAdjacent) {
		if (t->covered) {
			t->clear();
			t->covered = false;
			t->image(images.at(EMPTYUNCOVEREDPIC));
			t->redraw();
			try {
				t->clearAdjacentEmptyTiles();
			}
			catch (out_of_range& e) {}
		}
	}
	
}

//fills image vector with all the possible tile images
void Tile::populateImageVector() {
	images.push_back(new Fl_JPEG_Image(pictures.at(BOMBPIC).c_str()));
	for (int i = 1; i < 9; i++)
		images.push_back(new Fl_JPEG_Image(pictures.at(i).c_str()));
	images.push_back(new Fl_JPEG_Image(pictures.at(EMPTYUNCOVEREDPIC).c_str()));
	images.push_back(new Fl_JPEG_Image(pictures.at(COVEREDPIC).c_str()));
	images.push_back(new Fl_JPEG_Image(pictures.at(FLAGGEDPIC).c_str()));
	images.push_back(new Fl_JPEG_Image(pictures.at(INCORRECTPIC).c_str()));
	images.push_back(new Fl_JPEG_Image(pictures.at(LOSINGPIC).c_str()));
	images.push_back(new Fl_JPEG_Image(pictures.at(QUESTIONPIC).c_str()));
	this->image(*images.at(BOMBPIC));
}

//default constructor
Tile::Tile() : Fl_Button(0, 0, 0, 0) {
	x = 0;
	y = 0;
	loc = Point(x, y);
	tileType = 0;
	covered = true;
	flagged = false;
	question = false;
	populateImageVector();
}

//constructs Tile object with given coordinates x and y, all other fields default
Tile::Tile(int x, int y) : x(x), y(y), Fl_Button(x*PIXELWIDTH, y*PIXELHEIGHT+YDISPLACEMENT, PIXELWIDTH, PIXELHEIGHT) {
	loc = Point(x, y);
	tileType = 0;
	covered = true;
	flagged = false;
	question = false;
	populateImageVector();
}

//constructs Tile object with given coordinates and tile type
Tile::Tile(int x, int y, int tileType) : x(x), y(y), tileType(tileType), Fl_Button(x*PIXELWIDTH, y*PIXELHEIGHT+YDISPLACEMENT, PIXELWIDTH, PIXELHEIGHT) {
	loc = Point(x, y);
	covered = true;
	flagged = false;
	question = false;
	populateImageVector();
}

//constructs Tile object with given coordinates within point object
Tile::Tile(Point loc) : loc(loc), Fl_Button(loc.getX()*PIXELWIDTH, loc.getY()*PIXELHEIGHT+YDISPLACEMENT, PIXELWIDTH, PIXELHEIGHT) {
	x = loc.getX();
	y = loc.getY();
	flagged = false;
	covered = true;
	question = false;
	tileType = 0;
	populateImageVector();
}

//creates Tile object with given coordinates and tile type
Tile::Tile(Point loc, int tileType) : Fl_Button(loc.getX()*PIXELWIDTH, loc.getY()*PIXELHEIGHT+YDISPLACEMENT, PIXELWIDTH, PIXELHEIGHT), loc(loc), tileType(tileType) {
	x = loc.getX();
	y = loc.getY();
	flagged = false;
	covered = true;
	question = false;
	populateImageVector();
}

//output override
ostream& operator<<(ostream& os, Tile* t) {
	if (t->getTileType() == -1) {
		os << -1 << " ";
	}
	else {
		os << " " << t->getTileType() << " ";
	}
	return os;
}


//returns if the tile is covered
bool Tile::isCovered() {
	return covered;
}


//returns if the tile is flagged
bool Tile::isFlagged() {
	return flagged;
}