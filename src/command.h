#define MaxValues 16
  
class Command
{
  public:
    String _message;
    int _count;
    int _values[MaxValues];
    const char* _pAfterNumbers;
    int _period;

    static bool IsDigit(const char c)
    {
      return (c >= '0' && c <= '9') || c == '-';
    }

    static bool IsHexDigit(const char c)
    {
      return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
    }
    
    static const char* SkipToFirstDigit(const char* pString)
    {
            // skip to first digit
      while (!IsDigit(*pString) && *pString != '\0')
      {
        pString++;    
      }

      return pString;
    }

    static const char* SkipToFirstHexDigit(const char* pString)
    {
            // skip to first digit
      while (!IsHexDigit(*pString) && *pString != '\0')
      {
        pString++;    
      }

      return pString;
    }

    static const char* SkipToFirstNonDigit(const char* pString)
    {
            // skip to first digit
      while (IsDigit(*pString) && *pString != '\0')
      {
        pString++;    
      }

      return pString;
    }

    Command() 
    {
      _message = "";
      _period = -1;  
    }

    Command(const char* pMessage) :
      _message(pMessage),
      _period(-1) 
      {
        Parse(pMessage);
      }

    Command(const char* pMessage, int period) :
      _message(pMessage),
      _period(period)
    {
      Parse(pMessage);
    }

    Command(String &message, int period) :
      _message(message),
      _period(period)
    {
      Parse(message.c_str());
    }
    
    void Parse(const char* pValues)
    {
      _count = 0;
      if (!pValues)
      {
        return;
      }
      
      pValues = SkipToFirstDigit(pValues);
      
      while (*pValues != '\0')
      {
        if (_count == MaxValues)
        {
          Serial.println("Too many values in Command.Parse()");
          return;
        }
        _values[_count] = atoi(pValues);
        _count++;

        pValues = SkipToFirstNonDigit(pValues);

        const char* pNext = SkipToFirstDigit(pValues);

        if (*pNext == '\0')
        {
          _pAfterNumbers = pValues;
        }

        pValues = pNext;
      }

      _pAfterNumbers = pValues;
    }

    bool StartsWith(String candidate)
    {
      return _message.startsWith(candidate);
    }

    bool IsMatch(const char* pMatch, int numbers)
    {
      return _message.startsWith(pMatch) && _count == numbers;
    }    

    void Dump()
    {
      Serial.println("Numbers");
      for (int i = 0; i < _count; i++)
      {
        Serial.print(i);
        Serial.print(" ");
        Serial.println(_values[i]);
      }
    }
};

