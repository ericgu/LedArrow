class AnimationAlternate: public IAnimation
{
  private:
    RgbColor _firstColor;
    RgbColor _secondColor;
    int _count;
    int _currentCount = 0;
 
  public: 

    AnimationAlternate(Strip* pStrip) : 
        IAnimation(pStrip, "Alternate")
        {}

    // alt rrr,ggg,bbb,rrr,ggg,bbb,cccc
    //      (first)       (second)  (count)
    bool ProcessMessage(Command* pCommand)
    {
      if (pCommand->IsMatch("alt", 7))
      {
        _firstColor = RgbColor(pCommand->_values[0], pCommand->_values[1], pCommand->_values[2]);
        _secondColor = RgbColor(pCommand->_values[3], pCommand->_values[4], pCommand->_values[5]);
        _count = pCommand->_values[6];
        _currentCount = 0;

        return true;
      }

      return false;
    }  

    void Update()
    {
      _currentCount = (_currentCount + 1) % (_count + _count);
      
      RgbColor color = _currentCount < _count ? _firstColor : _secondColor;

      SetAllPixelColorWithGammaAndShow(color);
    }      
};

