#include "../headers/game.h"
#include "../headers/scene.h"

const float game::floatSpeeds[game::speeds::numSpeeds] = {0.18, 0.135, 0.090};
const block::location game::gameSquareBoundary = {0.21, 0.3, 0.4, 0.74};

game::game(ID2D1HwndRenderTarget* renderTarget, IDWriteFactory* pDWriteFactory, RECT screenSize, wchar_t playerName[20], int width, int height, game::speeds currentSpeed, songManager& effectGenerator)
	:count(0), blocksWidth(width), blocksHeight(height), dead(false), win(false), runningTime(0),
	scoreLabel(renderTarget, {0.05, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"Score"), 
    scoreDisplay(renderTarget, {0.1, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"0"),
	timeLabel(renderTarget, {0.05, 0.8, 0.15, 0.05}, screenSize, pDWriteFactory, L"Time"), 
    timeDisplay(renderTarget, {0.1, 0.8, 0.15, 0.05}, screenSize, pDWriteFactory, L"0"),
	effectGenerator(std::move(effectGenerator))
{
	wcsncpy(currentMetrics.name, playerName, 20);
	for (int i = 0; i < blocksWidth; i++)
	{
		for(int j = 0; j < blocksHeight; j++)
		{
			renderScreenBlocks[i][j] = new minesweeperBlock(renderTarget, {0.1, 0.1, 0.1, 0.1}, screenSize);
		}
	}
	for (int i = 0; i < 0xffff; i++)
	{
		keys[i] = false;
	}
	currentMetrics.score = 0;
	helper::intToText(currentMetrics.scoreText, 9, currentMetrics.score);
	currentMetrics.time = 0;
	helper::intToText(currentMetrics.timeText, 9, currentMetrics.time);
	currentMetrics.speed = currentSpeed;
	for (int i = 0; i < 1; i++)
	{
		calculateApple();
	}

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
	scoreLabel.render();
    scoreDisplay.render();
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
	scoreLabel.resize(newScreen);
    scoreDisplay.resize(newScreen);
	timeLabel.resize(newScreen);
    timeDisplay.resize(newScreen);
}

void game::onKey(wchar_t inputChar)
{
	keys[inputChar] = false;
}

void game::keyDown(wchar_t inputChar)
{
	keys[inputChar] = true;
}





void game::calculateApple() 
{

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distribW(0, blocksWidth-1);
	std::uniform_int_distribution<> distribH(0, blocksHeight-1);
	int appleX = distribW(gen);
	int appleY = distribH(gen);

}