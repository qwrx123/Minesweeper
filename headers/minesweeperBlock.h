#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include "clickableTextBox.h"

class minesweeperBlock : public clickableTextBox
{
public:
private:
int numBombs;
bool bomb;
bool opened;
bool& whenDied;
wchar_t bombText[2];
bool flag;
public:
minesweeperBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t myText[], bool& clickedBomb, block::style myStyle = block::boarder);
void resize(block::location newLocation, RECT screenSize);
virtual bool render();
virtual void onMove(D2D1_POINT_2F moved);
virtual void onClick(D2D1_POINT_2F clicked);
void addBomb();
void setBomb();
bool checkBomb();
void open();
void toggleFlag();
bool getFlag();
bool checkOpen();
int getNumBombs();
private:
};