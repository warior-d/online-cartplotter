#include <SoftwareSerial.h>

SoftwareSerial ss(4, 5);

float GLOBAL_DEPTH;

const char *coord[] = {
"55.069531,38.821459",
"55.069697,38.820460",
"55.069854,38.819533",
"55.069979,38.818661",
"55.069958,38.817916",
"55.069729,38.816026",
"55.069572,38.815481",
"55.069166,38.815063",
"55.068884,38.815245",
"55.068676,38.815590",
"55.068551,38.816335",
"55.068509,38.817244",
"55.068530,38.817970",
"55.068561,38.819042",
"55.068738,38.820169",
"55.069291,38.820986"
};


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

  RMC - Рекомендуемый минимум навигационных данных.
  VTG - Скорость и курс относительно земли.

*/
boolean READY_WRITE = false;

#define DEBUG 0


void setup() {
  Serial.begin(9600);
  ss.begin(9600);
}



void loop() {



  Serial.println("$GPGLL,5504.10546,N,03848.80004,E,211606.00,A,A*67");
  delay(1000);
/*
if (DEBUG == 1){
  char cur_coord[19];
  for(int i=0; i < sizeof(coord)/2; i++){
    Serial.println(sizeof(coord[i]));
    delay(1000);
    char buf;
    
    int y = 0;
    int n = 0;
    Serial.println(strlen(coord[i]));
    for(int t = 0; t <= strlen(coord[i]); t++){
      buf = coord[i][t];
      if((buf != '\0') && (buf != ',')){
        cur_coord[y] = buf;
        
        y++;
      }
      else if(buf == ','){
        Serial.println(cur_coord);
        y = 0;
      }
      else if(buf == '\0'){
        Serial.println(cur_coord);
        y = 0;
        
      }
    }

    
  }
  
  //Serial.println("$GPGLL,5504.10546,N,03848.80004,E,211606.00,A,A*67");
  delay(1000);
}
else{*/
  if (ss.available() > 0) {
    
    char c = ss.read();
    PrintString(c);

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
    
    is = strstr(Buffer,strFound);
    if (is > 0){
      /**
       * Печатать будем ТОЛЬКО
       * - GPGLL строку (координаты) - как есть
       * - GPVTG строку (курс + скорость) - подставляем курс, скорость не трогаем
       * - SDDBT строку сотворяем заново сами :)
       */

      
      Serial.println(Buffer);
      float dist_cm = 7;
      byte base = random(0,9);
      float all_depth = base + dist_cm/10;
      //Serial.println("$SDDBT,64.6,f,19.09,M,10.77,F*32");
      Serial.print(beginStr); Serial.print(all_depth,2);Serial.println(endStr);
    }
  }
  
}
