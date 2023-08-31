#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include "block.h"

class minesweeperBlock : public block
{
public:
private:
public:
minesweeperBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, block::style myStyle = block::style::boarder);
void resize(block::location newLocation, RECT screenSize);
virtual bool render();
private:
};