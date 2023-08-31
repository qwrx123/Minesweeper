#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "cyclingTextBox.h"

class autoCyclingTextBox : public cyclingTextBox
{
public:
autoCyclingTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t** myText, int arraySize, const int* outCycleNumber
        , int startCycle, block::style myStyle = block::boarder);
private:
public:
virtual bool render();
private:
int cycle;
const int* checkCycle;
};