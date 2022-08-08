#include "Tokenizer.h"

class AnimationCommands
{
  private:
  
//    int _currentCommand;
//    int _commandCount;
    int _cycle;
    Tokenizer _tokenizer;

  public:
    AnimationCommands() :
      _tokenizer('$', 128)
    {}

    void SetAnimation(const char* pAnimation)
    {
      if (*pAnimation == '$')
      {
        _tokenizer.SetString(pAnimation);
      }
      _cycle = -1;
    }

    Command GetNextCommand()
    {
        if (_tokenizer.GetFinished())
        {
          _tokenizer.Reset();
        }

        char* pNumber = _tokenizer.GetToken();
        int period = atoi(pNumber);
        char* pMessage =  _tokenizer.GetToken();
        String message = pMessage;

        return Command(message, period);
    }

    Command GetNextMessage()
    {
      //Serial.println("GetNextMessage");
      //Serial.println(_cycle);
      //Serial.println(_currentCommand);

      if (_cycle <= 0)
      {
        Command command = GetNextCommand();

        _cycle = command._period;
        Serial.print(command._message); Serial.print(" "); Serial.println(command._period);
        //cout << command._message << " " << command._period << endl;
        return command;
      }
      _cycle--;
        
      //cout << "0, -1" << endl;
      //Serial.println("GetNextMessage");

      return Command(0, -1);
    }
};

