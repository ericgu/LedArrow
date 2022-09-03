class FaderGroup
{
private:
    int _count;
    int _total;
    Fader **_pFaders;
    float _totalDistance;

public:
    void Init(int total)
    {
        _total = total;
        _count = 0;
        _pFaders = new Fader*[_total];
        _totalDistance = 0;
    }

    void Add(Fader* pFader)
    {
        if (_count == _total)
        {
            Serial.println("Fader add at end");
            return;
        }

        _pFaders[_count] = pFader;
        _count++;
        _totalDistance += pFader->GetDistance();
    }

    float GetTotalDistance()
    {
        return _totalDistance;
    }

    RgbColorReal GetCurrent(float value)
    {
        while (value > _totalDistance)
        {
            value = value - _totalDistance;
        }


        for (int i = 0; i < _count; i++)
        {
            Fader *pFader = _pFaders[i];
            float distance = pFader->GetDistance();

            if (value < distance)
            {
                return pFader->GetColor(value);
            }
            value -= distance;
        }

        return RgbColorReal(0, 0, 0);
    }
};