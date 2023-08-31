#include "../headers/minesweeperBlock.h"

minesweeperBlock::minesweeperBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, block::style myStyle)
    :block(renderTarget, setLocation, screenSize, myStyle)
{

}

void minesweeperBlock::resize(block::location newLocation, RECT screenSize)
{
    coordinates = newLocation;
    block::resize(screenSize);
}

bool minesweeperBlock::render()
{
	block::render();

	return true;
}
