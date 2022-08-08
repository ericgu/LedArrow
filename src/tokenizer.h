class Tokenizer
{
  String _string;
  int _start;
  char _separator;
  bool _finished;
  const char* _pString;
  const char* _pStart;
  char* _pBuffer;
  int _maxLength;
  
public:
  Tokenizer(char separator, int maxLength)
  {
    _separator = separator;
    _pBuffer = new char[maxLength + 1];
    _maxLength = maxLength;
  }

  void SetString(const char* pString)
  {
    //Serial.print("SetString: "); Serial.println(pString);
    _pString = pString;
    _pStart = pString;
    _finished = false;
  }

  char* GetToken()
  {
    //Serial.print("GetToken1: "); Serial.println(_pString); Serial.flush();

    const char* pEnd = _pStart;
    if (*pEnd == '$')
    {
      pEnd++;
    }

    //Serial.print("GetToken2: "); Serial.println(pEnd); Serial.flush();

    while (*pEnd != '$' && *pEnd != '\0')
    {
      pEnd++;
    }

    //Serial.print("GetToken3: "); Serial.println(pEnd); Serial.flush();
    if (pEnd == _pStart)
    {
      *_pBuffer = '\0';
    }
    else
    {
      int length = pEnd - _pStart - 1;
      //Serial.println(length);
      if (length < _maxLength)
      {
        //Serial.println("strncpy1");
        strncpy(_pBuffer, _pStart + 1, length);
        *(_pBuffer + length) = '\0';
        //Serial.println("strncpy2");
      }
      else
      {
        *_pBuffer = '\0';
      }
    }

    if (*pEnd == '\0')
    {
      _finished = true;
    }
    _pStart = pEnd;

    //Serial.print("GetToken4: "); Serial.println(_pBuffer); Serial.flush();

    return _pBuffer;
  }

  bool GetFinished()
  {
    return _finished;
  }

  void Reset()
  {
    _finished = false;
    _pStart = _pString;
  }
};