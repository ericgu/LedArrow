
class AnimationColorRotate: public IAnimation
{
  private:
    int _colorNumber;
    int _steps;
    int _currentStep = 0;

    int _brightness;
    int _brightnessTwoColor = (int) (_brightness * 0.7);

    RgbColor _red;
    RgbColor _yellow;
    RgbColor _green;
    RgbColor _cyan;
    RgbColor _blue;
    RgbColor _violet;
  
  public: 

    AnimationColorRotate(Strip* pStrip) : 
        IAnimation(pStrip, "Color rotate"),
      _colorNumber(0)
        {}

    bool ProcessMessage(Command* pCommand)
    {
      // col cccc,bbbb
      if (pCommand->IsMatch("col", 2))
      {
        _steps = pCommand->_values[0] / 6;
        int brightness = pCommand->_values[1];
        int brightnessTwoColor = (int)(brightness * 0.85);

        _red = RgbColor(brightness, 0, 0);
        _yellow = RgbColor(brightnessTwoColor, brightnessTwoColor, 0);
        _green  = RgbColor(0, brightness, 0);
        _cyan = RgbColor(0, brightnessTwoColor, brightnessTwoColor);
        _blue = RgbColor(0, 0, brightness);
        _violet = RgbColor(brightnessTwoColor, 0, brightnessTwoColor);

        return true;
      }

      return false;
    }
      
    int Interpolate(int startValue, int endValue, float factor)
    {
      return startValue + (int)((endValue - startValue) * factor);
    }

    RgbColor GetColor()
    {
      if (_currentStep >= _steps || _currentStep < 0)
      {
        _currentStep = 0;

        _colorNumber = (_colorNumber + 1) % 6;
      }

      RgbColor start(0, 0, 0);
      RgbColor end(0, 0, 0);

      switch (_colorNumber)
      {
      case 0:
        start = _red;
        end = _yellow;
        break;

      case 1:
        start = _yellow;
        end = _green;
        break;

      case 2:
        start = _green;
        end = _cyan;
        break;

      case 3:
        start = _cyan;
        end = _blue;
        break;

      case 4:
        start = _blue;
        end = _violet;
        break;

      case 5:
        start = _violet;
        end = _red;
        break;
      }

      float factor = ((float) _currentStep) / _steps;
      _currentStep++;

      RgbColor current(
        Interpolate(start.R, end.R, factor),
        Interpolate(start.G, end.G, factor),
        Interpolate(start.B, end.B, factor));

      return current;
    }

    void Update()
    {
      RgbColor color = GetColor();
      //Serial.print(color.R);
      //Serial.print(" ");
      //Serial.print(color.G);
      //Serial.print(" ");
      //Serial.println(color.B);

      SetAllPixelColorWithGammaAndShow(color);
    }
};

