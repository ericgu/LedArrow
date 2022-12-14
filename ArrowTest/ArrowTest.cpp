// ArrowTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define uint8_t unsigned char

class RgbColor
{
private:
public:
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;

    RgbColor(char red, char green, char blue)
    {
        Red = red;
        Green = green;
        Blue = blue;
    }

};

#include "..\src\RgbColorReal.h"
#include "..\src\Fader.h"
#include "..\src\FaderGroup.h"


int main()
{
    FaderGroup faderGroup(5);

    faderGroup.Add(new Fader(0.5, 0, 0, 1, 1, 1, 0));   // blue to yellow
    faderGroup.Add(new Fader(0.5, 1, 1, 0, 1, 1, 0));   // yellow
    faderGroup.Add(new Fader(3, 1, 1, 0, 1, 0, 0));     // yellow to red
    faderGroup.Add(new Fader(2, 1, 0, 0, 0, 0, 1));     // red to blue
    faderGroup.Add(new Fader(10, 0, 0, 1, 0, 0, 1));     // blue

    for (float value = 0; value < 20; value += 0.1F)
    {
        RgbColorReal colorReal = faderGroup.GetCurrent(value);
        RgbColor color = colorReal.GetRgbColor();

        printf("%f %d %d %d\n", value, color.Red, color.Green, color.Blue);
    }



    return 0;
}

