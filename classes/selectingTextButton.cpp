#include "../headers/selectingTextButton.h"

selectingTextButton::selectingTextButton(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
    IDWriteFactory* pDWriteFactory, const wchar_t myText[], int startingInt, int minInt, int maxInt, int* trackInt, block::style myStyle)
	:clickableTextBox(renderTarget, {setLocation.top, setLocation.left, setLocation.width, setLocation.height/3},
	screenSize, pDWriteFactory, myText, myStyle),
	currentNumber(renderTarget, {setLocation.top + setLocation.height/3, setLocation.left, setLocation.width, setLocation.height/3},
		screenSize, pDWriteFactory, L"0", myStyle),
	leftSelect(renderTarget, {setLocation.top + 2*(setLocation.height/3), setLocation.left, setLocation.width/2, setLocation.height/3},
		screenSize, pDWriteFactory, L"<-", myStyle),
	rightSelect(renderTarget, {setLocation.top + 2*(setLocation.height/3), setLocation.left + setLocation.width/2, setLocation.width/2, setLocation.height/3},
		screenSize, pDWriteFactory, L"->", myStyle),
	currentInt(startingInt), minInt(minInt), maxInt(maxInt), numDigets(helper::intToNumDigets(maxInt - minInt)), currentIntString(new wchar_t(numDigets+1)),
	outNumber(trackInt)
{
	if (currentInt < this->minInt)
	{
		currentInt = this->minInt;
	}
	if (currentInt > this->maxInt)
	{
		currentInt = this->maxInt;
	}
	helper::intToText(currentIntString, numDigets, currentInt);
	currentNumber.changeText(currentIntString);
}

selectingTextButton::~selectingTextButton()
{
	delete[] currentIntString;
}
bool selectingTextButton::render()
{
	clickableTextBox::render();
	currentNumber.render();
	leftSelect.render();
	rightSelect.render();
	return true;
}

bool selectingTextButton::resize(RECT newScreen)
{
	clickableTextBox::resize(newScreen);
	currentNumber.resize(newScreen);
	leftSelect.resize(newScreen);
	rightSelect.resize(newScreen);
	return true;
}

void selectingTextButton::onMove(D2D1_POINT_2F moved)
{
	leftSelect.onMove(moved);
	rightSelect.onMove(moved);
}

void selectingTextButton::onClick(D2D1_POINT_2F clicked)
{
	leftSelect.onClick(clicked);
	rightSelect.onClick(clicked);
	if (leftSelect.parseClick())
	{
		currentInt--;

	}
	if (rightSelect.parseClick())
	{
		currentInt = ++currentInt%(maxInt+1);
		if (currentInt < minInt)
		{
			currentInt = minInt;
		}
	}
	if (currentInt < minInt)
	{
		currentInt = maxInt;
	}
	helper::intToText(currentIntString, numDigets, currentInt);
	currentNumber.changeText(currentIntString);

	*outNumber = currentInt;
}