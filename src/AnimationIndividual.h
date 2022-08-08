
class AnimationIndividual: public IAnimation
{
  private:
    RgbColor* _pColors;
    bool _rendered;
 
  public: 

    AnimationIndividual(Strip* pStrip) :
        IAnimation(pStrip, "Individual")
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
      int red   = GetHexCharacter(*(pMessage + 0)) * 16 + GetHexCharacter(*(pMessage + 1));
      int green = GetHexCharacter(*(pMessage + 2)) * 16 + GetHexCharacter(*(pMessage + 3));
      int blue  = GetHexCharacter(*(pMessage + 4)) * 16 + GetHexCharacter(*(pMessage + 5));
      pMessage += 6;

      return RgbColor(red, green, blue);
    }
    
    // ind bbb,RRGGBBRRGGBB...
    // block size (number for each color), followed by 2 digit Hex color value
    bool ProcessMessage(Command* pCommand)
    {
      if (pCommand->StartsWith("ind")) // individual with specified chunk size.
      //if (strstr(pMessage, "ind") == pMessage) // individual with specified chunk size.
      {
        const char* pMessage = pCommand->_message.c_str();
        
        pMessage = Command::SkipToFirstDigit(pMessage);

        int blockSize = atoi(pMessage);

        pMessage = Command::SkipToFirstNonDigit(pMessage);
        pMessage = Command::SkipToFirstHexDigit(pMessage);

        Serial.println(pMessage);

        // two hex bytes for color of each led

        RgbColor* pColor = _pColors;
        for (int led = 0; led < GetPixelCount(); led += blockSize)
        {
          RgbColor rgbColor = GetRgbColorFromBytes(pMessage);
          pMessage += 6;

          for (int block = 0; block < blockSize; block++)
          {
            *pColor = rgbColor;
            pColor++;
          }
        }
        
        _rendered = false;
        
        return true;
      }

      return false;
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

