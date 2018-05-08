const int led = D3;
const int PIRout= D2;
int i =0;

void setup() {  //定義針腳輸出入
  Serial.begin(57600);
  pinMode(led,OUTPUT);  
  pinMode(PIRout,INPUT);
  Serial.println("");
  }
void loop() {
  int val=digitalRead(PIRout);  //讀取 PIR 輸出
  if (val == HIGH) {   //PIR 有偵測到時 : LED 閃一下
    Serial.print("Detect! #");
    Serial.println(i);
    digitalWrite(led,HIGH);
    delay(1000*3);
    digitalWrite(led,LOW);
    delay(1000*2);
    i++;if(i>1000){i=0;}
    }
    else{
      digitalWrite(led,LOW);
    }


}
