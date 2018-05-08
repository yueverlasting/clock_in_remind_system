#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "pitch.h"
//定義變數
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 28800);

const int PIRout= D2;
const int LED= D6;
int i =0;
// notes in the melody:
int duration[4] = {200,200,200,400};  // 500 miliseconds
int note_DOWN   [4] ={NOTE_C5, NOTE_G5,NOTE_C6,NOTE_C5};
int note_UP     [4] ={NOTE_C5, NOTE_F5,NOTE_G5,NOTE_C6};

unsigned long timeNow = 0;
unsigned long timeLast = 0;
uint32_t  SECOND;

int seconds = 0,minutes = 0,hours = 0,days = 0;

const char *ssid     = "TVTC-MOA";
const char *password = "22592566";
char*  Ntime ;
String YEAR,HOUR,MIN,SEC;

////副函式
void run_time(){
  if(timeLast != 0){
      seconds = seconds + (timeNow - timeLast);
    if (seconds > 59) 
    { seconds -= 60 ;minutes += 1;}
    if (minutes > 59)
    { minutes -= 60 ;hours = hours+1;}
    if (hours == 24)
    {hours = 0;}
  }
  timeLast = timeNow;
  SECOND = ( (hours*3600 + minutes*60 +seconds) - millis()/1000 ) ;
}
void set_Date(){
  //print the date EG   3-1-11 23:59:59
  timeClient.update();
  YEAR  = timeClient.getYear() ;
  HOUR  = timeClient.getHours();hours = HOUR.toInt();
  MIN   = timeClient.getMinutes();minutes = MIN.toInt();
  SEC   = timeClient.getSeconds();seconds = SEC.toInt();
  SECOND = ( (hours*3600 + minutes*60 +seconds) - millis()/1000 ) ;
}

void print_Date(){
    //print the date EG  23:59:59
    Serial.print("     Time : ");
    if(hours <10)   {Serial.print("0");Serial.print(hours);}    else{Serial.print(hours);}    Serial.print(":");
    if(minutes <10) {Serial.print("0");Serial.print(minutes);}  else{Serial.print(minutes);}  Serial.print(":");
    if(seconds <10) {Serial.print("0");Serial.println(seconds);}else{Serial.println(seconds);}
}

void print_time(){
  uint32_t NOW = SECOND +  millis()/1000 ;
  uint32_t NOW_HOUR = NOW /3600 ;
  uint32_t NOW_MIN  = ((NOW %3600 )/60)  ;
  uint32_t NOW_SEC  = NOW % 60 ;
    Serial.print("Uint Time : ");
    if(NOW_HOUR <=9){Serial.print("0");Serial.print(NOW_HOUR);}   else{Serial.print(NOW_HOUR);} Serial.print(":");
    if(NOW_MIN  <=9){Serial.print("0");Serial.print(NOW_MIN);}    else{Serial.print(NOW_MIN);}  Serial.print(":");
    if(NOW_SEC  <=9){Serial.print("0");Serial.println(NOW_SEC);}  else{Serial.println(NOW_SEC);}
}
void play(){
    for (int thisNote = 0; thisNote < 4; thisNote++) {
    // 在 pinD5 上輸出聲音，每個音階響 0.5 秒
    if(HOUR == "06" || HOUR == "07" || HOUR == "08" || HOUR == "09")
    {tone(D5, note_UP[thisNote], duration[thisNote]);}
    if(HOUR == "17" || HOUR == "18" || HOUR == "19" || HOUR == "20" || HOUR == "21" || HOUR == "22" || HOUR == "23")
    {tone(D5, note_DOWN[thisNote], duration[thisNote]);}
    // 間隔一段時間後再播放下一個音階
    delay(200);
    }
    delay(10000);
}

void setup(){
  
    /*WIFI部分*/
  Serial.begin(57600);
  Serial.setTimeout(5000);
  Wire.begin(D4,D3);
  pinMode(PIRout,INPUT);
  pinMode(LED,OUTPUT);
  Serial.println("");
  
  WiFi.begin(ssid, password);  //wifi開始連接
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println ( "START CONNECT" );
  timeClient.begin();
  delay(1000);
  set_Date();
  while( YEAR == "1970") {set_Date();digitalWrite(LED,HIGH);}
  digitalWrite(LED,LOW);
}

void loop() {
  timeNow = millis()/1000 ;// millis is the function i have mentioned above
  
  if(timeNow != timeLast) {run_time();}
  
  if( HOUR == "01" && (MIN == "0" || MIN == "1") ) {
    Serial.println("set_time");
    set_Date();
    digitalWrite(LED,HIGH);
    delay(1000*10);
    digitalWrite(LED,LOW);
  }
  else{
      while(HOUR == "06" || HOUR == "07" || HOUR == "08" || HOUR == "09" || HOUR == "17" || HOUR == "18" || HOUR == "19" || HOUR == "20" || HOUR == "21" || HOUR == "22" || HOUR == "23")
       {
          int val=digitalRead(PIRout);  //讀取 PIR 輸出
          if (val == HIGH) {   //PIR 有偵測到時 : LED 閃一下
          play();
          delay(1000);
          } 
          break;
        }   
  print_time();
  print_Date();
  Serial.println("");
  delay(1000);
  }
}

