#include <Arduino.h>
#include "Task.h"

//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <WiFiUdp.h>

#include <NeoPixelBus.h>

extern "C"
{
#include "user_interface.h"
}


#include "PersistentStorage.h"

#include "RgbColorReal.h"
#include "Fader.h"
#include "FaderGroup.h"

#include "Strip.h"

#include "taskProcessMessages.h"

#include "ArrowColor.h"


LEDStrip strip(2, 24);

const uint8_t PixelPin = 2; // make sure to set this to the correct pin, ignored for Esp8266
const char *Version = "1.3";

TaskManager taskManager;

FaderGroup faderGroup(5);

TaskProcessMessages taskProcessMessages(LED_BUILTIN, MsToTaskTime(10), &strip, &faderGroup);

void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println("Arrow controller ");
  // Serial.setDebugOutput(true);

  strip.Init();

  faderGroup.Add(new Fader(0.5, 0, 0, 1, 1, 1, 0));   // blue to yellow
  faderGroup.Add(new Fader(0.5, 1, 1, 0, 1, 1, 0));   // yellow
  faderGroup.Add(new Fader(3, 1, 1, 0, 1, 0, 0));     // yellow to red
  faderGroup.Add(new Fader(2, 1, 0, 0, 0, 0, 1));     // red to blue
  faderGroup.Add(new Fader(15, 0, 0, 1, 0, 0, 1));     // blue

  taskProcessMessages.Init();

  Serial.println("Starting Task Manager");
  taskManager.StartTask(&taskProcessMessages);
}

void loop()
{
  taskManager.Loop();
}
