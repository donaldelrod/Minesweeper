#pragma once
#include <vector>
#include "Tile.h"
#include "Point.h"
#include "projectfunctions.h"
#include <Windows.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Progress.H>

class GameWindow : public Fl_Window {
private:
	std::vector < std::vector < Tile* > > board;
	int totalWidth;
	int totalHeight;
	int boardWidth;
	int boardHeight;
	int numBombs;
	int numFlags;
	bool isplaying;
	bool cleared;
	bool timerOn;
	bool debugOn;
	int count;

	static int BW;
	static int BH;
	static int BB;
public:
	Fl_Menu_Bar* menu;
	Fl_Progress* flagprogress;
	Fl_Button* timerButton;
	Fl_Button* statusButton;
	GameWindow();
	void clearBoard();
	int getNumBombs();
	int getNumFlags();
	int getBoardHeight();
	int getBoardWidth();
	int getTotalWidth();
	int getTotalHeight();
	void decFlag();
	void incFlag();
	std::vector< std::vector < Tile* > >& getB();
	void startPlaying();
	void stopPlaying();
	void resetCount();
	void incCount();
	int getCount();
	bool isPlaying();
	bool isCleared();
	void setCleared();
	void setUncleared();
	void startTimer();
	void stopTimer();
	bool timerStarted();
	int handle(int event);
	int getBB();
	int getBW();
	int getBH();
	static void setBB(int newBB);
	static void setBW(int newBW);
	static void setBH(int newBH);
};