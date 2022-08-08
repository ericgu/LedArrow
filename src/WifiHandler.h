
#include "esp8266truerandom.h"
#include <Ethernet.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include "Networks.h"

#define STATE_UNKNOWN 0
#define STATE_AP 1
#define STATE_STARTING_STA 2
#define STATE_STA 3

class WifiHandler
{
  private:
    PersistentStorage* _pConfiguration;
    int _wifiState = STATE_UNKNOWN;
    int _lastWifiState = -1;
    bool _provisionOtherNodes = false;
    bool _saveAfterProvisioning = true;
    DNSServer* _pDnsServer;
    IPAddress _apIP;
    IPAddress _netMask;
    Networks _networks;

    const byte DNS_PORT = 53;

  public:
    WifiHandler() : 
      _apIP(192, 168, 4, 1),
      _netMask(255, 255, 255, 0)
    {
    }

    String LocalIp()
    {
      IPAddress ip = WiFi.localIP();

      String result;
      result += ip[0];
      for (int i = 1; i < 4; i++)
      {
        result += ".";
        result += ip[i];
      }

      return result;
    }

    String GetStatus()
    {
      switch (_wifiState)
      {
        case STATE_AP:
          return "Access Point Mode: waiting to provision";

        case STATE_STA:
          return String("Connected: ") + _pConfiguration->_ssid + " (" + LocalIp() + ")";

        default:
          return "Unknown";
      }
    }

    void SetSaveAfterProvisioning(bool saveAfterProvisioning)
    {
      _saveAfterProvisioning = saveAfterProvisioning;
    }

    void LoadConfiguration(PersistentStorage* pPersistentStorage)
    {
      _pConfiguration = pPersistentStorage;
      if (_pConfiguration->SsidGet().length() != 0)
      {
        _wifiState = STATE_STARTING_STA;
        Serial.println("STATE_STARTING_STA");
      }
    }

    void setParamsForDebug(String ssid, String password, String hostname)
    {
      _pConfiguration->SsidSet(ssid);
      _pConfiguration->PasswordSet(password);
      _pConfiguration->HostNameSet(hostname);
      _wifiState = STATE_STARTING_STA;
    }

    void setWirelessParametersAndRestart(String ssid, String password, bool provisionOtherNodes)
    {
      Serial.println("setWirelessParametersAndRestart");
      
      _pConfiguration->SsidSet(ssid);
      _pConfiguration->PasswordSet(password);
      _provisionOtherNodes = provisionOtherNodes;
      _wifiState = STATE_STARTING_STA;
    }

    void PrintStatus()
    {
        switch (WiFi.status())
        {
          case WL_CONNECTED:
            Serial.println("WL_CONNECTED");
            break;
          case WL_NO_SHIELD:
            Serial.println("WL_NO_SHIELD");
            break;
          case WL_IDLE_STATUS:
            Serial.println("WL_IDLE_STATUS");
            break;
          case WL_NO_SSID_AVAIL:
            Serial.println("WL_NO_SSID_AVAIL");
            break;
          case WL_SCAN_COMPLETED:
            Serial.println("WL_SCAN_COMPLETED");
            break;
          case WL_CONNECT_FAILED:
            Serial.println("WL_CONNECT_FAILED");
            break;
          case WL_CONNECTION_LOST:
            Serial.println("WL_CONNECTION_LOST");
            break;
          case WL_DISCONNECTED:
            Serial.println("WL_DISCONNECTED");
            break;
        }
    }

    void startAsClient()
    {
      if (_pDnsServer)
      {
        _pDnsServer->stop();
        _pDnsServer = NULL;
      }
      
      Serial.println("> SAC");
      Serial.println("  SAC: Disconnecting, waiting to finish");
      //WiFi.mode(WIFI_OFF);
      WiFi.disconnect();
      while (WiFi.status() != WL_DISCONNECTED && WiFi.status() != WL_IDLE_STATUS)
      {
        Serial.print("  ");
        Serial.println(WiFi.status());
        delay(50);
      }

      Serial.println("  starting STA");
      Serial.print("  SAC: Connecting to: ");
      Serial.print(_pConfiguration->_ssid);
      Serial.print(" ");
      Serial.println(_pConfiguration->_password);
      //Serial.print(WiFi.status());
      
      WiFi.mode(WIFI_STA);
      Serial.print("  ");
      while (WiFi.getMode() != WIFI_STA)
      {
        Serial.print("M");
        delay(50);
      }
      Serial.println();
      WiFi.disconnect();
      Serial.print("  ");
      Serial.println(_pConfiguration->HostNameGet());
      WiFi.hostname(_pConfiguration->HostNameGet());
      WiFi.begin(_pConfiguration->_ssid, _pConfiguration->_password);

      int timeout = 30;
      Serial.print("  ");
      while (WiFi.status() != WL_CONNECTED && timeout > 0)
      {
        Serial.print(".");
        delay(500);
 
        //PrintStatus();
        //Serial.print(WiFi.status());
        timeout--;
      }
      Serial.println();

      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.print("  Connected: ");
        Serial.println(WiFi.localIP());

        _wifiState = STATE_STA;
        if (_saveAfterProvisioning)
        {
          _pConfiguration->Save();
        }

        UdpLogger.println("Connected");
      }
      else
      {
        Serial.print("  Connect failed, status = ");
        Serial.println(WiFi.status());
        _wifiState = STATE_UNKNOWN;
      }

