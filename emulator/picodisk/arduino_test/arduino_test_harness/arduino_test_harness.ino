const int _datapins[] =  {22, 24, 26, 28, 30, 32, 34, 36};
const int _addrpins[] =  {39, 41, 43, 45};
// 47, 49, 51, 53
const int _selectDrive = 47;
const int _selectCom = 49;
const int _readWrite = 51;
const int _phi2 = 53;

const int _slow = 23;

bool _readMode = false;
bool _clockHigh = false;
char _buf[255];
uint8_t _addr = 0;
uint16_t _count = 0;

void SetReadWrite(bool read)
{
  _readMode = read;
  digitalWrite(_readWrite, read);

  for(int i = 0; i < 8; i++)
  {
    pinMode(_datapins[i], read ? INPUT : OUTPUT);
  } 
}

void WriteData(uint8_t data)
{
  for(int i = 0; i < 8; i++)
  {
    bool bitset = ((data >> i) & 1) == 1;
    digitalWrite(_datapins[i], bitset);
  }
}

void WriteAddress(uint8_t addr)
{
    for(int i = 0; i < 4; i++)
    {
      bool bitset = ((addr >> i) & 1) == 1;
      digitalWrite(_addrpins[i], bitset);
    }
}

uint8_t ReadData()
{
  uint8_t val = 0;
  if (_readMode)
  {
    for(int i = 0; i < 8; i++)
    {
      bool isSet = digitalRead(_datapins[i]);
      if (isSet)
      {
        val |= ((isSet ? 1 : 0) << i);
      }
    }
  }

  return val;
}

void setup() 
{
  Serial.begin(9600);
  Serial.println("setting up");
  // put your setup code here, to run once:
  for(int i = 0; i < 4; i++)
  {
    pinMode(_addrpins[i], OUTPUT);
  }

  pinMode(_selectDrive, OUTPUT);
  pinMode(_selectCom, OUTPUT);
  pinMode(_readWrite, OUTPUT);
  pinMode(_phi2, OUTPUT);
  pinMode(_slow, OUTPUT);

  digitalWrite(_selectCom, false);
  digitalWrite(_selectDrive, true);

  SetReadWrite(true);
}

void loopbacktest()
{
    while(true)
    {
      delay(2);

      if (_clockHigh)
      {
        uint8_t d = ReadData();
        sprintf(_buf,"%d: data=%02x", _count++, d);
        Serial.println(_buf);
      }
      else
      {
        WriteAddress(_count & 0xFF);
      }

      _clockHigh = !_clockHigh;
      digitalWrite(_phi2, _clockHigh);

      delay(250);
    }
}

void testconsole()
{
  bool bWrite = false;
  SetReadWrite(true);
  WriteAddress(0);
  
  while(true)
  {
    if (!_clockHigh)
    {
      //delay(1);
      SetReadWrite(true);
      bWrite = false;
      if(Serial.available() > 0)
      {
        int b = Serial.read();
        if (b != -1)
        {
          SetReadWrite(false);
          WriteData(b);
          Serial.print((char)b);
          bWrite = true;
        }
      }
    }
    else  
    {
      if (!bWrite)
      {
        //delay(1);
        uint8_t c = ReadData();

        if(c != 0)
        {
          sprintf(_buf, "%c", c);
          Serial.print(_buf);
        }
      }
    }
    
    _clockHigh = !_clockHigh;
    digitalWrite(_phi2, _clockHigh);

  }

}

void testread()
{
    delay(1000);
    SetReadWrite(true);
    while(true)
    {
      if (_clockHigh)
      {
        uint8_t d = ReadData();
        sprintf(_buf,"%d: data=%02x", _count++, d);
        Serial.println(_buf);
      }
      else
      {
        WriteAddress(0xF);
        delay(7);
      }


      _clockHigh = !_clockHigh;
      digitalWrite(_phi2, _clockHigh);

    }
}

void testwrite()
{
      delay(1000);

    SetReadWrite(false);
    WriteAddress(0x0);
    WriteData(_count++);

    while(true)
    {
      if (!_clockHigh)
      {
        WriteData(_count++);
        delay(1000);
      }
      else
      {
        delay(1000);
      }

    
      _clockHigh = !_clockHigh;
      digitalWrite(_phi2, _clockHigh);
    }
}
void loop() {
  // put your main code here, to run repeatedly:
  // testwrite();
  // testread();
  testconsole();
}
