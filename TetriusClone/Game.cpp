#include "Game.h"

Game::Game()
{
	window = NULL;
	instance = NULL;
	int i, j, k;
	int StdSRSt[8][5][2] = { { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} },
		{ {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} }, 
		{ {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} }, 
		{ {0, 0}, {1, 0}, {1, -1}, {0, -2}, {-1, -2} }, 
		{ {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} },
		{ {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} }, 
		{ {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} }, 
		{ {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} } };
	
	for (i = 0; i < 8; i++)
		for (j = 0; j < 5; j++)
			for (k = 0; k < 2; k++)
				StdSRS[i][j][k] = StdSRSt[i][j][k];

	int ISRSt[8][5][2] = { { {0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2} },
		{ {0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1} },
		{ {0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1} },
		{ {0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2} },
		{ {0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2} },
		{ {0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1} },
		{ {0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1} },
		{ {0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2} } };
	
	for (i = 0; i < 8; i++)
		for (j = 0; j < 5; j++)
			for (k = 0; k < 2; k++)
				ISRS[i][j][k] = ISRSt[i][j][k];

}

Game::~Game()
{

}

bool Game::GameInit()
{
	//set the client area size
	RECT rcTemp;
	SetRect(&rcTemp, 0, 0, MINOSIZE * HOLD + MAPWIDTH * MINOSIZE + MINOSIZE * GREY, MAPHEIGHT * MINOSIZE); //160x480 client area
	AdjustWindowRect(&rcTemp, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, FALSE); //adjust the window size based on desired client area
	SetWindowPos(window, NULL, 0, 0, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, SWP_NOMOVE); //set the window width and height

	//create map image
	HDC hdc = GetDC(window);
	bmoMap.Create(hdc, MAPWIDTH * MINOSIZE + MINOSIZE * GREY + MINOSIZE * HOLD, MAPHEIGHT * MINOSIZE);
	FillRect(bmoMap, &rcTemp, (HBRUSH)GetStockObject(BLACK_BRUSH));
	ReleaseDC(window, hdc);

	bmoPieces.Load(NULL, L"blocks.bmp");
	NewGame();
	return(true);//return success
}

void Game::GameDone()
{
	//clean up code goes here
}

void Game::GameLoop()
{
	int elapsed = GetTickCount64() - start_time;
	if (elapsed > 16) //60fps
	{
		if (timer == 30)
		{
			timer = 0;
			Move(0, 1);
		}
		timer++;
		start_time = GetTickCount64();
		DrawMap();
	}
}

void Game::NewGame()
{
	std::srand(GetTickCount64());
	start_time = GetTickCount64();
	timer = 0;
	GAMESTARTED = false;
	for (int x = 0; x < MAPWIDTH; x++)
	{
		for (int y = 0; y < TOTALHEIGHT; y++)
		{
			Map[x][y] = MINOBLACK;
		}
	}
	int i, temp[7];
	for (i = 0; i < 7; i++)
		temp[i] = i;

	std::random_shuffle(std::begin(temp), std::end(temp));
	BagCount = 7;
	for (i = 7; i < 14; i++)
		Bag[i] = temp[i - 7];

	NextPiece();
	DrawMap();
}

void Game::DrawTile(int x, int y, int tile)//put a tile
{
	//mask first
	BitBlt(bmoMap, x * MINOSIZE, y * MINOSIZE, MINOSIZE, MINOSIZE, bmoPieces, tile * MINOSIZE, MINOSIZE, SRCAND);
	//then image
	BitBlt(bmoMap, x * MINOSIZE, y * MINOSIZE, MINOSIZE, MINOSIZE, bmoPieces, tile * MINOSIZE, 0, SRCPAINT);
}

void Game::DrawMap()//draw screen
{
	int xmy, ymx, pp;

	//place the toolbar
	for (xmy = MAPWIDTH + HOLD; xmy < HOLD + MAPWIDTH + GREY; xmy++)
		for (ymx = 0; ymx < MAPHEIGHT; ymx++)
			DrawTile(xmy, ymx, MINOGREY);

	//draw preview pieces
	for (pp = 0; pp < 5; pp++)
		for (xmy = 0; xmy < 4; xmy++)
			for (ymx = 0; ymx < 4; ymx++)
				if (sPreviews[pp].size[xmy][ymx] != MINONODRAW)
					DrawTile(MAPWIDTH + xmy + HOLD + 1, 1 + ymx + pp * 3, sPreviews[pp].size[xmy][ymx]);

	//draw the map
	//loop through the positions
	for (xmy = 0; xmy < MAPWIDTH; xmy++)
		for (ymx = 0; ymx < MAPHEIGHT; ymx++)
			DrawTile(xmy + HOLD, ymx, Map[xmy][ymx + INVISROWS]);

	//place the toolbar
	for (xmy = 0; xmy < HOLD; xmy++)
		for (ymx = 0; ymx < MAPHEIGHT; ymx++)
			DrawTile(xmy, ymx, MINOGREY);


	//draw hold piece
	for (xmy = 0; xmy < 4; xmy++)
		for (ymx = 0; ymx < 4; ymx++)
			if (sHold.size[xmy][ymx] != MINONODRAW)
				DrawTile(1 + xmy, 1 + ymx, sHold.size[xmy][ymx]);

	Tetramino test = sCurrent;
	FindLowest(test);
	//draw shadow
	for (xmy = 0; xmy < 4; xmy++)
		for (ymx = 0; ymx < 4; ymx++)
			if (test.size[xmy][ymx] != MINONODRAW)
				DrawTile(test.x + xmy + HOLD, test.y + ymx - INVISROWS, test.size[xmy][ymx]);

	//draw moving block
	for (xmy = 0; xmy < 4; xmy++)
		for (ymx = 0; ymx < 4; ymx++)
			if (sCurrent.size[xmy][ymx] != MINONODRAW)
				DrawTile(sCurrent.x + xmy + HOLD, sCurrent.y + ymx - INVISROWS, sCurrent.size[xmy][ymx]);

	//invalidate the window rect
	InvalidateRect(window, NULL, FALSE);
}

