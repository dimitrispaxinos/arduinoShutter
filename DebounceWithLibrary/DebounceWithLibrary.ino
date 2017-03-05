/*
 Name:		DebounceWithLibrary.ino
 Created:	3/5/2017 8:17:53 PM
 Author:	RevilS
*/

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

#define BUTTON_PIN_1 10
//#define BUTTON_PIN_2 3


#define LED_PIN 5
int ledState = 0;

// Instantiate a Bounce object
Bounce debouncer1 = Bounce();	 

void setup() {

	// Setup the first button with an internal pull-up :
	pinMode(BUTTON_PIN_1, INPUT);
	// After setting up the button, setup the Bounce instance :
	debouncer1.attach(BUTTON_PIN_1);
	debouncer1.interval(220); // interval in ms		  

	//Setup the LED :
	pinMode(LED_PIN, OUTPUT);		
	digitalWrite(LED_PIN, ledState);
}

void loop() {
	// Update the Bounce instances :
	debouncer1.update();

	// Get the updated value :
	int value1 = debouncer1.read();

	// Turn on the LED if either button is pressed :
	if (value1 == HIGH) {
		ledState = !ledState;
		digitalWrite(LED_PIN, ledState);
	}
	/*else {
		Serial.println("LOW");
		digitalWrite(LED_PIN, LOW);
	}*/

}

