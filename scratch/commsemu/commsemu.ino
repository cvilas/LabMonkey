/* Serial comms emulator for LabMonkey */

/*
  command  response
  EN  OK
  DI  OK
  OST  (string to int)0x10000 (check OST output)
  V   OK
  SP  OK
  AC  OK
  DEC  OK
  M    OK
  LR  OK
  HO  OK
  LA  OK
  POS  (string to int) POS
  DELAY  OK
  PROGSEQ OK
  ENDPROG  OK
*/
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // 1. wait for message
  // 2. Read message
  // 3. Remove first character (node number)
  // 4.  if OST reply with ??
  //      - else if POS reply with a number
  //      - else reply with OK\r\n
  char buf[100]; buf[0] = '\0';
  static int n = 0;
  int bytesAvailable = Serial.readBytes(buf, 100);
  if( bytesAvailable > 0 )
  {
    buf[5] = '\0';
    
    if( buf[1] == 'O' && buf[2] == 'S')
    {
      Serial.println(0x10000,DEC);
    }
    else if( buf[1] == 'P' && buf[2] == 'O')
    {
      n++;
      Serial.println(n,DEC);
    }
    else
    {
      Serial.println("OK");
    }
    //Serial.write("OK\r\n");
  }
  
}
