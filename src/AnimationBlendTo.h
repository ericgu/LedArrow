
class AnimationBlendTo: public IAnimation
{
  private:
    RgbColor _baseColor;
    RgbColor _targetColor;
    int _steps;
    int _currentStep = 0;

    float _deltaRed;
    float _deltaGreen;
    float _deltaBlue;
  
  public: 

    AnimationBlendTo(Strip* pStrip) : 
        IAnimation(pStrip, "Blend")
        {}

    bool ProcessMessage(Command* pCommand)
    {
      // rgb rrr,ggg,bbb,cccc
      if (pCommand->IsMatch("rgb", 4))
      {
        RgbColor color = RgbColor(pCommand->_values[0], pCommand->_values[1], pCommand->_values[2]);
        int count = pCommand->_values[3];

        DoBlend(color, count);
        
        return true;
      }

      return false;
    }
      
    void DoBlend(RgbColor targetColor, int steps)
    {
      _baseColor = _targetColor; // the old target color...
      _targetColor = targetColor;
      _steps = steps;
      _currentStep = 0;
    }
  
    RgbColor GetColor()
    {
      if (_currentStep < _steps)
      {
        _currentStep++;
      }

      float factor = _currentStep / (float) _steps;
      int red = _baseColor.R + (int) ((_targetColor.R - _baseColor.R) * factor);
      int green = _baseColor.G + (int) ((_targetColor.G - _baseColor.G) * factor);
      int blue = _baseColor.B + (int) ((_targetColor.B - _baseColor.B) * factor);

      return RgbColor(red, green, blue);
    }

    void Update()
    {
      RgbColor color = GetColor();

      SetAllPixelColorWithGammaAndShow(color);
    }
};

