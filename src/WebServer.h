#include <ESPAsyncWebServer.h>

class WebServer
{
private:
    const int BufferSize = 1024;

    static WebServer *_pWebServer;

    AsyncWebServer *_pServer;
    TargetBrightnesses *_pTargetBrightnesses;
    String _colorTableHtml;
    char *_pBuffer;

    static void OnNotFound(AsyncWebServerRequest *pRequest)
    {
        Serial.println("not found");
        Serial.println(pRequest->url().c_str());

        // pRequest->send(200, "text/html", "<html><body><html>Not found</html></body></html>");
        pRequest->redirect("http://192.168.1.1/");
    }

    static char *GenerateMainPage()
    {
        snprintf(_pWebServer->_pBuffer, _pWebServer->BufferSize,

                 "<html xmlns = \"http://www.w3.org/1999/xhtml\">"
                 "<head>"
                 "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"\
                 "<title> Arrow Updater</title>"
                 "<style type = \"text/css\">"
                 ".auto - style1 { font - size : x - large;}"
                 ".auto - style2 { font - size : larger; }"
                 "</style>"
                 "</head>"
                 "<body>"
                 "<h1> Arrow Updater</h1>"
                 "<form action = \"\" method = \"post\">"
                 "<span class = \"auto-style1\"> Blue: %4.2f</span><br />"
                 "<input name = \"BluePlus\" type = \"submit\" value = \"+\" class = \"auto-style2\" /> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                 "<input name = \"BlueMinus\" type = \"submit\" value = \"-\" class = \"auto-style2\" /><br /><br />"
                 "<span class = \"auto-style1\"> Highlight: %4.2f </span><br />"
                 "<input name = \"HighlightPlus\" type = \"submit\" value = \"+\" class = \"auto-style2\" /><span class = \"auto-style2\"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                 "</span><input name=\"HighlightMinus\" type=\"submit\" value=\"-\" class=\"auto-style2\" />"

                 "</form></body></html>",
                 _pWebServer->_pTargetBrightnesses->Blue.Value,
                 _pWebServer->_pTargetBrightnesses->Highlight.Value);

        return _pWebServer->_pBuffer;
    }

    static void OnMainPage(AsyncWebServerRequest *pRequest)
    {
        Serial.println("> MainPage <");

        bool redirect = false;
        if (pRequest->hasArg("BluePlus"))
        {
            Serial.println("BluePlus");
            _pWebServer->_pTargetBrightnesses->Blue.Increase();
            redirect = true;
        }
        if (pRequest->hasArg("BlueMinus"))
        {
            Serial.println("BlueMinus");
            _pWebServer->_pTargetBrightnesses->Blue.Decrease();
            redirect = true;
        }
        if (pRequest->hasArg("HighlightPlus"))
        {
            Serial.println("HighlightPlus");
            _pWebServer->_pTargetBrightnesses->Highlight.Increase();
            redirect = true;
        }
        if (pRequest->hasArg("HighlightMinus"))
        {
            Serial.println("HighlightMinus");
            _pWebServer->_pTargetBrightnesses->Highlight.Decrease();
            redirect = true;
        }

        if (redirect)
        {
            pRequest->redirect("http://192.168.1.1/");
            return;
        }

        pRequest->send(200, "text/html", GenerateMainPage());
    }


public:
    void Init(TargetBrightnesses* pTargetBrightnesses)
    {
        _pTargetBrightnesses = pTargetBrightnesses;

        _pBuffer = new char[BufferSize];

        _pWebServer = this;

        _pServer = new AsyncWebServer(80);

        _pServer->onNotFound(OnNotFound);
        _pServer->on("/", OnMainPage);

        _pServer->begin();
    }
};

WebServer *WebServer::_pWebServer = 0;