      Serial.println("< SAC");
    }

    void startAsAccessPoint() {
      generateRandomName();
      
      Serial.println("> SAAP");
      Serial.print("  SAAP: Setting soft-AP:  ");
      Serial.println(_pConfiguration->_hostName);
      WiFi.mode(WIFI_AP);

      WiFi.softAPConfig(_apIP, _apIP, _netMask);
      
      boolean result = WiFi.softAP(_pConfiguration->_hostName, "12345678");
      if(result == true)
      {
        Serial.println("  SAAP: Ready");
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("  SAAP: AP IP address: ");
        Serial.println(myIP);
        Serial.println("  HTTP server started");
        _wifiState = STATE_AP;

        Serial.println("  setting up DNS server...");
        _pDnsServer = new DNSServer();
        _pDnsServer->setErrorReplyCode(DNSReplyCode::NoError);        
        bool result = _pDnsServer->start(DNS_PORT, "*", _apIP);
        Serial.print("  "); Serial.println(result);
      }
      else
      {
        Serial.println("  Failed!");
      }
      Serial.println("< SAAP");
    }  

    String HostNameGet()
    {
      return _pConfiguration->HostNameGet();
    }

    void sendPrivisioningMessage()
    {
      Serial.println(  "Telling server to provision");
      HTTPClient client;

      client.begin("http://192.168.4.1/provision");
      client.addHeader("Content-Type", "text/plain");

      String payload = String("ssid=") + _pConfiguration->_ssid +
                       "&password=" + _pConfiguration->_password +
                       "&SkipOtherNodes=1";
      Serial.print("  ");
      Serial.println(payload);
      int httpCode = client.POST((uint8_t *) payload.c_str(), payload.length());
      Serial.println(httpCode);
    }

    void connectToClientAndProvision(String ssid)
    {
      WiFi.persistent(false);
      WiFi.mode(WIFI_OFF);  // workaround?
      WiFi.mode(WIFI_STA);
      delay(100);

      Serial.print("  Connecting to: ");
      Serial.print(ssid.c_str());
      Serial.print("... ");
      WiFi.begin(ssid.c_str(), "12345678");

      int timeout = 20;
      while (WiFi.status() != WL_CONNECTED && timeout > 0)
      {
        delay(500);
        Serial.print(WiFi.status());
        timeout--;
      }

      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println(  "Connected");

        sendPrivisioningMessage();
      }
  
      WiFi.disconnect();
    }

    void provisionOtherNodesWithCredentials()
    {
      Serial.println("> Provision other nodes");
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      delay(100);

      Serial.println(  "Scanning for networks");

      int n = WiFi.scanNetworks();
      
      for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        if (ssid.startsWith("EDP_"))
        {   
          connectToClientAndProvision(ssid);
        }
      }
      Serial.println("< Provision other nodes");
    }

    Networks getNetworks()
    {
      return _networks;
    }

    void loadNetworks()
    {
      int n = WiFi.scanNetworks();
      Serial.print("Found networks: ");
      Serial.println(n);

      _networks.count = n * 2;
      _networks.pValues = new String[_networks.count];
      
      for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        if (!ssid.startsWith("EDP_"))
        {   
          _networks.pValues[i * 2] = ssid;

          long rssi = WiFi.RSSI(i);
          int percentage = 2 * (rssi + 100);
          if (percentage > 100)
          {
            percentage = 100;
          }
          else if (percentage < 0)
          {
            percentage = 0;
          }

          char number[10];
          itoa(percentage, number, 10);

          String display = ssid;
          display += " (";
          display += percentage;
          display += "%)";

          _networks.pValues[i * 2 + 1] = display;
        }
      }
    }

    void generateRandomName()
    {
      Serial.println("generateRandonName");
       char buf[16];
      ltoa(ESP8266TrueRandom.random(), buf, 10);
      String hostName = String("EDP_") + buf;
      
      _pConfiguration->HostNameSet(hostName);
    }

    void Init(void (*f)(int, void*), void* userParam) 
    {
      if (_wifiState != _lastWifiState)
      {
        Serial.print("handleServerTasks, state = ");
        Serial.println(_wifiState);
        _lastWifiState = _wifiState;
        f(_wifiState, userParam);
      }
      
        // if we aren't in station mode and we have an SSID, try to start up in station mode:
      if (_wifiState == STATE_STARTING_STA && _pConfiguration->SsidGet().length() != 0)
      {
        startAsClient();

        if (_wifiState == STATE_STA && _provisionOtherNodes)
        {
          provisionOtherNodesWithCredentials();
          startAsClient();
        }
      }

      if (_wifiState == STATE_UNKNOWN)
      {
        startAsAccessPoint();        
      }

      if (_pDnsServer)
      {
        _pDnsServer->processNextRequest();
      }
    }    
};

