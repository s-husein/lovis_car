#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Kay";
const char* pass = "ZraKay71";
WiFiUDP udp;
unsigned short port = 1234;
const unsigned char servopin = 13;

const unsigned char frwd_pin = 12;
const unsigned char bkwd_pin = 14;
const unsigned short freq = 10000;
const unsigned char res = 8;

unsigned char incom_pack[3];
unsigned char pack_size = 0;


Servo servo1;

void setup() {
  pinMode(2, OUTPUT);

  ledcAttachChannel(frwd_pin, freq, res, 0);
  ledcAttachChannel(bkwd_pin, freq, res, 1);
  servo1.attach(servopin);
  
  digitalWrite(2, LOW);
  
  incom_pack[2] = 127;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
  }
  if(WiFi.status() == WL_CONNECTED) digitalWrite(2, HIGH);
  udp.begin(port);
}


void loop() {
  pack_size = udp.parsePacket();
  if(pack_size){
    udp.read(incom_pack, 3);
  }
  ledcWrite(frwd_pin, incom_pack[0]);
  ledcWrite(bkwd_pin, incom_pack[1]);
  int value = map(incom_pack[2], 97, 157, 52, 112);
  servo1.write(value);
  
}
