#include "TargetBrightness.h"
#include <eeprom.h>

class TargetBrightnesses
{
    public:
    char signature[4];
    TargetBrightness Black;
    TargetBrightness Blue;
    TargetBrightness Highlight;

    TargetBrightnesses()
    {
    }

    bool GetUpdated()
    {
        return Black.Updated | Blue.Updated | Highlight.Updated;
    }

    void Load()
    {
        Serial.println("Load");

        EEPROM.begin(4096);

        byte *pStart = (byte *)this;
        byte *pData = pStart;
        int size = sizeof(TargetBrightnesses);
        for (int i = 0; i < size; i++)
        {
            *pData = EEPROM.read(i);
            pData++;
        }

        if (strcmp(signature, "Bri") != 0)
        {
            Serial.println("default");
            Black = TargetBrightness(0);
            Blue = TargetBrightness(1);
            Highlight = TargetBrightness(1);
            strcpy(signature, "Bri");
        }

        Serial.println("Load:");
        Serial.println(Blue.Value);
        Serial.println(Highlight.Value);
    }

    void Save()
    {
        int size = sizeof(TargetBrightnesses);

        EEPROM.begin(256);
        byte *pData = (byte *)this;
        for (int i = 0; i < size; i++)
        {
            EEPROM.write(i, *pData);
            pData++;
        }

        EEPROM.commit();
        Serial.println("Save:");
        Serial.println(Blue.Value);
        Serial.println(Highlight.Value);
        Black.Updated = false;
        Blue.Updated = false;
        Highlight.Updated = false;
    }
};