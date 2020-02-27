
#include "ThingSpeak.h"
//#include <secrets.h>

unsigned long myChannelNumber = 902378; //put channel number from thingspeak cloud channel
const char * myWriteAPIKey = "HFOXQ46C8QKDXT"; //thingspeak channel API key

#include <ESP8266WiFi.h>

char ssid[] = "abcd" ;   // your wifi network SSID (name)
char pass[] = "1234567890";   // your wifi network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;



int LED13 = 13;   //  The on-board Arduion LED
int count=0;
int state= 0;
int laststate= 0;

int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550; // Determine which Signal to "count as a beat", and which to ingore. 
int time1=0;
int time2=0;




void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);
}

// the loop function runs over and over again forever
void loop() {


  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("abcd");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  
  
  if(count==0) {
    time1=millis();
  }
  time2=millis();

  Signal = analogRead(A0);  // Read the PulseSensor's value. 
                                              
    if(Signal > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.  
     digitalWrite(LED_BUILTIN,HIGH);
     state=1;          
   } else {
     digitalWrite(LED_BUILTIN,LOW);
     state=0;//  Else, the sigal must be below "550", so "turn-off" this LED.
   }

   if (state != laststate) {
    // if the state has changed, increment the counter
    if (state == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      count++;
    } 
    else {
      // if the current state is LOW then the button went from on to off:
      //Serial.println("off");
    }
    delay(50);
   }
    laststate=state;

    if(time2>=time1+15000) {
      int bpm;
       if(count>5){ 
       bpm=count*4;}
       else{
        bpm=0;
       }


  int httpCode = ThingSpeak.writeField(myChannelNumber, 1,bpm, myWriteAPIKey);

  if (httpCode == 200) {
    Serial.print("Heartbeat rate is ");
    Serial.print(bpm);
    Serial.print(" per min\n");
//    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }

  // Wait 20 seconds to uodate the channel again
  delay(15000);


  time1=0;
  time2=0;
  count=0;
      }}