void Game::Hold()
{
	if (held)
		return;
	if (sHold.hold == -1)
	{
		sHold = GenerateTetramino(sCurrent.piece);
		NextPiece();
	}
	else
	{
		Tetramino temp;
		temp = sHold;
		sHold = GenerateTetramino(sCurrent.piece);
		sCurrent = GenerateTetramino(temp.piece);
	}
	sCurrent.x = MAPWIDTH / 2 - 2;
	sCurrent.y = 1;
	sCurrent.state = 0;
	held = 1;
}

void Game::NewBag()
{
	int i;
	for (i = 0; i < 14; i++)
		Bag[i] = i % 7;

	std::random_shuffle(std::begin(Bag), std::end(Bag));
	BagCount = 0;
}

void Game::NextPiece()
{
	int newblock;
	int i, j;

	held = 0;

	//let's see if the game's started yet
	if (GAMESTARTED == false)
	{
		GAMESTARTED = true;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				sHold.size[i][j] = MINONODRAW;
		sHold.hold = -1;
		sCurrent = GenerateTetramino(Bag[BagCount]);
		BagCount++;
		for (i = 0; i < 5; i++)
		{
			sPreviews[i] = GenerateTetramino(Bag[BagCount]);
			BagCount++;
		}
	}
	else
	{
		sCurrent = sPreviews[0];
		sCurrent.state = 0;
		for (i = 0; i < 4; i++)
			sPreviews[i] = sPreviews[i + 1];

		if (BagCount >= 14)
			NewBag();

		sPreviews[4] = GenerateTetramino(Bag[BagCount]);
		BagCount++;
	}
	sCurrent.x = MAPWIDTH / 2 - 2;
	sCurrent.y = 1;
	if (CollisionTest(sCurrent, 0, 0))
		NewGame();
}

Tetramino Game::GenerateTetramino(int piece)
{
	Tetramino result;
	int i, j;

	//initialize the piece to all blank.
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			result.size[i][j] = MINONODRAW;

	switch (piece)
	{
	case 0: // I
	{
		result.size[0][1] = MINOCYAN;
		result.size[1][1] = MINOCYAN;
		result.size[2][1] = MINOCYAN;
		result.size[3][1] = MINOCYAN;
		result.piviot = 0;
	}break;
	case 1: // J
	{
		result.size[0][0] = MINOBLUE;
		result.size[0][1] = MINOBLUE;
		result.size[1][1] = MINOBLUE;
		result.size[2][1] = MINOBLUE;
		result.piviot = 1;
	}break;
	case 2: // L
	{
		result.size[0][1] = MINOORANGE;
		result.size[1][1] = MINOORANGE;
		result.size[2][1] = MINOORANGE;
		result.size[2][0] = MINOORANGE;
		result.piviot = 1;
	}break;
	case 3:// O
	{
		result.size[1][0] = MINOYELLOW;
		result.size[2][0] = MINOYELLOW;
		result.size[1][1] = MINOYELLOW;
		result.size[2][1] = MINOYELLOW;
		result.piviot = -1;
	}break;
	case 4:// S
	{
		result.size[1][0] = MINOGREEN;
		result.size[2][0] = MINOGREEN;
		result.size[0][1] = MINOGREEN;
		result.size[1][1] = MINOGREEN;
		result.piviot = 1;
	}break;
	case 5:// Z
	{
		result.size[0][0] = MINORED;
		result.size[1][0] = MINORED;
		result.size[1][1] = MINORED;
		result.size[2][1] = MINORED;
		result.piviot = 1;
	}break;
	case 6:// T
	{
		result.size[1][0] = MINOPURPLE;
		result.size[0][1] = MINOPURPLE;
		result.size[1][1] = MINOPURPLE;
		result.size[2][1] = MINOPURPLE;
		result.piviot = 1;
	}break;
	}
	result.piece = piece;
	result.state = 0;
	result.hold = 1;
	return(result);
}

