
class AnimationSetChunk: public IAnimation
{
  private:
    RgbColor* _pColors;
    bool _rendered;
 
  public: 

    AnimationSetChunk(Strip* pStrip) : 
        IAnimation(pStrip, "Set chunk")
    {
      _pColors = new RgbColor[GetPixelCount()];      
    }

    int GetHexCharacter(const char ch)
    {
      static char Hex[] = "0123456789abcdef";
      char * found = strchr(Hex, tolower(ch));
      return found ? found - Hex: 0;
    }

    RgbColor GetRgbColorFromBytes(const char* pMessage)
    {
      int red   = GetHexCharacter(*pMessage) * 16 + GetHexCharacter(*(pMessage + 1));
      int green = GetHexCharacter(*(pMessage + 2)) * 16 + GetHexCharacter(*(pMessage + 3));
      int blue  = GetHexCharacter(*(pMessage + 4)) * 16 + GetHexCharacter(*(pMessage + 5));
      pMessage += 6;

      return RgbColor(red, green, blue);
    }
    
    // chunk count,r,g,b<,count,r,g,b>...
    // chunk, where each chunk has a number of pixels to set and the rgb value to use.
    bool ProcessMessage(Command* pCommand)
    {
      if (pCommand->StartsWith("chunk")) // individual with specified chunk size.
      {
        int currentPixel = 0;

        int i = 0;
        while (i < pCommand->_count)
        {
          int chunk = pCommand->_values[i++];
          int r = pCommand->_values[i++];                  
          int g = pCommand->_values[i++];                  
          int b = pCommand->_values[i++];
          RgbColor color(r, g, b);

          for (int p = currentPixel; p < currentPixel + chunk; p++)
          {
            _pColors[p] = color;
          }
          currentPixel += chunk;
        }
        
        _rendered = false;
        
        return true;
      }

      return false;
    }  

    RgbColor GetColor(int pixelIndex)
    {
      return _pColors[pixelIndex];
    }

    void Update()
    {
      if (!_rendered)
      {
        RgbColor* pColor = _pColors;

        for (int led = 0; led < GetPixelCount(); led++)
        {
          SetPixelColor(led, *pColor);
          pColor++;
        }
        _pStrip->Show(); 
        
        _rendered = true;
      }
    }      
};

