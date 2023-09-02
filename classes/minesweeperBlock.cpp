#include "../headers/minesweeperBlock.h"
#include "../headers/helpers.h"

minesweeperBlock::minesweeperBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t myText[], bool& clickedBomb, block::style myStyle)
    :clickableTextBox(renderTarget, setLocation, screenSize, pDWriteFactory, myText, myStyle),
	numBombs(0), bomb(false), whenDied(clickedBomb), opened(false), flag(false)
{
	bombText[1] = L'\0';
	helper::intToText(bombText, 1, numBombs);
	textBox::changeText(bombText);	
}

void minesweeperBlock::resize(block::location newLocation, RECT screenSize)
{
    coordinates = newLocation;
    clickableTextBox::resize(screenSize);
}

bool minesweeperBlock::render()
{
	if (opened)
	{
		clickableTextBox::render();
	}
	else 
	{
		block::render();
	}
	return true;
}

void minesweeperBlock::addBomb()
{
	numBombs++;
	helper::intToText(bombText, 1, numBombs);
	textBox::changeText(bombText);	
}

void minesweeperBlock::setBomb()
{
	bomb = true;
}

bool minesweeperBlock::checkBomb()
{
	return bomb;
}

void minesweeperBlock::onMove(D2D1_POINT_2F moved)
{
    if (moved.x > drawnRect.left && moved.x < drawnRect.right &&
        moved.y > drawnRect.top && moved.y < drawnRect.bottom)
    {
		if (!opened)
		{
        	this->block::changeColor(block::colors::gray);
		}
    }
    else
    {
        if (flag)
		{
			this->block::changeColor(block::colors::red);
		}
		else if (opened)
		{
			this->block::changeColor(block::colors::lightGray);
		}
		else
		{
			this->block::changeColor(block::colors::white);
		}
    }
}

void minesweeperBlock::onClick(D2D1_POINT_2F clicked)
{

}

void minesweeperBlock::open()
{
	if (flag)
	{
		return;
	}
	if (bomb)
	{
		whenDied = true;
	}
	else
	{
		opened = true;
	}
}

int minesweeperBlock::getNumBombs()
{
	return numBombs;
}

bool minesweeperBlock::checkOpen()
{
	return opened;
}

void minesweeperBlock::toggleFlag()
{
	if (opened)
	{
		return;
	}
	
	flag = !flag;

	if (flag)
	{
		block::changeColor(block::colors::red);
	}
	else
	{
		block::changeColor(block::colors::white);
	}
}

bool minesweeperBlock::getFlag()
{
	return flag;
}