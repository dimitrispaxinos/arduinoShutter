#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

	Serial.begin(9600);

	// Transmitter is connected to Arduino Pin #10 
	mySwitch.enableTransmit(10);

	// Optional set pulse length.
	mySwitch.setPulseLength(322);

	// set protocol (default is 1, will work for most outlets)
	// mySwitch.setProtocol(2);

	// Optional set number of transmission repetitions.
	// mySwitch.setRepeatTransmit(15);

	//pinMode(13,OUTPUT);

}

void loop() {

	//// Turn on
	//mySwitch.send("000000000001010100010001");
	//delay(5000);
	//// Turn off  
	//mySwitch.send("000000000001010100010100");
	//delay(5000);
}