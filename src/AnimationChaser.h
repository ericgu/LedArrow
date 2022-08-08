
class AnimationChaser : public IAnimation
{
private:
  int _red;
  int _blue;
  int _green;
  int _on;
  int _inc;
  int _countBetweenChanges;
  int _fade;

  int _current;
  int _count;

public:

  AnimationChaser(Strip* pStrip) :
    IAnimation(pStrip, "Chaser")
  {
  }

  bool ProcessMessage(Command* pCommand)
  {
    // chase r,g,b,on,inc,count,fade
    // r,g,b => color
    // on => # of on pixels
    // inc => # of pixels to increment each time
    // count => cycles between move
    // fade => 0 == all the same color, 1 == fade down from center
    // chase_0,200,0,1,1,1,0

    if (pCommand->IsMatch("chase", 7))
    {
      _red   = pCommand->_values[0];
      _green = pCommand->_values[1];
      _blue  = pCommand->_values[2];
      _on = pCommand->_values[3];
      _inc = pCommand->_values[4];
      _countBetweenChanges = pCommand->_values[5];
      _fade = pCommand->_values[6];
      _current = 0;
      _count = 0;
      
      return true;
    }

    return false;
  }

  void Update()
  {
    if (_count == 0)
    {
      RgbColor black(0, 0, 0);
      for (int led = 0; led < GetPixelCount(); led++)
      {
        SetPixelColor(led, black);
      }

      float red = _red;
      float green = _green;
      float blue = _blue;

      float redDelta = _fade ? red / _on : 0;
      float greenDelta = _fade ? green / _on : 0;
      float blueDelta = _fade ? blue / _on : 0;

      for (int led = 0; led < _on; led++)
      {
        int index = (_current + GetPixelCount() - led) % GetPixelCount();
        SetPixelColor(index, RgbColor((int) red, (int) green, (int) blue));
        red -= redDelta;
        green -= greenDelta;
        blue -= blueDelta;
      }

      _current = (_current + _pStrip->GetPixelCount() + _inc) % _pStrip->GetPixelCount();

      _pStrip->Show();
    }

    _count = (_count + 1) % _countBetweenChanges;
  }
};

