#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "block.h"
#include "textBox.h"
#include "minesweeperBlock.h"
#include "helpers.h"
#include "time.h"

#include "songManager.h"

#include <random>

class game
{
public:
struct leaderboard {
	wchar_t name[4][20];
	int score[4];
	wchar_t scoreText[4][10];
};
enum difficulty
{
	easy,
	normal,
	hard,
	master,
	numDifficulty
};
struct gameMetrics
{
	wchar_t name[20];
	int score;
	wchar_t scoreText[10];
	int time;
	wchar_t timeText[10];
	difficulty difficulty;
};
static const int numMines[numDifficulty];
private:
textBox scoreLabel;
textBox scoreDisplay;
textBox timeLabel;
textBox timeDisplay;
gameMetrics currentMetrics;
float runningTime;
int blocksWidth;
int blocksHeight;
minesweeperBlock* renderScreenBlocks[25][25];
static const block::location gameSquareBoundary;
RECT screenSize;
block::location game::gameSquare;
RECT currentScreenSize;
Time countTime;
float count;
bool keys[0xffff];
bool dead;
bool win;
bool firstClick;
int numBombs;
songManager effectGenerator;
public:
game(ID2D1HwndRenderTarget* renderTarget, IDWriteFactory* pDWriteFactory, RECT screenSize, wchar_t playerName[20], int width, int height, game::difficulty currentDifficulty, songManager& effectGenerator);
~game();
void render();
void gameLoop();
void onMove(D2D1_POINT_2F moved);
void onClick(D2D1_POINT_2F clicked);
void onRClick(D2D1_POINT_2F clicked);
void onCapture(D2D1_POINT_2F captrue);
void keyDown(wchar_t inputChar);
void onKey(wchar_t inputChar);
void resize(RECT newScreen);
bool testDeath();
bool testWin();
void resetDeltaTime();
void populateMetrics(game::gameMetrics& populatedMetrics);
private:
void calculateBombs();
void updateBoard(int x, int y);
};