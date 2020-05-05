#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Choose two Arduino pins to use for software serial
int RXPin = 3;
int TXPin = 4;   

// Create a TinyGPS++ object
TinyGPSPlus gps;

SoftwareSerial gpsSerial(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
   gpsSerial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(20);
  
}

void loop() {
  
 while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
    
        if (gps.location.isValid())
  {
    Serial.println("Sending to LoRa");
    LoRa.beginPacket();
    LoRa.print("Lat: ");
    LoRa.print(gps.location.lat(), 6);
    LoRa.print("c");
    LoRa.print("Long: ");
    LoRa.print(gps.location.lng(), 6);
    Serial.println("Sent via LoRa");
    LoRa.endPacket();
  }

}

Lora Receiver Code

/*Program to receive the value of temperature and Humidity via LoRa and prin on LCD
 *Dated: 24-06-2019  
 *For: www.circuitdigest.com
 */

#include <SPI.h> //SPI Library 
#include <LoRa.h> //LoRa Library 
#include <LiquidCrystal.h> //Library for LCD

const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3; //Mention the pin number for LCD connection
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//Initialize LCD method

void setup() {
  Serial.begin(9600); //Serial for Debugging 
  
  lcd.begin(16, 2); //Initialise 16*2 LCD
  lcd.print("Arduino LoRa"); //Intro Message line 1
  lcd.setCursor(0, 1);
  lcd.print("Receiver"); //Intro Message line 2
  delay(2000);
  
  if (!LoRa.begin(433E6)) { //Operate on 433MHz
    Serial.println("Starting LoRa failed!");
    lcd.print("LoRa Failed");
    while (1);
  }
}

void loop() {

  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {     // If packet received
    Serial.print("Received packet '");
    lcd.clear();

    while (LoRa.available()) {
      char incoming = (char)LoRa.read();
      if (incoming == 'c')
      {
        lcd.setCursor(0, 1);
      }
      else
      {
        lcd.print(incoming);
      }
      
    }

  }
}
