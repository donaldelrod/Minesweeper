#include <iostream>
#include "GameWindow.h"

using namespace std;


int GameWindow::BB = EASYBOMBS;
int GameWindow::BW = EASYWIDTH;
int GameWindow::BH = EASYHEIGHT;

GameWindow::GameWindow() : 
	Fl_Window(BW*PIXELWIDTH, BH*PIXELHEIGHT + YDISPLACEMENT), boardWidth(BW), boardHeight(BH), numBombs(BB) {
	numFlags = 0;
	totalWidth = boardWidth*PIXELWIDTH;
	totalHeight = boardHeight*PIXELHEIGHT + YDISPLACEMENT;
	menu = new Fl_Menu_Bar(0, 0, totalWidth, 25);
	timerButton = new Fl_Button(0, 30, totalWidth / 4, 50);
	statusButton = new Fl_Button((totalWidth / 4) * 3, 30, totalWidth / 4, 50, "not started");
	isplaying = false;
	cleared = true;
	debugOn = false;
}

void GameWindow::decFlag() {
	this->numFlags--;
}

void GameWindow::incFlag() {
	this->numFlags++;
}

void GameWindow::clearBoard() {
	board.clear();
}
int GameWindow::getNumBombs() {
	return this->numBombs;
}
int GameWindow::getNumFlags() {
	return this->numFlags;
}
int GameWindow::getBoardHeight() {
	return boardHeight;
}
int GameWindow::getBoardWidth() {
	return boardWidth;
}
int GameWindow::getTotalWidth() {
	return totalWidth;
}
int GameWindow::getTotalHeight() {
	return totalHeight;
}
vector< vector < Tile* > >& GameWindow::getB() {
	return board;
}

void GameWindow::startPlaying() {
	isplaying = true;
}

//stops the game
void GameWindow::stopPlaying() {
	isplaying = false;
	this->statusButton->label("game over");
	Fl::remove_timeout(projectfunctions::timerCallback, this);
	cleared = false;
	for (int j = 0; j < boardHeight; j++) {
		for (int i = 0; i < boardWidth; i++) {
			Tile* t = board.at(j).at(i);
			t->deactivate();
		}
	}
}

void GameWindow::incCount() {
	count++;
}

void GameWindow::resetCount() {
	count = 0;
}

int GameWindow::getCount() {
	return count;
}

bool GameWindow::isPlaying() {
	return this->isplaying;
}

bool GameWindow::isCleared() {
	return this->cleared;
}

void GameWindow::setCleared() {
	cleared = true;
}

void GameWindow::setUncleared() {
	cleared = false;
}

void GameWindow::startTimer() {
	timerOn = true;
}
void GameWindow::stopTimer() {
	timerOn = false;
}
bool GameWindow::timerStarted() {
	return timerOn;
}

int GameWindow::handle(int event) {
	//debugging mode
	if (Fl::event_shift() != 0 && event  == FL_KEYUP){
		cout << debugOn << endl;
		debugOn = !debugOn;
		for (int j = 0; j < boardHeight; j++) {
			for (int i = 0; i < boardWidth; i++) {
				Tile* t = this->board.at(j).at(i);
				if (t->getTileType() == -1 && !debugOn && t->isFlagged() == false) {
					t->clear();
					t->image(t->getImageVector().at(BOMBPIC));
					t->redraw();
				}
				else if (t->getTileType() == -1 && debugOn && t->isFlagged() == false) {
					t->clear();
					t->image(t->getImageVector().at(COVEREDPIC));
					t->redraw();
				}
			}
		}
		return 1;
	}
	else return (Fl_Window::handle(event));
}

int GameWindow::getBB() {
	return BB;
}

int GameWindow::getBH() {
	return BH;
}

int GameWindow::getBW() {
	return BW;
}

void GameWindow::setBB(int newBB) {
	BB = newBB;
}

void GameWindow::setBH(int newBH) {
	BH = newBH;
}

void GameWindow::setBW(int newBW) {
	BW = newBW;
}