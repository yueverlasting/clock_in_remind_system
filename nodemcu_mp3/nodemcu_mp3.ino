//#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

#include "viola.h"
#include "viola2.h"

AudioGeneratorMP3 *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2SNoDAC *out;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//定義wifi
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 28800);
const char *ssid     = "TVTC-MOA";
const char *password = "22592566";

//定義PIR
const int PIRout = D5;
int i = 0;

//時間計算
int seconds = 0, minutes = 0, hours = 0, days = 0;
String YEAR, HOUR, MIN, SEC;

unsigned long timeNow = 0;
unsigned long timeLast = 0;
uint32_t  SECOND ;
uint32_t NOW, NOW_HOUR , NOW_MIN, NOW_SEC ;

////副函式
void run_time() {
  if (timeLast != 0) {
    NOW = (SECOND +  millis() / 1000) % 86400 ;
    NOW_HOUR = NOW / 3600             ; hours   = NOW_HOUR;
    NOW_MIN  = ((NOW % 3600 ) / 60)   ; minutes = NOW_MIN ;
    NOW_SEC  = NOW % 60               ; seconds = NOW_SEC ;
  }
  timeLast = timeNow;
  SECOND = ( (hours * 3600 + minutes * 60 + seconds) - millis() / 1000 );
}

void set_Date() {
  WiFi.begin(ssid, password);  //wifi開始連接
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print( "SET TIME!" );

  timeClient.update();
  YEAR  = timeClient.getYear() ;
  HOUR  = timeClient.getHours();    hours = HOUR.toInt();
  MIN   = timeClient.getMinutes();  minutes = MIN.toInt();
  SEC   = timeClient.getSeconds();  seconds = SEC.toInt();
  SECOND = ( (hours * 3600 + minutes * 60 + seconds) - millis() / 1000 );
  if (YEAR == "1970") {
    set_Date();
  }
}

void print_Date() {
  Serial.print("     Time : ");
  if (hours < 10)   {
    Serial.print("0");
    Serial.print(hours);
  }    else {
    Serial.print(hours);
  }    Serial.print(":");
  if (minutes < 10) {
    Serial.print("0");
    Serial.print(minutes);
  }  else {
    Serial.print(minutes);
  }  Serial.print(":");
  if (seconds < 10) {
    Serial.print("0");
    Serial.println(seconds);
  } else {
    Serial.println(seconds);
  }

  lcd.setCursor ( 0, 1 ); lcd.print( "Time : " );
  if (hours   < 10) {
    lcd.print("0");
    lcd.print( hours );
  } else {
    lcd.print(hours);
  }   lcd.print(":");
  if (minutes < 10) {
    lcd.print("0");
    lcd.print(minutes);
  } else {
    lcd.print(minutes);
  } lcd.print(":");
  if (seconds < 10) {
    lcd.print("0");
    lcd.print(seconds);
  } else {
    lcd.print(seconds);
  }
}

void play() {
  //return ;
  if ( hours >= 6 && hours <= 9 ) {
    file = new AudioFileSourcePROGMEM( viola, sizeof(viola) );
  } else if (hours >= 17 && hours <= 23){
    file = new AudioFileSourcePROGMEM( viola2, sizeof(viola2) );
  }
  else{
    return ;
  }
  Serial.println(__LINE__);
  out = new AudioOutputI2SNoDAC();
  Serial.println(__LINE__);
  wav = new AudioGeneratorMP3();
  Serial.println(__LINE__);
  wav -> begin(file, out);
  Serial.println(__LINE__);
  delay (10);
  
  while( wav->isRunning() ){
    delay(1);
    if (!wav->loop()) {wav->stop();Serial.println(__LINE__);}
  }
   Serial.println(__LINE__);
   delete wav ;
   Serial.println(__LINE__);
   delete out ;
   Serial.println(__LINE__);
   delete file ;
   Serial.println(__LINE__);

   delay(15000);
}

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.print("Hello !");

  Serial.begin(57600);
  Serial.setTimeout(5000);
  pinMode(PIRout, INPUT);
  Serial.println("");

  Serial.println ( "START CONNECT" );
  lcd.setCursor( 0, 0 ); lcd.print( "START CONNECT" );
  timeClient.begin();
  delay(1000);
  set_Date();
  lcd.clear();  lcd.setCursor( 0, 0 ); lcd.print( "Running--" );
}

void loop() {
  timeNow = millis() / 1000 ; // millis is the function i have mentioned above
  if (timeNow != timeLast) {
    run_time();
  }

  if ( hours == 1 && (minutes == 1 || minutes == 2) ) {
    Serial.println("set_time");
    set_Date();
    delay(1000 * 10);
  }
  else {
    while ( (hours >= 6 && hours <= 9) || (hours >= 17 && hours <= 23)) {
      lcd.clear(); lcd.setCursor( 0, 0 );  lcd.print( "Ring" );
      
      digitalWrite(PIRout,LOW)  ;
      delay(1);
      int val = digitalRead(PIRout); //讀取 PIR 輸出
      if (val == HIGH) {   //PIR 有偵測到時
        play();
        delay(1000);
      }
      break;
    }
    print_Date();
    Serial.println("");
    delay(1000);
  }
}
