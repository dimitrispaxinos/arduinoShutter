#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

	Serial.begin(9600);

	// Transmitter is connected to Arduino Pin #10 
	mySwitch.enableTransmit(10);

	// Optional set pulse length.
	mySwitch.setPulseLength(150);

	// set protocol (default is 1, will work for most outlets)
	// mySwitch.setProtocol(2);

	// Optional set number of transmission repetitions.
	 mySwitch.setRepeatTransmit(5);

	//pinMode(13,OUTPUT);

}
int done = 0;
void loop() {
	/*if (done == 1)
		return;*/
	//// Turn on
	//mySwitch.send("1000100101001110000");
	done = 1;
	delay(200);
	////// Turn off  
	/*mySwitch.send("010101010100010101010100");
	delay(18000);*/
}