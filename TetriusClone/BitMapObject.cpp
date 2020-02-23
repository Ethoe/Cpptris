#include "bitmapobject.h"

BitMapObject::BitMapObject()
{
	hdcMemory = NULL;
	nextBitMap = NULL;
	oldBitMap = NULL;
	iWidth = 0;
	iHeight = 0;
}

BitMapObject::~BitMapObject()
{
	if (hdcMemory)
		Destroy();
}

void BitMapObject::Load(HDC hdcCompatible, LPCTSTR lpszFilename)
{
	if (hdcMemory)
		Destroy();

	hdcMemory = CreateCompatibleDC(hdcCompatible);
	nextBitMap = (HBITMAP)LoadImage(NULL, lpszFilename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(hdcMemory, nextBitMap);
	
	BITMAP bmp;
	GetObject(nextBitMap, sizeof(BITMAP), (LPVOID)&bmp);
	iWidth = bmp.bmWidth;
	iHeight = bmp.bmHeight;
}

void BitMapObject::Create(HDC hdcCompatible, int width, int height)
{
	if (hdcMemory)
		Destroy();

	hdcMemory = CreateCompatibleDC(hdcCompatible);
	nextBitMap = CreateCompatibleBitmap(hdcCompatible, width, height);
	oldBitMap = (HBITMAP)SelectObject(hdcMemory, nextBitMap);
	iWidth = width;
	iHeight = height;
}

void BitMapObject::Destroy()
{
	SelectObject(hdcMemory, oldBitMap);
	DeleteObject(nextBitMap);
	DeleteDC(hdcMemory);
	hdcMemory = NULL;
	nextBitMap = NULL;
	oldBitMap = NULL;
	iWidth = 0;
	iHeight = 0;
}

BitMapObject::operator HDC()
{
	return(hdcMemory);
}

int BitMapObject::GetWidth()
{
	return(iWidth);
}

int BitMapObject::GetHeight()
{
	return(iHeight);
}
