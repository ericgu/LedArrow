class TaskProcessMessages : public Task
{
public:
    TaskProcessMessages(uint32_t timeInterval, LEDStrip* pStrip, FaderGroup* pFaderGroup) : // pass any custom arguments you need
        Task(timeInterval),
        _pFaderGroup(pFaderGroup),
        _pStrip(pStrip)
    { 
      _value = 0;
      _valueIncrement = 0.05;
    };

    void Init()
    {
      _maximumValue = _pFaderGroup->GetTotalDistance();
    }

private:
    FaderGroup* _pFaderGroup;
    LEDStrip* _pStrip;

    float _value;
    float _valueIncrement;
    float _maximumValue;

    virtual bool OnStart() // optional
    {
        return true;
    }

    virtual void OnStop() // optional
    {

    }

    void SetPixelsTopBottom(int pixelNumber, RgbColorReal color)
    {
      RgbColor rgbColor = color.GetRgbColor();
      _pStrip->SetPixelColor(pixelNumber, rgbColor);
      _pStrip->SetPixelColor(50 - pixelNumber, rgbColor);
    }

    virtual void OnUpdate(uint32_t deltaTime)
    {
      const float normalSpacing = 0.95F;
      const float headSpacing = 0.708F;    

      //Serial.println("OnUpdate");
      //Serial.flush();


      _value = _value + _valueIncrement;
      if (_value >= _maximumValue)
      {
        _value = 0;
        Serial.print("0");
        Serial.flush();
      }

      RgbColorReal color;

      float current = _value;

      color = _pFaderGroup->GetCurrent(current);
      _pStrip->SetPixelColor(0, color.GetRgbColor());
      current += headSpacing;

      for (int pixelNumber = 1; pixelNumber < 6; pixelNumber++)
      {
        SetPixelsTopBottom(pixelNumber, _pFaderGroup->GetCurrent(current));
        current += headSpacing;
      }

      // back of arrow head...
      color = _pFaderGroup->GetCurrent(current);
      SetPixelsTopBottom(6, _pFaderGroup->GetCurrent(current));
      SetPixelsTopBottom(7, _pFaderGroup->GetCurrent(current));
      SetPixelsTopBottom(8, _pFaderGroup->GetCurrent(current));
      current += normalSpacing;

      // arrow tail
      for (int pixelNumber = 9; pixelNumber < 23; pixelNumber++)
      {
        SetPixelsTopBottom(pixelNumber, _pFaderGroup->GetCurrent(current));
        current += normalSpacing;
      }

      // back of arrow
      color = _pFaderGroup->GetCurrent(current);
      for (int pixelNumber = 23; pixelNumber < 28; pixelNumber++)
      {
        _pStrip->SetPixelColor(pixelNumber, color.GetRgbColor());
      }

      _pStrip->Show();
    }     
};
