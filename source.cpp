#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "Point.h"
#include "Tile.h"
#include "GameWindow.h"
#include <FL/Fl_Window.H>
#include "projectfunctions.h"



using namespace std;




int main(int argc, char ** argv) {
	bool game = true;//this is for the entire game
	
	while (game) {
		
		GameWindow* gw = new GameWindow();//GameWindow::BW, GameWindow::BH, GameWindow::BB);
		gw->clearBoard();
		gw->resetCount();
		srand(time(NULL));
		gw->begin();
		
		gw->menu->add("Game/NewGame", FL_CTRL + 'n', projectfunctions::menuCallback, gw);
		gw->menu->add("Game/Difficulty/Easy", FL_CTRL+'e', projectfunctions::menuCallback, gw);
		gw->menu->add("Game/Difficulty/Intermediate", FL_CTRL + 'i', projectfunctions::menuCallback, gw);
		gw->menu->add("Game/Difficulty/Expert", FL_CTRL + 'x', projectfunctions::menuCallback, gw);


		gw->timerButton->label("999");
		gw->timerButton->deactivate();

		gw->statusButton->deactivate();
		

		gw->flagprogress = new Fl_Progress(gw->getTotalWidth() / 4, 30, gw->getTotalWidth() / 2, 50);
		gw->flagprogress->minimum(0);
		gw->flagprogress->maximum(gw->getNumBombs());
		gw->flagprogress->color(0x8888888800);
		gw->flagprogress->selection_color(0x4444ff00);
		gw->flagprogress->labelcolor(FL_WHITE);
		gw->flagprogress->callback(projectfunctions::progressCallback, gw);

		vector<Point> bomblocs = projectfunctions::randomizeBombLocs(gw->getNumBombs(), gw->getBoardWidth(), gw->getBoardHeight());

		//for all of the code, iterating thru the board will be done [y, then x] so as to be able to acces elements linearly and not vertically
		for (int j = 0; j < gw->getBoardHeight(); j++) {//y
			for (int i = 0; i < gw->getBoardWidth(); i++) {//x

				bool bombplaced = false; //if a bomb was placed at this location
				Point currentPoint(i, j);//current point on the board
				for (Point bl : bomblocs) { //iterated thru bomblocs to compare them to currentPoint
					if (bl == currentPoint) {//if a bombloc is at the same location as currentPoint
						gw->getB().at(j).push_back(new Tile(bl, BOMB));//a new bomb tile is added to the board at location currentPoint 
						bombplaced = true;//flags that a tile (bomb) has already been added at this location
						break;//breaks out of loop comparing currentPoint to points in bombloc (because its already been found)
					}
				}
				if (!bombplaced) { //if a bomb was not placed
					gw->getB().push_back(vector< Tile* >(0));
					gw->getB().at(j).push_back(new Tile(currentPoint, EMPTY));//a new tile of tileType empty is added
				}
	
			}
			
		}
		//populates all empty tiles around the bombs with the number displayed(the warning number)
		projectfunctions::populateWarnings(gw->getB(), gw->getBoardWidth(), gw->getBoardHeight());
		

		//everything so far has been set-up of the board, now is where the game starts


		bool playing = true;//whether the player is playing the game still
		for (int j = 0; j < gw->getBoardHeight(); j++) {
			for (int i = 0; i < gw->getBoardWidth(); i++) {
				Tile* tempt = gw->getB().at(j).at(i);
				if (tempt->getTileType() == -1)
					tempt->image(tempt->getImageVector().at(COVEREDPIC));
				else 
					tempt->image(tempt->getImageVector().at(COVEREDPIC));
				tempt->callback(projectfunctions::tileCallback);
				
			}
		}

		gw->end();
		gw->startPlaying();
		gw->show(argc, argv);
		Fl::run();
	}
}