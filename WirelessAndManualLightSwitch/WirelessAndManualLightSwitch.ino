/*
 Name:		WirelessAndManualLightSwitch.ino
 Created:	3/14/2017 10:49:38 PM
 Author:	RevilS
*/

class WirelessAndManualSwitch{

public:
	WirelessAndManualSwitch() {};

public:
	WirelessAndManualSwitch(int relay, int manualSwitch, int switchOnRfCode, int switchOffRfCode) {
		_relay = relay;
		_switch = manualSwitch;
		_switchOnRfCode = switchOnRfCode;
		_switchOffRfCode = switchOffRfCode;
	};


	void setup()	 	{
		pinMode(_switch, INPUT_PULLUP);
		pinMode(_relay, OUTPUT);
	}

	int _relay = 10;
	int _switch = 12;
	int _switchOnRfCode = 280100;
	int _switchOffRfCode = 280200;
	int relayState;
	int switchState = HIGH;
	int lastSwitchState = HIGH;

	void Update(int rfCode)
	{
		switchState = digitalRead(_switch);
		relayState = digitalRead(_relay);

		if (switchState != lastSwitchState)
		{
			lastSwitchState = switchState;
			digitalWrite(_relay, !relayState);
		}

		if (rfCode == _switchOffRfCode)
		{
			digitalWrite(_relay, HIGH);
		}

		if (rfCode == _switchOnRfCode)
		{
			digitalWrite(_relay, LOW);
		}
	}  
};

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

WirelessAndManualSwitch wSwitch = WirelessAndManualSwitch(RELAY, SWITCH, SWITCH_ON_RF_CODE, SWITCH_OFF_RF_CODE);

// the setup function runs once when you press reset or power the board
void setup() {
	wSwitch.setup();

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

	wSwitch.Update(rfCodeInput);	
}

