/*
Aman :The low cost wireless stress monitoring sock
Quantify stress comfortably by measuring Electrodermal Activity(EDA) and Heart Rate along with contextual info like Humidity,Temp & Motion.
Uses Pulse Sensor Amped for HR sensing, HTU21 for Hum&Temp, fabric electrodes for EDA & ADXL345 for motion
Connections: 
Connect SDA, SCL of HTU21d to D0,D1 respectively and Vin,Gnd to 3.3V and Gnd.
Connect Pulse sensor signal out to A2 and EDA filtered output to A0

DISCLAIMER 
**While I aim to make this more safe, accurate it this is experimental software (and pre-beta) at that.
It is not suitable for any particular purpose. No life-critical devices should be based on this software.**

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "adafruit-htu21df/adafruit-htu21df.h"
#include "ThingSpeak/ThingSpeak.h"
#include "PulseSensor_Spark/SparkIntervalTimer.h"

//SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

void interruptSetup(void);
unsigned int myChannelNumber = ; // replace with your ChannelID
const char * myWriteAPIKey = ""; // replace with your WriteAPIKey
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
TCPClient client;
uint32_t tsLastReport1 = 0;

extern int pulsePin;
extern int blinkPin;
extern volatile int BPM;;
extern volatile int Signal;;
extern volatile int IBI;
extern volatile boolean Pulse;
extern volatile boolean QS;

void setup(){
	pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
	ThingSpeak.begin(client);
	interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
    WiFi.on();
    Particle.connect();    
    waitUntil(Particle.connected);    
}



void loop(){

WiFi.on();
waitUntil(WiFi.ready);
Particle.connect();    
int sensorValue = analogRead(A0); //Get EDA reading
float temp=htu.readTemperature();
float hum=htu.readHumidity();
Serial.print("HR  "); 
Serial.print(BPM); //Heart rate
Serial.print("  Temp:");
Serial.print(temp); //Temperature
Serial.print("  eda ");
Serial.print(sensorValue); //EDA value
Serial.println(" ");
waitUntil(Particle.connected);
Particle.process();
Particle.publish("BPM", String(BPM));
Particle.publish("temp", String(temp));
Particle.publish("hum", String(hum));
Particle.publish("EDA", String(sensorValue));   
ThingSpeak.setField(1,BPM);
ThingSpeak.setField(2,sensorValue);
ThingSpeak.setField(3,hum);
ThingSpeak.setField(4,temp);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);   //Update Thingspeak
WiFi.off();
delay(6000);
}








