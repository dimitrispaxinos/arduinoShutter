/*
 Name:		Sketch1.ino
 Created:	3/2/2017 10:29:23 PM
 Author:	RevilS
*/

const int LEDPIN = 5;
//const int LEDPIN = LED_BUILTIN;
// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LEDPIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
	Serial.println("Turning it on");
	digitalWrite(LEDPIN,HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);                       // wait for a second
	Serial.println("Turning it off");
	digitalWrite(LEDPIN, LOW);    // turn the LED off by making the voltage LOW
	delay(1000);                       // wait for a second
}

