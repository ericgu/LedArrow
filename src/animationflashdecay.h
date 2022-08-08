
class AnimationFlashDecay: public IAnimation
{
  private:
    float _hue;
    RgbColor _baseColor;
    int _stepsOn;
    int _stepsWaitMin;
    int _stepsWaitMax;
    int _stepsWait;
    int _currentStep = 0;

    float _saturation = 1.0;
    float _brightness = 0.25;

    int _red;
    int _green;
    int _blue;
  
  public: 

    AnimationFlashDecay(Strip* pStrip) : 
        IAnimation(pStrip, "Flash decay")
        {}

    bool ProcessMessage(Command* pCommand)
    {
      // fdc cccc,wmin,wmax

      if (pCommand->IsMatch("fdc", 3))
      {
        _stepsOn = pCommand->_values[0];
        _stepsWaitMin = pCommand->_values[1];
        _stepsWaitMax = pCommand->_values[2];

        return true;
      }

      return false;
    }

    void Next()
    {
      int red = ESP8266TrueRandom.random(255);
      int green = ESP8266TrueRandom.random(255);
      int blue = ESP8266TrueRandom.random(255);

      int stepsWait = _stepsWaitMin + ESP8266TrueRandom.random(_stepsWaitMax - _stepsWaitMin);

      SetColorAndSteps(red, green, blue, _stepsOn, stepsWait);
    }

    void SetColorAndSteps(int red, int green, int blue, int stepsOn, int stepsWait)
    {
      int max = red;
      if (green > max)
      {
        max = green;
      }
      if (blue > max)
      {
        max = blue;
      }
      //cout << "max: " << max << endl;
      //cout << "set1: " << red << " " << green << " " << blue << endl;

      float factor = 255.0 / max;  // factor to bump all the colors up by...
      //cout << "factor: " << factor << endl;

      _red = (int)(red * factor);
      _green = (int)(green * factor);
      _blue = (int)(blue * factor);

      //cout << "set2: " << _red << " " << _green << " " << _blue << endl;

      _stepsWait = stepsWait;
      _stepsOn = stepsOn;
    }
      
    RgbColor GetColor()
    {
      //cout << "Getcolor" << endl;

      if (_currentStep == 0)
      {
        Next();
      }
      
      _currentStep = (_currentStep + 1) % (_stepsOn + _stepsWait);

      if (_currentStep < _stepsOn)
      {
        float brightness = 1.0 - (1.0 * _currentStep) / _stepsOn;

        return RgbColor((int)(_red * brightness), (int)(_green * brightness), (int)(_blue * brightness));
      }
      else
      {
        return RgbColor(0, 0, 0);
      }
    }

    void Update()
    {
      RgbColor color = GetColor();

      SetAllPixelColorWithGammaAndShow(color);
    }
};

