
class ColorTable
{
    static String GetHexDigit(int digit)
    {
      static char digitChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

      return String(digitChars[digit]);
    }

    static String GetHexValue(int number)
    {
      return GetHexDigit(number / 16) + GetHexDigit(number % 16);
    }

    static String GetIntValue(int number)
    {
      char buffer[16];
      itoa(number, buffer, 10);
      return buffer;      
    }

    static String GetTableCell(int red, int green, int blue, bool singleMode)
    {
      String cell = "<td bgColor=\"#";
      cell += GetHexValue(red) + GetHexValue(green) + GetHexValue(blue);
      cell += "\"><a style=\"text-decoration: none;\" href=\"/message?c=1&content=rgbx";

      cell += GetIntValue(red);
      cell += ",";
      cell += GetIntValue(green);
      cell += ",";
      cell += GetIntValue(blue);

      cell += ",100";
      if (!singleMode)
      {
        cell += "&period=500";
      }
      cell += "\">&nbsp;&nbsp;&nbsp;&nbsp;</a></td>";

      return cell;
    }

    static String GetTableRow(int intensity, bool singleMode)
    {
      String row;

      row += GetTableCell(intensity, intensity, intensity, singleMode);
      row += GetTableCell(0, 0, 0, singleMode);
      row += GetTableCell(intensity, 0, 0, singleMode);
      row += GetTableCell(0, intensity, 0, singleMode);
      row += GetTableCell(0, 0, intensity, singleMode);
      row += GetTableCell(intensity, intensity, 0, singleMode);
      row += GetTableCell(intensity, 0, intensity, singleMode);
      row += GetTableCell(0, intensity, intensity, singleMode);
      
      return row;
    }

public:
    static String GetTable(bool singleMode)
    {
      String response;
      
      response += "<table>";

      for (int intensity = 255; intensity > 100; intensity -= 50)
      {
        response += "<tr>";

        response += GetTableRow(intensity, singleMode);

        response += "</tr>";
      }
            
      response += "</table>";

      return response;
    }  
};

