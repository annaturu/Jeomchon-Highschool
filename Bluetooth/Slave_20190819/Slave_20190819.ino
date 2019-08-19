// Uno Slave Device: Aug. 19, 2019

#include <SoftwareSerial.h>
#include <avr/power.h>

#define rxPin 2
#define txPin 3
#define RELAY 6

SoftwareSerial BtSerial(txPin, rxPin);

int val = 100;
int cnt = 0;
void setup() {
// pinMode(12, OUTPUT);
pinMode(RELAY, OUTPUT); //Set Pin3 as Relay output
Serial.begin(9600);
BtSerial.begin(9600);

digitalWrite(RELAY, LOW); //Set Pin3 as Relay output
delay(500);
digitalWrite(RELAY, HIGH); //Set Pin3 as Relay output
delay(500);
}
void loop() {
// Some example procedures showing how to display to the pixels:
// if (BtSerial.available()) {
val = BtSerial.read();
// }
cnt++;
Serial.println(val);
if (val == 1 && cnt>=2) {
digitalWrite(RELAY, LOW); //Turn on relay
/*
digitalWrite(13, HIGH);
delay(100);
digitalWrite(13, LOW);
delay(100);
*/
delay(500);
digitalWrite(RELAY, HIGH);
delay(500);
cnt=0;

}
else if(val == 0 ){
digitalWrite(RELAY, LOW); //Turn off relay
// digitalWrite(13, LOW);
delay(100);

}
else{
digitalWrite(RELAY, LOW); //Turn off relay
delay(100);
}
}
