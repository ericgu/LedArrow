class TargetBrightness
{
public:
    float Value;
    bool Updated;

    void Increase()
    {
        Value = Value + 0.05F;
        if (Value > 1.0F)
        {
            Value = 1.0F;
        }
        Updated = true;
    }

    void Decrease()
    {
        Value = Value - 0.05F;
        if (Value < 0.0F)
        {
            Value = 0.0F;
        }
        Updated = true;
    }

    TargetBrightness()
    {
        Updated = false;
    }

    TargetBrightness(float value)
    {
        Value = value;
        Updated = false;
    }
};