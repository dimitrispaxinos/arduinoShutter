/*
Debounce

Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
a minimum delay between toggles to debounce the circuit (i.e. to ignore
noise).

The circuit:
* LED attached from pin 13 to ground
* pushbutton attached from pin 2 to +5V
* 10K resistor attached from pin 2 to ground

* Note: On most Arduino boards, there is already an LED on the board
connected to pin 13, so you don't need any extra components for this example.


created 21 November 2006
by David A. Mellis
modified 30 Aug 2011
by Limor Fried
modified 28 Dec 2012
by Mike Walters
modified 30 Aug 2016
by Arturo Guadalupi


This example code is in the public domain.

http://www.arduino.cc/en/Tutorial/Debounce
*/

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 10;    // the number of the pushbutton pin
const int ledPin = 5;      // the number of the LED pin

						   // Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

							 // the following variables are unsigned long's because the time, measured in miliseconds,
							 // will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 120;    // the debounce time; increase if the output flickers
unsigned long nextUpdateTime = 0;


void setup() {
	pinMode(buttonPin, INPUT);
	pinMode(ledPin, OUTPUT);

	// set initial LED state
	digitalWrite(ledPin, ledState);

}
int reading = 0;
void loop() {
	// read the state of the switch into a local variable:
	int reading = digitalRead(buttonPin);
	//Serial.println(reading);

	if (reading == 1 && nextUpdateTime == 0)
	{
		nextUpdateTime = millis() + 3000;
		Serial.println(nextUpdateTime);
		ledState = debounce(buttonPin, ledPin, reading, buttonState, lastButtonState, lastDebounceTime, debounceDelay, ledState);
		digitalWrite(ledPin, ledState);
	}
	if (nextUpdateTime > 0 && nextUpdateTime < millis())
	{
		nextUpdateTime = 0;
		Serial.println("Back to zero");
		Serial.println(millis());
		reading = LOW;
		ledState = LOW;
		digitalWrite(ledPin, ledState);
	}


	lastButtonState = reading;
}


int debounce(int btnpin,
	int lpin,
	int reading,
	int &btnState,
	int lastbtnState,
	unsigned long &lDTime,
	unsigned long delay, int lState)
{

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH),  and you've waited
	// long enough since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != lastbtnState) {
		// reset the debouncing timer
		lDTime = millis();
	}

	if ((millis() - lDTime) > delay) {
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != btnState) {
			btnState = reading;

			// only toggle the LED if the new button state is HIGH
			if (btnState == HIGH) {
				lState = !lState;
			}
			return lState;
		}
	}
}