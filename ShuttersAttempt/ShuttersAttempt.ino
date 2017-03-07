/*
Name:		ShuttersAttempt.ino
Created:	3/7/2017 8:03:02 PM
Author:	RevilS
*/

int upRelayPin = 12;
int downRelayPin = 10;

int upButtonPin = 2;
int downButtonPin = 4;

int upRelayPinState = LOW;
int downRelayPinState = LOW;

int upButtonPinState = LOW;
int downButtonPinState = LOW;

// RF Instructions
int goUpRfCode = 12;
int goDownRfCode = 14;

int rfCodeInput = 0;


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(upRelayPin, OUTPUT);
	pinMode(downRelayPin, OUTPUT);

	pinMode(upButtonPin, INPUT);
	pinMode(downButtonPin, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {

	// Read Input
	upButtonPinState = digitalRead(upButtonPin);
	downButtonPinState = digitalRead(downButtonPin);

	upRelayPinState = digitalRead(upRelayPin);
	downRelayPinState = digitalRead(downRelayPin);

	if (upButtonPinState == HIGH || downButtonPinState == HIGH)
	{
		// If both are pressed, stop everything and return
		if (upButtonPinState == HIGH && downButtonPinState == HIGH)
		{
			stopEverything();
			return;
		}

		setUpRelayState(upButtonPinState);	   
		setDownRelayState(downButtonPinState);	   
	}
	else
	{
		if (rfCodeInput == 0)
			stopEverything();
	}

}

void stopEverything()
{
	setUpRelayState(LOW);
	setDownRelayState(LOW);
}

void setUpRelayState(int state)
{
	if (upRelayPinState != state)
	{
		upRelayPinState = state;
		digitalWrite(upRelayPin, upRelayPinState);
	}
}

void setDownRelayState(int state)
{
	if (downRelayPinState != state)
	{
		downRelayPinState = state;
		digitalWrite(downRelayPin, downRelayPinState);
	}
}
