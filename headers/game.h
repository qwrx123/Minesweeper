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
struct currentBlocks {
	int x;
	int y;
	int type;
	int turn;
};
// the drop down blocks you see
struct ghostBlocks {
	int x = 0;
	int y = 0;
	int type = 15;
};
struct leaderboard {
	wchar_t name[3][20];
	int score[3];
	wchar_t scoreText[3][10];
};
enum speeds
{
	slow,
	medium,
	fast,
	numSpeeds
};
struct gameMetrics
{
	wchar_t name[20];
	int score;
	wchar_t scoreText[10];
	int time;
	wchar_t timeText[10];
	speeds speed;
};
static const float floatSpeeds[numSpeeds];
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
RECT currentScreenSize;
Time countTime;
float count;
bool keys[0xffff];
bool dead;
bool win;
songManager effectGenerator;
public:
game(ID2D1HwndRenderTarget* renderTarget, IDWriteFactory* pDWriteFactory, RECT screenSize, wchar_t playerName[20], int width, int height, game::speeds currentSpeed, songManager& effectGenerator);
~game();
void render();
void gameLoop();
void keyDown(wchar_t inputChar);
void onKey(wchar_t inputChar);
void resize(RECT newScreen);
bool testDeath();
bool testWin();
void resetDeltaTime();
void populateMetrics(game::gameMetrics& populatedMetrics);
private:
void calculateApple();
};