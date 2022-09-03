#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncDNSServer.h>

#include "Task.h"
#include <NeoPixelBus.h>

extern "C"
{
#include "user_interface.h"
}

#include "TargetBrightnesses.h"
#include <WebServer.h>

#include "RgbColorReal.h"
#include "Fader.h"
#include "FaderGroup.h"
#include "Strip.h"
#include "taskProcessMessages.h"

LEDStrip *pStrip;

const char *Version = "1.3";

TaskManager taskManager;

TargetBrightnesses targetBrightnesses;

FaderGroup* pFaderGroup;

TaskProcessMessages* pTaskProcessMessages;

WebServer* pWebServer;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
AsyncDNSServer* pDnsServer;

void setup()
{
  Serial.begin(115200);
  //Serial.begin(74880);
  Serial.println();
  Serial.println("Arrow controller ");

  const char *ssid = "Arrow01";
  const char *password = "ArrowArrow";

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));


  if (!WiFi.softAP(ssid, password))
  {
    Serial.println("WiFi.softAP failed");
  }

  pDnsServer = new AsyncDNSServer();

  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  pDnsServer->setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is AsyncDNSReplyCode::NonExistentDomain
  pDnsServer->setErrorReplyCode(AsyncDNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  pDnsServer->start(DNS_PORT, "*", apIP);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  pWebServer = new WebServer();
  pWebServer->Init(&targetBrightnesses);

  // Set web server port number to 80
  //pWifiServer = new WifiServer(80);

  pStrip = new LEDStrip(2, 50);

  // Serial.setDebugOutput(true);
  pStrip->Init();
  pStrip->Clear();
  pStrip->SetPixelColor(0, RgbColor(0, 255, 0));
  pStrip->Show();
  //delay(250);

#if !fred
  targetBrightnesses.Load();
  pFaderGroup = new FaderGroup();
  pFaderGroup->Init(5);
  pFaderGroup->Add(new Fader(0.5, &targetBrightnesses.Black, &targetBrightnesses.Black, &targetBrightnesses.Blue, 
                                  &targetBrightnesses.Highlight, &targetBrightnesses.Highlight, &targetBrightnesses.Black)); // blue to yellow
  pFaderGroup->Add(new Fader(0.5, &targetBrightnesses.Highlight, &targetBrightnesses.Highlight, &targetBrightnesses.Black, 
                                  &targetBrightnesses.Highlight, &targetBrightnesses.Highlight, &targetBrightnesses.Black)); // yellow
  pFaderGroup->Add(new Fader(3,   &targetBrightnesses.Highlight, &targetBrightnesses.Highlight, &targetBrightnesses.Black, 
                                  &targetBrightnesses.Highlight, &targetBrightnesses.Black, &targetBrightnesses.Black));     // yellow to red
  pFaderGroup->Add(new Fader(2,   &targetBrightnesses.Highlight, &targetBrightnesses.Black, &targetBrightnesses.Black, 
                                  &targetBrightnesses.Black, &targetBrightnesses.Black, &targetBrightnesses.Blue));          // red to blue
  pFaderGroup->Add(new Fader(30,  &targetBrightnesses.Black, &targetBrightnesses.Black, &targetBrightnesses.Blue, 
                                  &targetBrightnesses.Black, &targetBrightnesses.Black, &targetBrightnesses.Blue));          // blue
#endif

  Serial.println("Faders added");
  pStrip->SetPixelColor(0, RgbColor(0, 255, 255));
  pStrip->Show();
  //delay(250);


  pTaskProcessMessages = new TaskProcessMessages(MsToTaskTime(10), pStrip, pFaderGroup);
  pTaskProcessMessages->Init();

  Serial.println("taskprocessmessages.init()");
  pStrip->SetPixelColor(0, RgbColor(255, 255, 0));
  pStrip->Show();
  //delay(250);

  Serial.println("Starting Task Manager");
  taskManager.StartTask(pTaskProcessMessages);
  pStrip->SetPixelColor(0, RgbColor(0, 0, 255));
  pStrip->Show();
  //delay(250);
}

void loop()
{
  if (targetBrightnesses.GetUpdated())
  {
    targetBrightnesses.Save();
  }

  taskManager.Loop();
}
