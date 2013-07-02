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
  char buf[256];
  char out[20];
  int bytesAvailable = Serial.available();
  if( bytesAvailable > 0 )
  {
    Serial.readBytesUntil('\n', buf, bytesAvailable);
    
    if( buf[1] == 'O' && buf[1] == 'S')
    {
      sprintf(out,"%d\r\n", 0x10000);
    }
    else if( buf[1] == 'P' && buf[2] == 'O')
    {
      sprintf(out, "%d\r\n", 0);
    }
    else
    {
      sprintf(out, "%s\r\n","OK"); 
    }
    
    Serial.write(buf);
  }
  
}
