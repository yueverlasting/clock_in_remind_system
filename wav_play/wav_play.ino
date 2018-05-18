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

AudioGeneratorMP3 *wav ;
AudioFileSourcePROGMEM *file;
AudioOutputI2SNoDAC *out;

void setup()
{
  //WiFi.mode(WIFI_OFF);
  Serial.begin(115200);
  delay(1000);
  Serial.printf("\n");
  Serial.printf("WAV start\n");
  file = new AudioFileSourcePROGMEM( viola, sizeof(viola) );
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorMP3();
  wav->begin(file, out);
}

void loop()
{
  static int i = 0 ;
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
    
  } else {
    delete wav ;
    delete out ;
    delete file ;
    
    if ((++i)%2 == 0){
      file = new AudioFileSourcePROGMEM( viola, sizeof(viola) );
    }
    else{
      file = new AudioFileSourcePROGMEM( viola2, sizeof(viola2) );
    }
    out = new AudioOutputI2SNoDAC();
    wav = new AudioGeneratorMP3();
    Serial.printf("WAV done\n");
    wav -> begin(file,out);
    delay(1000);
  }
}

