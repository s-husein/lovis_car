#include <SPI.h>
#include <LoRa.h> 

int SyncWord = 0x22;
uint8_t data[3];


void setup() {
  Serial.begin(1000000);
  
  while (!Serial);  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(7);           // ranges from 6-12,default 7 see API docs
  LoRa.setSignalBandwidth(500E3 );           // for -139dB (page - 112)
  LoRa.setCodingRate4(5);                 // for -139dB (page - 112)
  LoRa.setSyncWord(SyncWord);
  LoRa.setTxPower(20, true);
  Serial.println("Success setup");
  
}
 
void loop() {
  if(Serial.available()){
    Serial.readBytes(data, 3);
    LoRa.beginPacket();
    LoRa.write(data, 3);
    LoRa.endPacket();
    Serial.print(data[0]);
    Serial.print(", ");
    Serial.print(data[1]);
    Serial.print(", ");
    Serial.println(data[2]);
//    delay(50);
  }
  
//  Serial.println(val);
 
}
