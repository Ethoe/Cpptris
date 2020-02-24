#ifndef GAME_H
#define GAME_H

#pragma once
#include <algorithm>
#include <stdlib.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include "bitmapobject.h"


struct Tetramino {
	int size[4][4];
	int x;
	int y;
	int piviot;
	int state = 0;
	int piece;
	int hold;
};

const int MINOSIZE = 16;
const int MAPWIDTH = 10;
const int MAPHEIGHT = 20;
const int INVISROWS = 2;
const int GREY = 5;
const int HOLD = 5;
const int TOTALHEIGHT = MAPHEIGHT + INVISROWS;

const int MINONODRAW = -1;
const int MINOBLACK = 0;
const int MINOORANGE = 1;
const int MINOBLUE = 2;
const int MINORED = 3;
const int MINOGREEN = 4;
const int MINOYELLOW = 5;
const int MINOGREY = 6;
const int MINOCYAN = 7;
const int MINOPURPLE = 8;

class Game
{
private:
	HINSTANCE instance;
	HWND window;

	BitMapObject bmoMap;
	BitMapObject bmoPieces;

	int Map[MAPWIDTH][TOTALHEIGHT];
	int StdSRS[8][5][2];
	int ISRS[8][5][2];

	Tetramino sPreviews[5];
	Tetramino sCurrent;
	Tetramino sHold;

	DWORD start_time;
	DWORD total_time;
	int timer;
	int Bag[14];
	int BagCount;
	int held;
	bool GAMESTARTED;

public:
	Game();
	~Game();
	bool GameInit();
	void GameLoop();
	void GameDone();
	void DrawTile(int x, int y, int tile);
	void DrawMap();
	void NextPiece();
	void Hold();
	void NewBag();
	void HardDrop();
	void Lock(int y);
	void FindLowest(Tetramino& test);
	Tetramino GenerateTetramino(int piece);
	void RotateBlock(bool direction);
	void Move(int x, int y);
	int CollisionTest(Tetramino& test, int dx, int dy);
	void RemoveRow(int row);
	void NewGame();
	HWND GetWindow();
	HINSTANCE GetInstance();
	void SetWindow(HWND _window);
	void SetInstance(HINSTANCE _instance);
	BitMapObject GetMap();
	void Paint();
};

#endif