class Fader
{
    float _distance;
    float _r1;
    float _g1;
    float _b1;
    float _r2;
    float _g2;
    float _b2;

public:
  Fader(float distance, float r1, float g1, float b1, float r2, float g2, float b2)
  {
      _distance = distance;
      _r1 = r1;
      _g1 = g1;
      _b1 = b1;
      _r2 = r2;
      _g2 = g2;
      _b2 = b2;
  }

  float GetDistance()
  {
    return _distance;
  }

  RgbColorReal GetColor(float value)
  {
    if (value > _distance)
    {
        return RgbColorReal(0, 0, 0);
    }

    float ratio = value / _distance;
    return RgbColorReal(
        (_r2 - _r1) * ratio + _r1, 
        (_g2 - _g1) * ratio + _g1, 
        (_b2 - _b1) * ratio + _b1);
  }

};