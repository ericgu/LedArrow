

class ArrowColor
{
    const static int SequenceLength = 16;

    //const static float YellowStartEnd = 0.5;
    //const static float YellowStaticEnd = 1.0;
    //const static float YellowRedBlendEnd = 4.0;

#if fred
    RgbColorReal GetColor(float value)
    {
        while (value > SequenceLength)
        {
            value -= SequenceLength;
        }

        if (value < YellowStartEnd) // blend from blue into yellow
        {
            return RgbColorReal(value * 2, value * 2, 1 - value * 2);
        }

        if (value < YellowStaticEnd)
        {
            return RgbColorReal(1, 1, 0);
        }

        if (value < YellowRedBlendEnd)
        {
            return RgbColorReal(1, 1 - value / 4, 0);
        }


         
         elseif(value < 6) return
        {
            (6 - value) / 2, 0, (value - (6 - 2)) / 2
        }
        else return
        {
            0, 0, 1
        }

        return RgbColorReal(1, 2, 3);
    }



    if (value > 16)
        value = value - 16 endif

                if (value < 0.5) return {value * 2, value * 2, 1 - value * 2} elseif(value < 1) return {1, 1, 0} elseif(value < 4) return {1, 1 - value / 4, 0} elseif(value < 6) return
        {
            (6 - value) / 2, 0, (value - (6 - 2)) / 2
        }
    else
    return {0, 0, 1}
  endif
  
endfunc

time = 2

Pl()
for loop 0:16:0.1
  //P(loop)
  //P(" ")
  
  color = GetColor(loop + 10)
  D(time, 0, color)
  D(time, 1, color)
  D(time, 23, color)
  
  for s 4:9
    color = GetColor(loop + s)
    D(time, 11 - s, color)
    D(time, 13 + s, color)
  endfor
  
  color = GetColor(loop + 3)
  D(time, 8, color)
  D(time, 9, color)
  D(time, 15, color)
  D(time, 16, color)

  color = GetColor(loop + 2)
  D(time, 10, color)
  D(time, 14, color)

  color = GetColor(loop + 1)
  D(time, 11, color)
  D(time, 13, color)

  color = GetColor(loop)
  D(time, 12, color)


  A(time)
  
  //P(v)
  //P(" ")
  //Pl(color)
endfor
#endif
};