void Game::HardDrop()
{
	Tetramino test = sCurrent;
	FindLowest(test);
	Lock(test.y - sCurrent.y);
}

void Game::RotateBlock(bool rotate)
{
	if (sCurrent.piviot == -1)
	{
		return;
	}
	int i, j, x, y, change, right;
	Tetramino temp = sCurrent;

	if (rotate)
		right = 1;
	else
		right = 0;

	if (rotate)
		change = -1;
	else
		change = 1;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			temp.size[i][j] = MINONODRAW;

	if (!rotate)
	{
		if (sCurrent.piviot != 0)
		{
			//copy &rotate the piece to the temporary array
			for (i = 0; i < 3; i++)
				for (j = 0; j < 3; j++)
					temp.size[2 - j][i] = sCurrent.size[i][j];
		}
		else
		{
			//copy &rotate the piece to the temporary array
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					temp.size[3 - j][i] = sCurrent.size[i][j];
		}
	}
	else
	{
		if (sCurrent.piviot != 0)
		{
			//copy &rotate the piece to the temporary array
			for (i = 0; i < 3; i++)
				for (j = 0; j < 3; j++)
					temp.size[j][2 - i] = sCurrent.size[i][j];
		}
		else
		{
			//copy &rotate the piece to the temporary array
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					temp.size[j][3 - i] = sCurrent.size[i][j];
		}
	}
	for (i = 0; i < 5; i++)
	{
		if (sCurrent.piviot != 0)
		{
			x = StdSRS[sCurrent.state * 2 + right][i][0];
			y = StdSRS[sCurrent.state * 2 + right][i][1];
		}
		else
		{
			x = ISRS[sCurrent.state * 2 + right][i][0];
			y = ISRS[sCurrent.state * 2 + right][i][1];
		}
		if (!CollisionTest(temp, x, -y))
		{
			sCurrent = temp;
			int dstate = (sCurrent.state + change) % 4;
			if (dstate < 0)
				dstate = 3;
			sCurrent.state = dstate;
			return;
		}
	}
}

void Game::Lock(int y)
{
	int i, j;
	//new block time! add this &#111;ne to the list!
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (sCurrent.size[i][j] != MINONODRAW)
				Map[sCurrent.x + i][sCurrent.y + j + y] = sCurrent.size[i][j];

	//check for cleared row!
	for (j = 0; j < TOTALHEIGHT; j++)
	{
		bool filled = true;
		for (i = 0; i < MAPWIDTH; i++)
			if (Map[i][j] == MINOBLACK)
				filled = false;

		if (filled)
			RemoveRow(j);
	}
	NextPiece();
}

void Game::Move(int x, int y)
{
	Tetramino test = sCurrent;
	if (CollisionTest(test, x, y))
	{
		if (y == 1)
			Lock(0);
	}
	else
	{
		sCurrent.x += x;
		sCurrent.y += y;
	}
}

void Game::FindLowest(Tetramino& test)
{
	int i;
	for (i = 0; i < TOTALHEIGHT; i++)
		if (CollisionTest(test, 0, 1))
			return;
}

int Game::CollisionTest(Tetramino& test, int nx, int ny)
{
	int newx = test.x + nx;
	int newy = test.y + ny;

	int i, j, x, y;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (test.size[i][j] != MINONODRAW)
			{
				if (newx + i < 0 || newx + i > MAPWIDTH - 1 ||
					newy + j < 0 || newy + j > TOTALHEIGHT - 1)
					return 1;
				if (Map[newx + i][newy + j] != MINOBLACK)
					return 1;
			}

	test.x += nx;
	test.y += ny;
	return 0;
}

void Game::RemoveRow(int row)
{
	int x, y;
	int counter = 0;

	for (x = 0; x < MAPWIDTH; x++)
		for (y = row; y > 0; y--)
			Map[x][y] = Map[x][y - 1];

}


HWND Game::GetWindow()
{
	return window;
}

HINSTANCE Game::GetInstance()
{
	return instance;
}

void Game::SetWindow(HWND _window)
{
	window = _window;
}

void Game::SetInstance(HINSTANCE _instance)
{
	instance = _instance;
}

BitMapObject Game::GetMap()
{
	return bmoMap;
}

void Game::Paint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(window, &ps);
	BitBlt(hdc, 0, 0, MINOSIZE * HOLD + MINOSIZE * MAPWIDTH + MINOSIZE * GREY, MINOSIZE * MAPHEIGHT, bmoMap, 0, 0, SRCCOPY);
	TextOut(hdc, 24, 0, L"Hold", 4);
	TextOut(hdc, MAPWIDTH * MINOSIZE + MINOSIZE * HOLD + 24, 0, L"Next", 4);
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", sCurrent.state);
	//TextOut(hdc, MAPWIDTH * MINOSIZE + MINOSIZE * HOLD + 24, 0, buffer, 1);
	EndPaint(window, &ps);
}