#include <SoftwareSerial.h>

SoftwareSerial ss(4, 5);

float GLOBAL_DEPTH;

/*
  char* myStrings[]={
  "$GPGLL,5540.91588,N,03752.72131,E,211606.00,A,A*67",
  "$SDDBT,,,19.06,M,10.77,F*3D",
  "$TIME0,C1*7B"};
*/


/**
 * Описание протокола
 * https://wiki.iarduino.ru/page/NMEA-0183/
 * 
   $SDDBT,64.6,f,19.06,M,10.77,F*3D
   $TIME0,C1*7B
   Можно и так:
   $SDDBT,,,19.06,M,10.77,F*3D

  1. $ - символ начала пакета (1 байт)
  2. SDDPT — команда (5 байт)
  3. x.x - глубина в метрах, в дробном десятичном ASCII формате
  4. x.x - расстояние от передатчика до ватерлинии, в дробном
  десятичном ASCII формате
  5. * - разделитель контрольной суммы
  6. hh - контрольная сумма (2 байта)
  7. <CR><LF> - символы конца пакета (2 байта)


  НАСТРОЙКИ в DrDepth!

  COM5
  9600

  NMEA Sentence:
  GPS:      GPGLL
  Sounder:  SDDBT

  !!! Ignore NMEA cheksum

  Course vector

*/
boolean READY_WRITE = false;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
}



void loop() {

  if (ss.available() > 0) {
    char c = ss.read();
    PrintString(c);
    //Serial.write(c);
  }
  
}

void getDepth()
{
  int base = random(1,10);
  GLOBAL_DEPTH = base + 0.21;
}

void PrintString(char c)
{
  static int pos;
  static char Buffer[120];
  char strFound[10]="GPGLL";
  char* is;
  //char beginStr[20] = "$SDDBT,,,";
  char beginStr[20] = "$SDDBT,64.6,f,";
  char endStr[20] = ",M,10.77,F*32";

  
  if(pos>=120){
    pos = 0;
    READY_WRITE == true;
  }
  
  if(c == '$' && READY_WRITE == false){
    pos = 0;
    Buffer[pos] = c;
    READY_WRITE = true;
  }
  
  if(c != '$' && READY_WRITE == true && c != '\r' && c != '\n'){
    pos = pos + 1;
    Buffer[pos] = c;
  }
 
  if((c == '\r' || c == '\n')&&READY_WRITE==true){
    READY_WRITE = false;
    pos = pos + 1;
    Buffer[pos] = '\0';
    Serial.println(Buffer);
    is = strstr(Buffer,strFound);
    if (is > 0){
      float dist_cm = 7;
      byte base = random(0,9);
      float all_depth = base + dist_cm/10;
      //Serial.println("$SDDBT,64.6,f,19.09,M,10.77,F*32");
      Serial.print(beginStr); Serial.print(all_depth,2);Serial.println(endStr);
    }
  }
  
}
