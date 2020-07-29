#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "Adafruit_GFX.h"
//#include "OakOLED.h"
 
#define REPORTING_PERIOD_MS 1000
//OakOLED oled;

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "-iVdkyreJQSeqeUYLlE0cSH_JxqIeLAf";             // You should get Auth Token in the Blynk App.
char ssid[] = "BK Network";                                     // Your WiFi credentials.
char pass[] = "school100";
int incomingByte = 0;

 int i =0;
// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
 
 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
}
 
void setup()
{
    Serial.begin(115200);
     Wire.begin(D2, D1);
    
    lcd.begin();
    
    lcd.home();
    
    lcd.print("Hello"); 
    lcd.setCursor(0,1);
    lcd.print("Initilizing Sensors");
    Serial.println("Beat Detected!");
    pinMode(16, OUTPUT);
    Serial.println("Beat Detected!");
    
    Blynk.begin(auth, ssid, pass);
 
    Serial.print("Initializing Pulse Oximeter..");
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
     //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
   
}
 
void loop()
{  
    pox.update();
    Blynk.run();
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
     if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
        Blynk.virtualWrite(V7, BPM);
        Blynk.virtualWrite(V8, SpO2);
        tsLastReport = millis();
        lcd.clear();
        lcd.home();
        lcd.print("BPM : "); 
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("SpO2 : ");
        lcd.print(SpO2);
    }

    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte==84){
       Serial.println(" hello");
       Blynk.notify("Emergency: ");
       Blynk.email("sarthakrawat7295@gmail.com", "ESP8266 Alert", "Temperature over 28C!");
    }
  }

}
