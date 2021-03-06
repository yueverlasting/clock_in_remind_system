/*
 * 注意 NodeMCU 開發板上LED 預設 low為亮燈
 */
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "pitch.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

//定義wifi
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 28800);
const char *ssid     = "TVTC-MOA";
const char *password = "22592566";

//定義PIR
const int PIRout= D4;
int i =0;

// 定義音樂
int duration[4] = {200,200,200,400};  // 500 miliseconds
int note_DOWN   [4] ={NOTE_C5, NOTE_G5,NOTE_C6,NOTE_C5};
int note_UP     [4] ={NOTE_C5, NOTE_F5,NOTE_G5,NOTE_C6};

//時間計算
int seconds = 0,minutes = 0,hours = 0,days = 0;
String YEAR,HOUR,MIN,SEC;

unsigned long timeNow = 0;
unsigned long timeLast = 0;
uint32_t  SECOND ;
uint32_t NOW, NOW_HOUR ,NOW_MIN, NOW_SEC ;

////副函式
void run_time(){
  if(timeLast != 0){
    NOW = (SECOND +  millis()/1000) %86400 ;
    NOW_HOUR = NOW /3600          ; hours   = NOW_HOUR;
    NOW_MIN  = ((NOW %3600 )/60)  ; minutes = NOW_MIN ;
    NOW_SEC  = NOW % 60           ; seconds = NOW_SEC ;
  }
  timeLast = timeNow;
  SECOND = ( (hours*3600 + minutes*60 +seconds) - millis()/1000 );
}

void set_Date(){
  lcd.clear();
  lcd.setCursor( 0,0 );
  lcd.print( "SET TIME!" );
  
  timeClient.update();
  YEAR  = timeClient.getYear() ;
  HOUR  = timeClient.getHours();    hours = HOUR.toInt();
  MIN   = timeClient.getMinutes();  minutes = MIN.toInt();
  SEC   = timeClient.getSeconds();  seconds = SEC.toInt();
  SECOND = ( (hours*3600 + minutes*60 +seconds) - millis()/1000 );
  if(YEAR == "1970") {set_Date();} 
}

void print_Date(){
    //print the date EG  23:59:59
    Serial.print("     Time : ");
    if(hours <10)   {Serial.print("0");Serial.print(hours);}    else{Serial.print(hours);}    Serial.print(":");
    if(minutes <10) {Serial.print("0");Serial.print(minutes);}  else{Serial.print(minutes);}  Serial.print(":");
    if(seconds <10) {Serial.print("0");Serial.println(seconds);}else{Serial.println(seconds);}
    
    lcd.setCursor ( 0,1 ); lcd.print( "Time : " ); 
    if(hours   <10) {lcd.print("0");lcd.print( hours );} else {lcd.print(hours);}
    lcd.print(":");
    if(minutes <10) {lcd.print("0");lcd.print(minutes);} else {lcd.print(minutes);}
    lcd.print(":");
    if(seconds <10) {lcd.print("0");lcd.print(seconds);} else {lcd.print(seconds);} 
}

void play(){
    for (int thisNote = 0; thisNote < 4; thisNote++) {
    // 在 pinD5 上輸出聲音，每個音階響 0.5 秒
    if( (hours >= 6 && hours <= 9))
    {tone(D5, note_UP[thisNote], duration[thisNote]);}
    if( (hours >= 17 && hours <= 23) )
    {tone(D5, note_DOWN[thisNote], duration[thisNote]);}
    // 間隔一段時間後再播放下一個音階
    delay(200);
    }
    delay(10000);
}

void setup(){
    /*WIFI部分*/
  lcd.begin();
  lcd.backlight();
  lcd.print("Hello !");
  
  Serial.begin(57600);
  Serial.setTimeout(5000);
  
  pinMode(PIRout,INPUT);
  Serial.println("");
  
  WiFi.begin(ssid, password);  //wifi開始連接
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  
  Serial.println ( "START CONNECT" );
  lcd.setCursor( 0,0 );
  lcd.print( "START CONNECT" );
  timeClient.begin();
  delay(1000);
  set_Date();
  lcd.clear();
  lcd.setCursor( 0,0 );
  lcd.print( "Running--" );
}

void loop() {
  timeNow = millis()/1000 ;// millis is the function i have mentioned above
  if(timeNow != timeLast) {run_time();}

  if( hours == 1 && (minutes == 1 || minutes == 2) ) {
    Serial.println("set_time");
    set_Date();
    delay(1000*10);
  }
  else{
      while( (hours >= 6 && hours <= 9) || (hours >= 17 && hours <= 23)){
          lcd.clear(); lcd.setCursor( 0,0 );
          lcd.print( "Ring" );
          
          int val=digitalRead(PIRout);  //讀取 PIR 輸出
          if (val == HIGH) {   //PIR 有偵測到時
          play();
          Serial.println ( digitalRead(PIRout) ) ;
          delay(1000);} 
          break;
        }   
  print_Date();
  Serial.println("");
  delay(1000);
  }
}
