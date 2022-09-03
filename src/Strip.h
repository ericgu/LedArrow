
class LEDStrip
{
  private:
    NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>* _pStrip;
    int _pixelPin;
    int _pixelCount;

  public:
    LEDStrip(int pixelPin, int pixelCount) : 
        _pixelPin(pixelPin),
        _pixelCount(pixelCount)
    { 
      _pStrip = 0;
    }

    void Init()
    {
      if (_pStrip != 0)
      {
        Serial.println("Strip reinit!");
        return;
      }
      
      _pStrip = new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(_pixelCount, _pixelPin);
      _pStrip->Begin();
    }

    int GetPixelCount()
    {
      return _pixelCount;
    }    

    void SetPixelColor(int pixelIndex, RgbColor color)
    {
      _pStrip->SetPixelColor(pixelIndex, color);  
    }

    void Clear()
    {
      RgbColor black(0, 0, 0);
      for (int led = 0; led < _pixelCount; led++)
      {
        _pStrip->SetPixelColor(led, black);  
      }
      Show();
    }

    void Show()
    {
      _pStrip->Show();
    }
};
