#include "../headers/game.h"
#include "../headers/scene.h"

const int game::numMines[game::difficulty::numDifficulty] = {10, 40, 30, 99};
const int game::boardSizes[game::difficulty::numDifficulty][2] = {{10, 10},{16, 13},{16, 16},{30, 16}};
const block::location game::gameSquareBoundary = {0.21, 0.05, 0.9, 0.74};

game::game(ID2D1HwndRenderTarget* renderTarget, IDWriteFactory* pDWriteFactory, RECT screenSize, wchar_t playerName[20], game::difficulty currentDifficulty, songManager& effectGenerator)
	:count(0), blocksWidth(boardSizes[(int)currentDifficulty][0]), blocksHeight(boardSizes[(int)currentDifficulty][1]), dead(false), win(false), runningTime(0),
	flagsLabel(renderTarget, {0.05, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"Flags"), 
    flagsDisplay(renderTarget, {0.1, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"0"),
	timeLabel(renderTarget, {0.05, 0.8, 0.15, 0.05}, screenSize, pDWriteFactory, L"Time"), 
    timeDisplay(renderTarget, {0.1, 0.8, 0.15, 0.05}, screenSize, pDWriteFactory, L"0"),
	effectGenerator(std::move(effectGenerator)), firstClick(true), numBombs(numMines[(int)currentDifficulty]), numOpened(0)
{
	wcsncpy(currentMetrics.name, playerName, 20);
	for (int i = 0; i < blocksWidth; i++)
	{
		for(int j = 0; j < blocksHeight; j++)
		{
			renderScreenBlocks[i][j] = new minesweeperBlock(renderTarget, {0.1, 0.1, 0.1, 0.1}, screenSize, pDWriteFactory, L"", dead);
		}
	}
	for (int i = 0; i < 0xffff; i++)
	{
		keys[i] = false;
	}
	currentMetrics.flags = 0;
	helper::intToText(currentMetrics.flagsText, 9, currentMetrics.flags);
	currentMetrics.time = 0;
	helper::intToText(currentMetrics.timeText, 9, currentMetrics.time);
	currentMetrics.difficulty = currentDifficulty;


}

game::~game()
{
	for (int i = 0; i < blocksWidth; i++)
	{
		for(int j = 0; j < blocksHeight; j++)
		{
            delete renderScreenBlocks[i][j];
        }
    }
}

void game::gameLoop()
{
	countTime.calculateDeltaTime();
	float deltaTime = countTime.getDeltaTime();
	count -= deltaTime;
	runningTime += deltaTime;


	if (keys[L'W'] || keys[VK_UP])
	{

		keys[L'W'] = false;
		keys[VK_UP] = false;
	}
	if (keys[L'D'] || keys[VK_RIGHT])
	{

		keys[L'D'] = false;
		keys[VK_RIGHT] = false;
	}
	if (keys[L'S'] || keys[VK_DOWN])
	{

		keys[L'S'] = false;
		keys[VK_DOWN] = false;
	}
	if (keys[L'A'] || keys[VK_LEFT])
	{

		keys[L'A'] = false;
		keys[VK_LEFT] = false;
	}
	if ((int)runningTime > currentMetrics.time)
	{
		currentMetrics.time = (int)runningTime;
		helper::intToText(currentMetrics.timeText, 9, currentMetrics.time);
		timeDisplay.changeText(currentMetrics.timeText);
	}
}

void game::render()
{
	for (int i = 0; i < blocksWidth; i++)
	{
		for(int j = 0; j < blocksHeight; j++)
		{
			renderScreenBlocks[i][j]->render();
		}
	}
	flagsLabel.render();
    flagsDisplay.render();
	timeLabel.render();
    timeDisplay.render();
}

bool game::testDeath()
{
	return dead;
}

bool game::testWin()
{
	return win;
}

void game::resetDeltaTime()
{
	countTime.calculateDeltaTime();
	countTime.calculateDeltaTime();
}

void game::populateMetrics(game::gameMetrics& populatedMetrics)
{
	populatedMetrics = currentMetrics;
}

void game::resize(RECT newScreen)
{
	currentScreenSize = newScreen;
	float blockSize = (newScreen.right * gameSquareBoundary.width)/(float)blocksWidth < (newScreen.bottom * gameSquareBoundary.height)/(float)blocksHeight ? (newScreen.right * gameSquareBoundary.width)/(float)blocksWidth : (newScreen.bottom * gameSquareBoundary.height)/(float)blocksHeight;
	D2D1_POINT_2F middle = {(newScreen.right * gameSquareBoundary.width)/2.0f + newScreen.right * gameSquareBoundary.left, (newScreen.bottom * gameSquareBoundary.height)/2.0f + newScreen.bottom * gameSquareBoundary.top};
	for (int i = 0; i < blocksWidth; i++)
	{
		for(int j = 0; j < blocksHeight; j++)
		{
			renderScreenBlocks[i][j]->resize({((middle.y - blockSize*(blocksHeight/2.0f)) + j*blockSize)/newScreen.bottom, ((middle.x - blockSize*(blocksWidth/2.0f)) + i*blockSize)/newScreen.right, blockSize/newScreen.right, blockSize/newScreen.bottom}, newScreen);
		}
	}
	flagsLabel.resize(newScreen);
    flagsDisplay.resize(newScreen);
	timeLabel.resize(newScreen);
    timeDisplay.resize(newScreen);
	gameSquare = {(middle.y - blockSize*(blocksHeight/2.0f))/newScreen.bottom, (middle.x - blockSize*(blocksWidth/2.0f))/newScreen.right, (blockSize*blocksWidth)/newScreen.right, (blockSize*blocksHeight)/newScreen.bottom};
	screenSize = newScreen;
}

void game::onKey(wchar_t inputChar)
{
	keys[inputChar] = false;
}

void game::keyDown(wchar_t inputChar)
{
	keys[inputChar] = true;
}

void game::onMove(D2D1_POINT_2F moved)
{
	for (int i = 0; i < blocksWidth; i++)
	{
		for (int j = 0; j < blocksHeight; j++)
		{
			renderScreenBlocks[i][j]->onMove(moved);
		}
	}
}

void game::onClick(D2D1_POINT_2F clicked)
{
    float boxWidth = gameSquare.width * screenSize.right;
    float mousePercentW = clicked.x - gameSquare.left * screenSize.right;
    float posX = mousePercentW/boxWidth;
	posX *= blocksWidth;
    float boxHeight = gameSquare.height * screenSize.bottom;
    float mousePercentH = clicked.y - gameSquare.top * screenSize.bottom;
    float posY = mousePercentH/boxHeight;
    posY *= blocksHeight;
	if ((int)posX >= blocksWidth || (int)posY >= blocksHeight || (int)posX < 0 || (int)posY < 0)
	{
		return;
	}
	if (firstClick)
	{
		renderScreenBlocks[(int)posX][(int)posY]->setStart();

		if ((int)posX > 0 && (int)posY > 0)
		{
			renderScreenBlocks[(int)posX-1][(int)posY-1]->setStart();
		}
		if ((int)posY > 0)
		{
			renderScreenBlocks[(int)posX][(int)posY-1]->setStart();
		}
		if ((int)posX < blocksWidth-1 && (int)posY > 0)
		{
			renderScreenBlocks[(int)posX+1][(int)posY-1]->setStart();
		}
		if ((int)posX > 0)
		{
			renderScreenBlocks[(int)posX-1][(int)posY]->setStart();
		}
		if ((int)posX < blocksWidth-1)
		{
			renderScreenBlocks[(int)posX+1][(int)posY]->setStart();
		}
		if ((int)posX > 0 && (int)posY < blocksHeight-1)
		{
			renderScreenBlocks[(int)posX-1][(int)posY+1]->setStart();
		}
		if ((int)posY < blocksHeight - 1)
		{
			renderScreenBlocks[(int)posX][(int)posY+1]->setStart();
		}
		if ((int)posX < blocksWidth - 1 && (int)posY < blocksHeight - 1)
		{
			renderScreenBlocks[(int)posX+1][(int)posY+1]->setStart();
		}
		calculateBombs();
		firstClick = false;
	}
	updateBoard((int) posX, (int) posY);
}

void game::onRClick(D2D1_POINT_2F clicked)
{
    float boxWidth = gameSquare.width * screenSize.right;
    float mousePercentW = clicked.x - gameSquare.left * screenSize.right;
    float posX = mousePercentW/boxWidth;
	posX *= blocksWidth;
    float boxHeight = gameSquare.height * screenSize.bottom;
    float mousePercentH = clicked.y - gameSquare.top * screenSize.bottom;
    float posY = mousePercentH/boxHeight;
    posY *= blocksHeight;
	if ((int)posX >= blocksWidth || (int)posY >= blocksHeight)
	{
		return;
	}
	renderScreenBlocks[(int)posX][(int)posY]->toggleFlag();
	if (renderScreenBlocks[(int)posX][(int)posY]->getFlag())
	{
		currentMetrics.flags++;
	}
	else
	{
		currentMetrics.flags--;
	}
	helper::intToText(currentMetrics.flagsText, 9, currentMetrics.flags);
	flagsDisplay.changeText(currentMetrics.flagsText);
}
void game::updateBoard(int x, int y)
{
	if (renderScreenBlocks[x][y]->checkOpen() || renderScreenBlocks[x][y]->getFlag())
	{
		return;
	}

	renderScreenBlocks[x][y]->open();
	numOpened++;
	win = numOpened == blocksWidth * blocksHeight - numBombs;
	if (renderScreenBlocks[x][y]->getNumBombs() > 0)
	{
		return;
	}

	if (x > 0 && y > 0)
	{
		updateBoard(x-1, y-1);
	}
	if (y > 0)
	{
		updateBoard(x, y-1);
	}
	if (x < blocksWidth-1 && y > 0)
	{
		updateBoard(x+1, y-1);
	}
	if (x > 0)
	{
		updateBoard(x-1, y);
	}
	if (x < blocksWidth-1)
	{
		updateBoard(x+1, y);
	}
	if (x > 0 && y < blocksHeight-1)
	{
		updateBoard(x-1, y+1);
	}
	if (y < blocksHeight - 1)
	{
		updateBoard(x, y+1);
	}
	if (x < blocksWidth - 1 && y < blocksHeight - 1)
	{
		updateBoard(x+1, y+1);
	}
}
void game::onCapture(D2D1_POINT_2F captrue)
{

}



void game::calculateBombs() 
{

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distribW(0, blocksWidth-1);
	std::uniform_int_distribution<> distribH(0, blocksHeight-1);
	int bombX = distribW(gen);
	int bombY = distribH(gen);
	for (int i = 0; i < numBombs; i++)
	{
		while(renderScreenBlocks[bombX][bombY]->checkBomb() || renderScreenBlocks[bombX][bombY]->getStart())
		{
			bombX = distribW(gen);
			bombY = distribH(gen);
		}

		renderScreenBlocks[bombX][bombY]->setBomb();

		if (bombX > 0 && bombY > 0)
		{
			renderScreenBlocks[bombX-1][bombY-1]->addBomb();
		}
		if (bombY > 0)
		{
			renderScreenBlocks[bombX][bombY-1]->addBomb();
		}
		if (bombX < blocksWidth-1 && bombY > 0)
		{
			renderScreenBlocks[bombX+1][bombY-1]->addBomb();
		}
		if (bombX > 0)
		{
			renderScreenBlocks[bombX-1][bombY]->addBomb();
		}
		if (bombX < blocksWidth-1)
		{
			renderScreenBlocks[bombX+1][bombY]->addBomb();
		}
		if (bombX > 0 && bombY < blocksHeight-1)
		{
			renderScreenBlocks[bombX-1][bombY+1]->addBomb();
		}
		if (bombY < blocksHeight - 1)
		{
			renderScreenBlocks[bombX][bombY+1]->addBomb();
		}
		if (bombX < blocksWidth - 1 && bombY < blocksHeight - 1)
		{
			renderScreenBlocks[bombX+1][bombY+1]->addBomb();
		}
	}
}