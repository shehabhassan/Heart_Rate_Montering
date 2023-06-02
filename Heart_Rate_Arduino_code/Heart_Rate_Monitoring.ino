/*******************************************
*
*New Version of IOT - Version - for project . 
* 
********************************************/
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleStream.h>
/***************************************************/
#include <DHT.h>
#define DHTPIN 2          
#define DHTTYPE DHT11     // DHT 11
/***************************************************/
DHT dht(DHTPIN, DHTTYPE);
/**************************************************/
#define BLYNK_TEMPLATE_ID "TMPL-Rbmpbc8"
#define BLYNK_TEMPLATE_NAME "shehab"
#define BLYNK_AUTH_TOKEN "VWXmpgv6UKZTMGCdsdkgTM3XFWpe_hzg"
/**************************************************/
//  Variables
const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
LiquidCrystal_I2C lcd(0x20,20,4);
/**************************************************************************/
BlynkTimer timer;
void myTimer() 
{
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);  
 //int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
 int myBPM = pulseSensor.getBeatsPerMinute();
   Blynk.virtualWrite(V0,myBPM);
 /****************************************************/
   float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
   if ( isnan(t)) {
   Serial.println("Failed to read from DHT sensor!");
    return;
  }
    Blynk.virtualWrite(V1, t);
    /**************************************************/
}
/**************************************************************************/

void setup() {   
  Serial.begin(9600);          // For Serial Monitor
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   
  lcd.init();
  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) 
   {
    Serial.println("DONE");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
    Blynk.begin(Serial, BLYNK_AUTH_TOKEN);
   timer.setInterval(2000L, myTimer); 
     /***************************/
  dht.begin();
  /*****************************/
}
void loop() {
 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".                                                          
if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("A Heart Beating!");
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
}
  delay(20);                    // considered best practice in a simple sketch.
 Blynk.run();
  timer.run(); 
}