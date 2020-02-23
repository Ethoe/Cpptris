#define WIN32_LEAN_AND_MEAN
#include "Game.h"

const wchar_t* WINDOWCLASS = L"Tetris";
const wchar_t* WINDOWTITLE = L"Tetris LOL JK";

Game game;

LRESULT CALLBACK TheWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(game.GetWindow());
			return(0);
		}
		else if (wParam == VK_DOWN) //check for down arrow key
		{
			game.Move(0, 1);
			return(0);
		}
		else if (wParam == VK_UP) //check for clockwise
		{
			game.RotateBlock(FALSE);
			return(0);
		}
		else if (wParam == 0x5A) //check for counterclockwise
		{
			game.RotateBlock(TRUE);
			return(0);
		}
		else if (wParam == VK_SHIFT) //check for left arrow key
		{
			game.Hold();
			return(0);
		}
		else if (wParam == VK_LEFT) //check for left arrow key
		{
			game.Move(-1, 0);
			return(0);
		}
		else if (wParam == VK_RIGHT) //check for right arrow key
		{
			game.Move(1, 0);
			return(0);
		}
		else if (wParam == VK_SPACE) //check for harddrop
		{
			game.HardDrop();
			return(0);
		}
	}break;
	case WM_DESTROY://the window is being destroyed
	{
		PostQuitMessage(0);
		return(0);

	}break;
	case WM_PAINT://the window needs repainting
	{
		game.Paint();
		return(0);
	}break;
	}

	//pass along any other message to default message handler

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//assign instance to global variable
	game.SetInstance(hInstance);

	//create window class
	WNDCLASSEX wcx;

	//set the size of the structure
	wcx.cbSize = sizeof(WNDCLASSEX);

	//class style
	wcx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//window procedure
	wcx.lpfnWndProc = TheWindowProc;

	//class extra
	wcx.cbClsExtra = 0;

	//window extra
	wcx.cbWndExtra = 0;

	//application handle
	wcx.hInstance = game.GetInstance();

	//icon
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	//cursor
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);

	//background color
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//menu
	wcx.lpszMenuName = NULL;

	//class name
	wcx.lpszClassName = WINDOWCLASS;

	//small icon
	wcx.hIconSm = NULL;

	//register the window class, return 0 if not successful
	if (!RegisterClassEx(&wcx)) return(0);

	//create main window
	HWND hWndMain = CreateWindowEx(0, WINDOWCLASS, WINDOWTITLE, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, 0, 0, 320, 240, NULL, NULL, game.GetInstance(), NULL);

	//error check
	if (!hWndMain) return(0);

	game.SetWindow(hWndMain);

	//if program initialization failed, then return with 0
	if (!game.GameInit()) return(0);

	//message structure
	MSG msg;

	//message pump
	for ( ; ; )
	{
		//look for a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//check that we arent quitting
			if (msg.message == WM_QUIT) break;

			//translate message
			TranslateMessage(&msg);

			//dispatch message
			DispatchMessage(&msg);
		}
		//run main game loop
		game.GameLoop();
	}

	//clean up program data
	game.GameDone();

	//return the wparam from the WM_QUIT message
	return(msg.wParam);
}