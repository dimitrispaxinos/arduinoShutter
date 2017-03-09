/*
Name:		ShuttersAttempt.ino
Created:	3/7/2017 8:03:02 PM
Author:	RevilS
*/

class Relay {

	int _pin;
	int _state;

public:
	Relay() {};


public:
	Relay(int pin)
	{
		_pin = pin;
		_state = LOW;
		pinMode(_pin, OUTPUT);
	}

	int getState()
	{
		_state = digitalRead(_pin);
		return _state;
	}

	void setState(int state)
	{
		if (_state != state)
		{
			_state = state;
			digitalWrite(_pin, _state);
		}
	}

};

class RfCommand {

public:
	RfCommand() {};

public:
	RfCommand(int commandCode, int commandDuration, Relay& relay)
	{
		_commandCode = commandCode;
		_relay = relay;
		_commandDuration = commandDuration;
	}

	int _commandCode;
	int _commandDuration;
	unsigned long _nextUpdateTime = 0;
	Relay _relay;

	void ResetDuration()
	{
		_nextUpdateTime = 0;
	};

	int getCode()
	{
		return _commandCode;
	};

	int getRelayState()
	{
		return _relay.getState();
	};

	void Update(int rfCode)
	{
		if (_nextUpdateTime < millis())
		{
			_nextUpdateTime = 0;
			_relay.setState(LOW);
		}

		// Return if code does not match
		if (rfCode != _commandCode)
		{
			return;
		}

		if (rfCode == _commandCode && _nextUpdateTime == 0)
		{
			_nextUpdateTime = millis() + _commandDuration;
			_relay.setState(HIGH);
		}
	}

	void reset()
	{
		_relay.setState(LOW);
		_nextUpdateTime = 0;
	}
};

class CommandCoordinator
{
	RfCommand _upCommand;
	RfCommand _downCommand;

	int _upCommandCode = _upCommand.getCode();
	int _downCommandCode = _downCommand.getCode();
	String _direction;

public:
	CommandCoordinator(RfCommand upCommand, RfCommand downCommand)
	{
		_upCommand = upCommand;
		_downCommand = downCommand;
	};

	void resetDuration()
	{
		_upCommand.ResetDuration();
		_downCommand.ResetDuration();
	};

	void update(int rfCode)
	{
		if (rfCode == _upCommand.getCode() && _downCommand.getRelayState() == HIGH)
		{
			_downCommand.reset();
			delay(2000);
			_upCommand.Update(rfCode);
		}
		else if (rfCode == _downCommand.getCode() && _upCommand.getRelayState() == HIGH)
		{
			_upCommand.reset();
			delay(2000);
			_downCommand.Update(rfCode);
		}
		else
		{
			_upCommand.Update(rfCode);
			_downCommand.Update(rfCode);
		}
	}
};


Relay upRelay(12);
Relay downRelay(10);
RfCommand upCommand(1500, 5000, upRelay);
RfCommand downCommand(1600, 5000, downRelay);
CommandCoordinator commandCoordinator(upCommand, downCommand);

int upButtonPin = 2;
int downButtonPin = 4;

int upRelayPinState = LOW;
int downRelayPinState = LOW;

int upButtonPinState = LOW;
int downButtonPinState = LOW;

int rfCodeInput = 0;

int goUpexperimentDone = false;
int goDownexperimentDone = false;


// the setup function runs once when you press reset or power the board
void setup() {
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

	//if (millis() > 4000 && !goDownexperimentDone)
	//{
	//	goDownexperimentDone = true;
	//	rfCodeInput = 1600;
	//}


	/*End of Testing*/

	// Read Input
	upButtonPinState = digitalRead(upButtonPin);
	downButtonPinState = digitalRead(downButtonPin);

	upRelayPinState = upRelay.getState();
	downRelayPinState = downRelay.getState();

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

		if (upButtonPinState == HIGH && downRelayPinState == HIGH)
		{
			downRelay.setState(LOW);
			delay(3000);
			upRelay.setState(HIGH);
		}
		else if (downButtonPinState == HIGH && upRelayPinState == HIGH)
		{
			upRelay.setState(LOW);
			delay(3000);
			downRelay.setState(HIGH);
		}
		else
		{
			upRelay.setState(upButtonPinState);
			downRelay.setState(downButtonPinState);
		}

		commandCoordinator.resetDuration();
	}
	else
	{
		commandCoordinator.update(rfCodeInput);
	}
}

void stopEverything()
{
	upRelay.setState(LOW);
	downRelay.setState(LOW);
}

