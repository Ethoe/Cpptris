#ifndef BITMAPOBJECT_H
#define BITMAPOBJECT_H

#pragma once
#include <Windows.h>

class BitMapObject
{
private:
	HDC hdcMemory;
	HBITMAP nextBitMap;
	HBITMAP oldBitMap;
	int iWidth;
	int iHeight;

public:
	BitMapObject();

	~BitMapObject();

	void Load(HDC hdcCompatible, LPCTSTR lpszFilename);

	void Create(HDC hdcCompatible, int width, int height);

	void Destroy();

	int GetWidth();

	int GetHeight();

	operator HDC();
};

#endif