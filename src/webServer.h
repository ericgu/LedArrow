#include <ESPAsyncWebServer.h>
#include "MainPage.h"
#include "ColorTable.h"

#define CodeVersionNumber "V1.0"

class WebServer
{
  private:
    static WebServer *_pWebServer;
  
    AsyncWebServer* _pServer;
    WifiHandler* _pWifiHandler;
    PixelHandler* _pPixelHandler;
    String _colorTableHtml;

    static void OnNotFound(AsyncWebServerRequest* pRequest)
    {
      Serial.println("not found");
      Serial.println(pRequest->url().c_str());

      //pRequest->send(200, "text/html", "<html><body><html>Not found</html></body></html>");
       pRequest->redirect("http://192.168.4.1/Provision");
    }

    static void OnInfo(AsyncWebServerRequest* pRequest)
    {
      String info = "EagleDecorations ESPClient";
      info += CodeVersionNumber;
      info += " ";
      info +=  _pWebServer->_pWifiHandler->HostNameGet();
      pRequest->send(200, "text/plain", info.c_str());
    }

    static void OnMainPage(AsyncWebServerRequest* pRequest)
    {
      Serial.println("> MainPage <");

      String response = MainPageHtml1;

      response += "<p>";
        
      response += MainPageHtml2;

      response += "Command: <input type=\"text\" name=\"content\"  style=\"width:600px;max-width:600px\" value=\"";
      response += _pWebServer->_pPixelHandler->GetCurrentCommand();
      response += "\" />&nbsp;&nbsp;";

      response += MainPageHtml3;

      //Serial.println(response);

      pRequest->send(200, "text/html", response);

      //Serial.println("< MainPage");
    }
    
    static void OnProvision(AsyncWebServerRequest* pRequest)
    {
      Serial.println("> Provision");

      if (pRequest->hasArg("ssid"))
      {
        String ssid = pRequest->arg("ssid");
        String password = pRequest->arg("password");

        Serial.print("  ssid: "); Serial.println(ssid);
        Serial.print("  password: "); Serial.println(password);
        _pWebServer->_pWifiHandler->setWirelessParametersAndRestart(ssid, password, true);

        pRequest->send(200, "text/html", "<h1>Provisioning...</h1><html>Disconnect from this wireless network and reconnect to your main network.</html>");
      }      
      else
      {
        String response = ProvisionPageHtml1;

        response += "<b>Status: </b>";
        response += _pWebServer->_pWifiHandler->GetStatus();
        response += "</br>";
        
        response += "<b>LEDs: </b>";
        response += _pWebServer->_pPixelHandler->GetPixelCount();
        response += "</br>";
        
        response += ProvisionPageHtml2;

        Networks networks = _pWebServer->_pWifiHandler->getNetworks();

        for (int i = 0; i < networks.count; i += 2)
        {
          String option = "<option value=\"" + networks.pValues[i] + "\">" + networks.pValues[i + 1] + "</option>";
          response += option;
        }

        response += ProvisionPageHtml3;
        pRequest->send(200, "text/html", response);
      }

      Serial.println("< Provision");
    }

    static void OnColorPage(AsyncWebServerRequest* pRequest)
    {
      Serial.println("> ColorPage");

      String response = ColorPageHtml1;

      response += "<p>";

      response += _pWebServer->_colorTableHtml;

      response += ColorPageHtml2;
      pRequest->send(200, "text/html", response);

      Serial.println("< ColorPage");
    }

    static void OnMessage(AsyncWebServerRequest* pRequest)
    {
      if (pRequest->hasArg("r"))
      {
        pRequest->redirect("/");
      }
      if (pRequest->hasArg("c"))
      {
        pRequest->redirect("/Color");
      }      
      else if (pRequest->hasArg("ledcount"))
      {
        pRequest->redirect("/Provision");
      }
      else
      {
        pRequest->send(200, "text/plain", "Accepted");      
      }

      if (pRequest->hasArg("content"))
      {
        String content = pRequest->arg("content");

        if (pRequest->hasArg("ledcount"))
        {
          content = "nx" + content; // prepend command...
        }

        Serial.print("content: "); Serial.println(content);

        _pWebServer->_pPixelHandler->ProcessMessage(content);
        Serial.println("Done with command execution");
      }      
    }

  public:
    void SetHandlers(WifiHandler* pWifiHandler, PixelHandler* pPixelHandler)
    {
      _pWifiHandler = pWifiHandler;  
      _pPixelHandler = pPixelHandler;
    }
    
    void Init()
    {
      _pWebServer = this;
      _colorTableHtml = ColorTable::GetTable(true);
      
      _pServer = new AsyncWebServer(80);

      _pServer->onNotFound(OnNotFound);
      _pServer->on("/", OnMainPage);
      _pServer->on("/Provision", OnProvision);
      _pServer->on("/Color", OnColorPage);
      _pServer->on("/info", OnInfo);
      _pServer->on("/message", OnMessage);

      _pServer->begin();
    }
  
};

WebServer* WebServer::_pWebServer = 0;

