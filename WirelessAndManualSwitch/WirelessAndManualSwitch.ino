/*
 Name:		WirelessAndManualSwitch.ino
 Created:	3/4/2017 6:31:54 PM
 Author:	RevilS
*/

#include <RCSwitch.h>

const int BUTTON = 3;
//const int RELAY1 = 3;
RCSwitch mySwitch = RCSwitch();

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(BUTTON, INPUT);
	//Serial.begin(9600);
	//mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
	digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
	int buttonValue = digitalRead(BUTTON);
	if (buttonValue == HIGH)
	{
		int currentStatus = digitalRead(LED_BUILTIN);
		if(currentStatus==LOW)
		{
			digitalWrite(LED_BUILTIN, HIGH);
		}
		else
		{
			digitalWrite(LED_BUILTIN, LOW);
		}
	}
	/*if (mySwitch.available()) {
		output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(), mySwitch.getReceivedProtocol());
		mySwitch.resetAvailable();
	}*/
}
