#pragma once
#include "Tile.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <FL/Fl_Window.H>
#include "Point.h"

class Tile;

class projectfunctions {
public:
	static void printBoard(std::vector < std::vector < Tile* > >& board, int boardWidth, int boardHeight);
	static void getSurroundingEmptyTiles(std::vector < std::vector < Tile* > >& board, Point pos, int boardWidth, int boardHeight, std::vector < Tile* >& surroundingEmpty);
	static void populateWarnings(std::vector < std::vector < Tile* > >& board, int boardWidth, int boardHeight);
	static std::vector < Point > randomizeBombLocs(int numBombs, int boardWidth, int boardHeight);
	static void getSurroundingTiles(int x, int y, int boardWidth, int boardHeight, std::vector < std::vector < Tile* > >& board, std::vector < Tile* >& surrounding);
	static void getSurroundingTiles(Point pos, int boardWidth, int boardHeight, std::vector < std::vector < Tile* > >& board, std::vector < Tile* >& surrounding);
	static void tileCallback(Fl_Widget* flw, void* w);
	static void menuCallback(Fl_Widget* f, void* w);
	static void progressCallback(Fl_Widget* f, void* w);
	static void timerCallback(void* w);
};