/*
 Name:		WirelessAndManualLightSwitch.ino
 Created:	3/14/2017 10:49:38 PM
 Author:	RevilS
*/

#include <RCSwitch.h>
int RELAY = 10;
int SWITCH = 12;
int SWITCH_ON_RF_CODE = 280100;
int SWITCH_OFF_RF_CODE = 280200;
int relayState;
int rfCodeInput = 0;
int switchState = HIGH;
int lastSwitchState = HIGH;

int experimentDone = 0;

RCSwitch mySwitch = RCSwitch();


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(SWITCH, INPUT_PULLUP);
	pinMode(RELAY, OUTPUT);

	Serial.begin(9600);
	mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2	
}

// the loop function runs over and over again until power down or reset
void loop() {
	rfCodeInput = 0; //activate after adding the RF module

	 if (mySwitch.available()) {
		 rfCodeInput = mySwitch.getReceivedValue();
		 Serial.println(mySwitch.getReceivedValue());
		 mySwitch.resetAvailable();
	 }

	switchState = digitalRead(SWITCH);
	relayState = digitalRead(RELAY);

	if (switchState != lastSwitchState)
	{
		lastSwitchState = switchState;
		digitalWrite(RELAY, !relayState);
	}

	if (rfCodeInput == SWITCH_OFF_RF_CODE)
	{
		digitalWrite(RELAY, HIGH);
	}

	if (rfCodeInput == SWITCH_ON_RF_CODE)
	{
		digitalWrite(RELAY, LOW);
	}
}
