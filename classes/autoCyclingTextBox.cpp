#include "../headers/autoCyclingTextBox.h"

autoCyclingTextBox::autoCyclingTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t** myText, int arraySize, const int* outCycleNumber
        , int startCycle, block::style myStyle)
    :cyclingTextBox(renderTarget, setLocation, screenSize, pDWriteFactory, myText,arraySize, &cycle
        , startCycle, myStyle), checkCycle(outCycleNumber)
{

}

bool autoCyclingTextBox::render()
{
    while (cycle != *checkCycle)
    {
        cyclingTextBox::cycleTextBox();
    }
    cyclingTextBox::render();
    return true;
}