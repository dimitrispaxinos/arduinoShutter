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
//int goUpRfCode = 12;
//int goDownRfCode = 14;

int rfCodeInput = 0;

// RfCommands
int rfGoUpCode = 1500;
int rfGoDownCode = 1600;

// RfCommands Duration
int rfGoUpDuration = 5000;
int rfGoDownDuration = 5000;

// RfCommands UpdateTime
unsigned long rfUpNextUpdateTime = 0;
unsigned long rfDownNextUpdateTime = 0;

int goUpexperimentDone = false;
int goDownexperimentDone = false;


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(upRelayPin, OUTPUT);
	pinMode(downRelayPin, OUTPUT);

	pinMode(upButtonPin, INPUT);
	pinMode(downButtonPin, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	/*For Testing*/
	if (rfCodeInput == 1500 || rfCodeInput == 1600)
	{
		rfCodeInput = 0;
	}

	if (millis() > 1500 && !goUpexperimentDone)
	{
		goUpexperimentDone = true;
		rfCodeInput = 1500;
	}

	if (millis() > 10000 && !goDownexperimentDone)
	{
		goDownexperimentDone = true;
		rfCodeInput = 1600;
	}


	/*End of Testing*/

	// Read Input
	upButtonPinState = digitalRead(upButtonPin);
	downButtonPinState = digitalRead(downButtonPin);

	upRelayPinState = digitalRead(upRelayPin);
	downRelayPinState = digitalRead(downRelayPin);

	// 
	if (upRelayPinState == HIGH && downRelayPinState == HIGH)
	{
		stopEverything();
		return;
	}


	if (upButtonPinState == HIGH || downButtonPinState == HIGH)
	{
		// If both are pressed, stop everything and return
		if (upButtonPinState == HIGH && downButtonPinState == HIGH)
		{
			stopEverything();
			return;
		}

		////////////////////////// Case otan allazei kateuthinsi amesws gia na min kaei to motori.



	/*	if (upButtonPinState == HIGH && downRelayPinState == HIGH)
		{
			setUpRelayState(upButtonPinState);
			setDownRelayState(downButtonPinState);
		}
		else
		{
			setDownRelayState(downButtonPinState);
			setUpRelayState(upButtonPinState);			
		}*/

		setUpRelayState(upButtonPinState);
		setDownRelayState(downButtonPinState);

		rfUpNextUpdateTime = 0;
		rfDownNextUpdateTime = 0;
	}
	else
	{
		if (rfUpNextUpdateTime < millis())
		{
			rfUpNextUpdateTime = 0;
			setUpRelayState(LOW);
		}

		if (rfDownNextUpdateTime < millis())
		{
			rfDownNextUpdateTime = 0;
			setDownRelayState(LOW);
		}

		// Return if code does not match
		if (rfCodeInput != rfGoUpCode && rfCodeInput != rfGoDownCode)
		{
			return;
		}
		else
		{
			if (rfCodeInput == rfGoUpCode && rfUpNextUpdateTime == 0)
			{
				if (downRelayPinState == HIGH && rfDownNextUpdateTime > 0)
				{
					setDownRelayState(LOW);
					rfDownNextUpdateTime = 0;
					delay(2000);
				}
				rfUpNextUpdateTime = millis() + rfGoUpDuration;
				setUpRelayState(HIGH);
			}
			else if (rfCodeInput == rfGoDownCode && rfDownNextUpdateTime == 0)
			{
				if (upRelayPinState == HIGH && rfUpNextUpdateTime > 0)
				{
					setUpRelayState(LOW);
					rfUpNextUpdateTime = 0;
					delay(2000);
				}
				rfDownNextUpdateTime = millis() + rfGoDownDuration;
				setDownRelayState(HIGH);
			}
		}
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
	{/*
		if (upRelayPinState==HIGH && (downRelayPinState == HIGH || rfDownNextUpdateTime > 0))
		{
			setDownRelayState(LOW);
			rfDownNextUpdateTime = 0;
			delay(2000);
		}	 
*/
		upRelayPinState = state;
		digitalWrite(upRelayPin, upRelayPinState);
	}
}

void setDownRelayState(int state)
{
	if (downRelayPinState != state)
	{
		/*if (downRelayPinState==HIGH && (upRelayPinState == HIGH || rfUpNextUpdateTime > 0))
		{
			setUpRelayState(LOW);
			rfUpNextUpdateTime = 0;
			delay(2000);
		}
*/
		downRelayPinState = state;
		digitalWrite(downRelayPin, downRelayPinState);
	}
}
