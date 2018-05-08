#include "pitch.h"

const int PIRout= D2;
int i =0;

// notes in the melody:
int duration[2] = {300,600};  // 500 miliseconds
int note [2] ={NOTE_F6, NOTE_CS6};

void play(){
    for (int thisNote = 0; thisNote < 2; thisNote++) {
    // 在 pinD3 上輸出聲音，每個音階響 0.5 秒
    tone(D3, note[thisNote], duration[thisNote]);
    // 間隔一段時間後再播放下一個音階
    delay(400);
    }
    i++;if(i>1000){i=0;}
    delay(2000);
}

void setup() {  //定義針腳輸出入
  Serial.begin(57600);
  pinMode(PIRout,INPUT);
  Serial.println("");
  }
void loop() {
  int val=digitalRead(PIRout);  //讀取 PIR 輸出
  if (val == HIGH) {   //PIR 有偵測到時 : LED 閃一下
    Serial.print("Detect! #");
    Serial.println(i);
    play();
  }

}
