
class AnimationRandomFlash: public IAnimation
{
  private:
    int _currentFlashTime = 0;
    int _currentFlashCount = 0;
    int _currentWait = 0;

    int _flashTime;
    int _flashCount;
    int _waitTime;
    bool _inFlash;
    
  public: 

    AnimationRandomFlash(Strip* pStrip) : 
        IAnimation(pStrip, "Random Flash")
        {}

    bool ProcessMessage(Command* pCommand)
    {
      // rf time,count,wait
      if (pCommand->IsMatch("rf", 3))
      {
        _flashTime = pCommand->_values[0];
        _flashCount = pCommand->_values[1];
        _waitTime = pCommand->_values[2];
        _inFlash = true;
        _currentFlashTime = 0;
        _currentFlashCount = 0;
        _currentWait = 0;

        return true;
      }

      return false;
    }

    int SetColor()
    {
      //cout << "SetColor: " << endl;
      int red = ESP8266TrueRandom.random(255);
      int green = ESP8266TrueRandom.random(255);
      int blue = ESP8266TrueRandom.random(255);

      int max = red;
      if (green > max)
      {
        max = green;
      }
      if (blue > max)
      {
        max = blue;
      }

      float factor = 255.0 / max;  // factor to bump all the colors up by...

      red = (int)(red * factor);
      green = (int)(green * factor);
      blue = (int)(blue * factor);

      SetAllPixelColorWithGammaAndShow(RgbColor(red, green, blue));

      return 0;
    }

    void Update()
    {
      //cout << "Update: " << _inFlash << " " << _currentFlashCount << " " << _currentFlashTime << " " << _currentWait << endl;

      if (_inFlash)
      {
        if (_currentFlashTime == 0)
        {
          _currentFlashCount = (_currentFlashCount + 1) % _flashCount;
          if (_currentFlashCount == 0)
          {
            _inFlash = false;
            _currentWait = 1;
          }
          else
          {
            SetColor();
          }
        }

        _currentFlashTime = (_currentFlashTime + 1) % _flashTime;
      }
      else
      {
        if (_currentWait == 0)
        {
          _inFlash = true;
        }
        _currentWait = (_currentWait + 1) % _waitTime;
      }
    }
};

