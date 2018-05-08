#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//定義變數
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 28800);

const int led = D3;
const int PIRout= D2;
int i =0;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 0;

const char *ssid     = "Yueverlasting";
const char *password = "10241228";
String cday[7]={"sun","mon","tue","wen","tur","fri","sat"};
char*  Ntime ;
String YEAR,MONTH,DATE,HOUR,MIN,SEC,CDAY;

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
}
void set_Date(){
  //print the date EG   3-1-11 23:59:59
  timeClient.update();
  YEAR  = timeClient.getYear() ;
  HOUR  = timeClient.getHours();hours = HOUR.toInt();
  MIN   = timeClient.getMinutes();minutes = MIN.toInt();
  SEC   = timeClient.getSeconds();seconds = SEC.toInt();
}
void set_Date2(){
  //print the date EG   3-1-11 23:59:59
  timeClient.update();
  YEAR  = timeClient.getYear() ;
  HOUR  = timeClient.getHours();
  MIN   = timeClient.getMinutes();
  SEC   = timeClient.getSeconds();
}
void print_Date(){
    //print the date EG  23:59:59
    Serial.print("    Time : ");
    if(hours <10){Serial.print("0");Serial.print(hours);}else{Serial.print(hours);}
    Serial.print(":");
    if(minutes <10){Serial.print("0");Serial.print(minutes);}else{Serial.print(minutes);}
    Serial.print(":");
    if(seconds <10){Serial.print("0");Serial.println(seconds);}else{Serial.println(seconds);}
}

void print_netDate(){
  set_Date2();
  Serial.print("NET Time : ");
    if(HOUR.toInt() <=9){Serial.print("0");Serial.print(HOUR.toInt());}else{Serial.print(HOUR.toInt());}
    Serial.print(":");
    if(MIN.toInt() <=9){Serial.print("0");Serial.print(MIN.toInt());}else{Serial.print(MIN.toInt());}
    Serial.print(":");
    if(SEC.toInt() <= 9){Serial.print("0");Serial.println(SEC.toInt());}else{Serial.println(SEC.toInt());}
}


void setup(){
  
    /*WIFI部分*/
  Serial.begin(57600);
  Serial.setTimeout(5000);
  Wire.begin(D4,D3);

   pinMode(led,OUTPUT);  
  pinMode(PIRout,INPUT);
  Serial.println("");
  
  WiFi.begin(ssid, password);  //wifi開始連接
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println ( "START CONNECT" );
  timeClient.begin();
  delay(1000);
  //timeClient.update();delay(1000);
  //timeClient.update();delay(1000);
  //timeClient.update();delay(1000);
  set_Date();
  while( YEAR == "1970") {Serial.println ( "Try set" );set_Date();}
  print_netDate();
}


void loop() {
  timeNow = millis()/1000 ;// millis is the function i have mentioned above
  Serial.print("timeLast = ");
  Serial.println(timeLast);
  Serial.print("timeNow  = ");
  Serial.println(timeNow);

  if(timeNow != timeLast) {run_time();}
  
  if( HOUR == "01" && (MIN == "0" || MIN == "1") ) {
    Serial.println("set_time");
    set_Date();
  }
  else{
      while(HOUR == "07" || HOUR == "08" || HOUR == "09" || HOUR == "17" || HOUR == "18" || HOUR == "19" || HOUR == "13")
       {
          Serial.println("ring~ring~ring~");
          delay(1000*5);
          break;
        }
  print_netDate();     
  print_Date();
  Serial.println("");
  delay(1000);
  }
}


