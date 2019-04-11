#include "SPI.h"
#include "MFRC522.h"
int butt=0;
#include <SoftwareSerial.h>
#define SS_PIN 10
#define RST_PIN 9
#define SP_PIN 8
#include <Servo.h>
SoftwareSerial mySerial(3, 4);

Servo servo;

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup()
{
  pinMode(6,INPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  servo.attach(8);
  servo.write(0);
  mySerial.println("AT+CNMI=2,2,0,0,0");
  delay(2000);
}

void loop()
{
  if(digitalRead(6) ==HIGH && butt==0)
  MakeCall();
  if(digitalRead(6) ==LOW)
  butt=0;
  if (mySerial.available()>0)
  Serial.write(mySerial.read());
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;
    
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  String strID = "";
  for (byte i = 0; i < 4; i++)
  {
    strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
 if (strID.indexOf("A0:9E:3A:25") >= 0)
 {
  servo.write(170);
  delay(3000);
  servo.write(0);
  delay(500);
 }
 else
 Serial.print("unauthorised");
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
 void ReceiveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a  live SMS
  delay(1000);
}
  void MakeCall()
 {
  mySerial.println("ATD+918871799762;");
  Serial.println("calling...");
  butt=1;
  delay(2000);
 }
