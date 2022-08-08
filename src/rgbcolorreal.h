class RgbColorReal
{
public:
    float Red;
    float Green;
    float Blue;

    RgbColorReal(float red, float green, float blue)
    {
        Red = red;
        Green = green;
        Blue = blue;
    }

    RgbColorReal()
    {
        Red = 0;
        Green = 0;
        Blue = 0;
    }

    RgbColor GetRgbColor()
    {
        uint8_t red = (uint8_t)(255.0 * Red);
        uint8_t green = (uint8_t)(255.0 * Green);
        uint8_t blue = (uint8_t)(255.0 * Blue);

        return RgbColor(red, green, blue);
    }
};