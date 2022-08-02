#include <SimpleKalmanFilter.h>
// Khai bao ID
#define BLYNK_TEMPLATE_ID "TMPL5KBQvvxg"
#define BLYNK_DEVICE_NAME "Plant"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

SimpleKalmanFilter simpleKalmanFilter(1, 1, 0.01);

//Khai bao bien
int sttled = D6;
int doc = A0;
int rl = D4;
int Percent_raw = 0; 
int maplow = 655;
int maphigh = 400;
int rlstt = 0;
// Ket thuc khai bao

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

void setup()
{
  pinMode(sttled, OUTPUT);  
  pinMode(rl, OUTPUT);
  pinMode(doc, INPUT);
  
  digitalWrite(rl, HIGH);
  
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();

  digitalWrite(sttled, HIGH);
  Blynk.virtualWrite(V1, digitalRead(sttled));
  
  int analogValue = analogRead(doc);
  Percent_raw = map(analogValue,maplow, maphigh,0, 100);
// Dung bo loc Kalman de lay gia tri chuan nhat
  int Percent = simpleKalmanFilter.updateEstimate(Percent_raw); 
  Serial.print(Percent);
  Serial.println("%");  
  if (Percent < 60){
    digitalWrite(rl, LOW); // Relay kich muc thap
    rlstt = 1;
    Blynk.virtualWrite(V2, digitalRead(rlstt));
  } 
  else if(Percent < 70){
    digitalWrite(rl, HIGH);
    rlstt = 0;
    Blynk.virtualWrite(V2, digitalRead(rlstt));   
  }
  Blynk.virtualWrite(V0, Percent);  
}
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();
  //Blynk.syncVirtual(V0, V2);
}
