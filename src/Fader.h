class Fader
{
    float _distance;
    TargetBrightness *_r1;
    TargetBrightness *_g1;
    TargetBrightness *_b1;
    TargetBrightness *_r2;
    TargetBrightness *_g2;
    TargetBrightness *_b2;

  public:
    Fader(float distance, TargetBrightness *r1, TargetBrightness *g1, TargetBrightness *b1, TargetBrightness *r2, TargetBrightness *g2, TargetBrightness *b2)
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
        (_r2->Value - _r1->Value) * ratio + _r1->Value,
        (_g2->Value - _g1->Value) * ratio + _g1->Value,
        (_b2->Value - _b1->Value) * ratio + _b1->Value);
  }

};