#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>
#define buzzer 4

Servo servo;
uint8_t data[3];


//String inString = "";
int SyncWord = 0x22;
void setup() {
  data[2]=127;
  servo.attach(3);
  servo.write(80);
  
  Serial.begin(1000000);
  
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(500E3);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(SyncWord);
  LoRa.setGain(6);
  Serial.println("Setup success");
//  
}
 
void loop() {
//  if(Serial.available()){
//    Serial.readBytes(data, 3);
//  }
  
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet    
    while (LoRa.available())
    {
      data[2] = uint8_t(LoRa.read());
    }
    LoRa.packetRssi();    
  }
  uint8_t value = map(data[2], 97, 157, 40, 120);
//  Serial.println(value);
  servo.write(value);
}
