class TaskProcessMessages : public Task
{
public:
    TaskProcessMessages(uint8_t ledPin, uint32_t timeInterval, LEDStrip* pStrip, FaderGroup* pFaderGroup) : // pass any custom arguments you need
        Task(timeInterval),
        _ledPin(ledPin),
        _ledOn(false),
        _pFaderGroup(pFaderGroup),
        //_pWifiHandler(pWifiHandler),
        _pStrip(pStrip)
    { 
      _ledUpdateCountInitialValue = 500 / _timeInterval;
      _ledUpdateCount = _ledUpdateCountInitialValue;
      _value = 0;
      _valueIncrement = 0.03;
      _pStrip->Init();
    };

    void Init()
    {
      //_udp.begin(localUdpPort);
      //Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
      _maximumValue = _pFaderGroup->GetTotalDistance();
    }

private:
    FaderGroup* _pFaderGroup;



    //const unsigned int localUdpPort = 4210;  // local port to listen on
    //const int udpLogCountMax = 100 * 60; // once per minute...

    const uint8_t _ledPin; // const means can't change other than in constructor
    bool _ledOn;
    //WifiHandler* _pWifiHandler;
    //WiFiUDP _udp;
    //PixelHandler* _pPixelHandler;
    LEDStrip* _pStrip;
    char _buffer[1024];
    int _packetCount= 0;
    int _ledUpdateCountInitialValue;
    int _ledUpdateCount;
    int _updatesWithoutPackets = 0;
    int _updateCount = 0;
    int _udpLogCount = 0;
    float _value;
    float _valueIncrement;
    float _maximumValue;

    virtual bool OnStart() // optional
    {
        // put code here that will be run when the task starts
        _ledOn = false;
        pinMode(_ledPin, OUTPUT);
        return true;
    }

    virtual void OnStop() // optional
    {
        // put code here that will be run when the task stops
        _ledOn = false;
        digitalWrite(_ledPin, LOW);    // turn the LED off by making the voltage LOW
    }

#if fred
    void UpdateHandleUdp()
    {
      int packetSize = _udp.parsePacket();
      if (packetSize)
      {
        // receive incoming UDP packets
        //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, _udp.remoteIP().toString().c_str(), _udp.remotePort());
        int len = _udp.read(_buffer, sizeof(_buffer) - 1);
        if (len > 0)
        {
          _buffer[len] = 0;
        }

        if (_packetCount % 1000 == 0)
        {
          Serial.print(_packetCount);
          Serial.print(" ");
          Serial.println(system_get_free_heap_size());
          //Serial.println(missedMessages);
        }
        _packetCount++;

        _pPixelHandler->ProcessMessage(_buffer);
      }
      else
      {
        _updatesWithoutPackets++;
      }
    }
    #endif

    void UpdateHandleLed()
    {
      _ledUpdateCount--;
      if (_ledUpdateCount == 0)
      {
        _ledUpdateCount = _ledUpdateCountInitialValue;

        digitalWrite(_ledPin, _ledOn ? LOW : HIGH);
        _ledOn = !_ledOn; // toggle led state
      }

      _updateCount++;
      if (_updateCount % 1000 == 0)
      {
        //Serial.print("Updates without packets: ");
        //Serial.println(_updatesWithoutPackets);
      }
    }

#if fred
    void UpdatePixelHandler()
    {
      _pPixelHandler->Update();
    }

    static void WifiStatusHandler(int status, void* userData)
    {
      TaskProcessMessages* pTaskProcessMessages = (TaskProcessMessages*) userData;

      if (status == STATE_AP)
      {
        pTaskProcessMessages->_pPixelHandler->SetAccessPointAnimation();
      }
      else if (status == STATE_STA)
      {
        pTaskProcessMessages->_pPixelHandler->SetProvisionedAnimation();
      }
    }

    void UpdateUdpLogger()
    {
      _udpLogCount++;

      if (_udpLogCount > udpLogCountMax)
      {
        UdpLogger.println("Heartbeat");
        _udpLogCount = 0;
      }
    }
#endif


    

    virtual void OnUpdate(uint32_t deltaTime)
    {

      _value = _value + _valueIncrement;
      if (_value >= _maximumValue)
      {
        _value = 0;
      }

      RgbColorReal color;
      
      color = _pFaderGroup->GetCurrent(_value);
      Serial.print(color.Red);
      Serial.print(" ");
      Serial.print(color.Green);
      Serial.print(" ");
      Serial.println(color.Blue);
      _pStrip->SetPixelColor(12, color.GetRgbColor());

      color = _pFaderGroup->GetCurrent(_value + 1);
      _pStrip->SetPixelColor(11, color.GetRgbColor());
      _pStrip->SetPixelColor(13, color.GetRgbColor());

      color = _pFaderGroup->GetCurrent(_value + 2);
      _pStrip->SetPixelColor(10, color.GetRgbColor());
      _pStrip->SetPixelColor(14, color.GetRgbColor());

      color = _pFaderGroup->GetCurrent(_value + 3);
      _pStrip->SetPixelColor(8, color.GetRgbColor());
      _pStrip->SetPixelColor(9, color.GetRgbColor());
      _pStrip->SetPixelColor(15, color.GetRgbColor());
      _pStrip->SetPixelColor(16, color.GetRgbColor());

      for (int led = 0; led < 6; led++)
      {
        color = _pFaderGroup->GetCurrent(_value + 4 + led);
        _pStrip->SetPixelColor(7 - led, color.GetRgbColor());
        _pStrip->SetPixelColor(17 + led, color.GetRgbColor());
      }

      color = _pFaderGroup->GetCurrent(_value + 10);
      _pStrip->SetPixelColor(0, color.GetRgbColor());
      _pStrip->SetPixelColor(1, color.GetRgbColor());
      _pStrip->SetPixelColor(23, color.GetRgbColor());

      _pStrip->Show();

      //Serial.println("update1"); Serial.flush();
      //UpdateHandleUdp();
      //Serial.println("update2"); Serial.flush();
      
      //UpdateHandleLed();
      //Serial.println("update3"); Serial.flush();

      //_pWifiHandler->Init(WifiStatusHandler, this);
      //Serial.println("update4"); Serial.flush();
 
      //UpdatePixelHandler();
      //Serial.println("update5"); Serial.flush();

      //UpdateUdpLogger();
    }     
};
