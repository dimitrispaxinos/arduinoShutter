/*
Name:		ShuttersAttempt.ino
Created:	3/7/2017 8:03:02 PM
Author:	RevilS
*/

#include <RCSwitch.h>

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
		turnOff();
	}

	int getState()
	{
		_state = digitalRead(_pin);
		return _state;
	}

	void turnOff()
	{
		_state = HIGH;
		digitalWrite(_pin, _state);
	}

	void turnOn()
	{
		_state = LOW;
		digitalWrite(_pin, _state);
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
			_relay.turnOff();
		}

		// Return if code does not match
		if (rfCode != _commandCode)
		{
			return;
		}

		if (rfCode == _commandCode && _nextUpdateTime == 0)
		{
			_nextUpdateTime = millis() + _commandDuration;
			_relay.turnOn();
		}
	}

	void reset()
	{
		_relay.turnOff();
		_nextUpdateTime = 0;
	}
};

class CommandCoordinator
{
	RfCommand _upCommand;
	RfCommand _downCommand;

	int _upCommandCode = _upCommand.getCode();
	int _downCommandCode = _downCommand.getCode();
	int _directionSwitchingDuration;
	unsigned long _goDownNextUpdateTime = 0;
	unsigned long _goUpNextUpdateTime = 0;
	int _delayUpCommand = false;
	int _delayDownCommand = false;

public:
	CommandCoordinator() {};

public:
	CommandCoordinator(RfCommand& upCommand, RfCommand& downCommand, int directionSwitchingDuration)
	{
		_upCommand = upCommand;
		_downCommand = downCommand;
		_directionSwitchingDuration = directionSwitchingDuration;
	};

	void resetDuration()
	{
		_upCommand.ResetDuration();
		_downCommand.ResetDuration();
	};

	void update(int rfCode)
	{
		if (_delayUpCommand && _goUpNextUpdateTime < millis())
		{
			_upCommand.Update(_upCommand.getCode());
			_goUpNextUpdateTime = 0;
			_delayUpCommand = false;
			return;
		}

		if (_delayDownCommand && _goDownNextUpdateTime < millis())
		{
			_downCommand.Update(_downCommand.getCode());
			_goDownNextUpdateTime = 0;
			_delayDownCommand = false;
			return;
		}

		if (_delayDownCommand || _delayUpCommand)
			return;

		if (rfCode == _upCommand.getCode())
		{
			if (_downCommand.getRelayState() == LOW)
			{
				_downCommand.reset();
				_goUpNextUpdateTime = _directionSwitchingDuration + millis();
				_delayUpCommand = true;
			}
			else {
				_upCommand.Update(rfCode);
			}
		}
		else if (rfCode == _downCommand.getCode())
		{
			if (_upCommand.getRelayState() == LOW)
			{
				_upCommand.reset();
				_goDownNextUpdateTime = _directionSwitchingDuration + millis();
				_delayDownCommand = true;
			}
			else {
				_downCommand.Update(rfCode);
			}
		}
		else
		{
			_upCommand.Update(rfCode);
			_downCommand.Update(rfCode);
		}
	}
};

class ButtonCoordinator
{
	Relay _upRelay;
	Relay _downRelay;

	int _upButtonPin;
	int _downButtonPin;
	int _upButtonPinState = LOW;
	int _downButtonPinState = LOW;
	int _upRelayPinState = LOW;
	int _downRelayPinState = LOW;

	unsigned long _goDownNextUpdateTime = 0;
	unsigned long _goUpNextUpdateTime = 0;
	int _directionSwitchingDuration;

public:
	ButtonCoordinator() {};

public:
	ButtonCoordinator(int upButtonPin, int downButtonPin, Relay& upRelay, Relay& downRelay, int directionSwitchingDuration)
	{
		_upButtonPin = upButtonPin;
		_downButtonPin = downButtonPin;
		_upRelay = upRelay;
		_downRelay = downRelay;
		_directionSwitchingDuration = directionSwitchingDuration;


	}

	void setup()
	{
		pinMode(_upButtonPin, INPUT_PULLUP);
		pinMode(_downButtonPin, INPUT_PULLUP);
	}

	int Update()
	{
		int _upRelayPinState = _upRelay.getState();
		int _downRelayPinState = _downRelay.getState();

		_upButtonPinState = digitalRead(_upButtonPin);
		_downButtonPinState = digitalRead(_downButtonPin);

		if (_upRelayPinState == LOW && _downRelayPinState == LOW)
		{
			stopEverything();
			return true;
		}

		if (_upButtonPinState == LOW || _downButtonPinState == LOW)
		{
			// If both are pressed, stop everything and return
			if (_upButtonPinState == HIGH && _downButtonPinState == HIGH)
			{
				stopEverything();
				return true;
			}

			if (_upButtonPinState == LOW)
			{
				if (_downRelayPinState == LOW && _goUpNextUpdateTime == 0)
				{
					_goUpNextUpdateTime = _directionSwitchingDuration + millis();
					_downRelay.turnOff();
				}

				if (_goUpNextUpdateTime < millis())
				{
					_goUpNextUpdateTime = 0;
					_upRelay.turnOn();
				}		
			}
			else if (_downButtonPinState == LOW)
			{
				if (_upRelayPinState == LOW && _goDownNextUpdateTime == 0)
				{
					_goDownNextUpdateTime = _directionSwitchingDuration + millis();
					_upRelay.turnOff();
				}

				if (_goDownNextUpdateTime < millis())
				{
					_goDownNextUpdateTime = 0;
					_downRelay.turnOn();
				}
			}
			else
			{
				_upRelay.turnOff();
				_downRelay.turnOff();
			}
			return true;
		}
		return false;
	};

	void stopEverything()
	{
		_upRelay.turnOff();
		_downRelay.turnOff();
	}
};

class ShutterSwitch
{
	CommandCoordinator commandCoordinator;
	ButtonCoordinator buttonCoordinator;
	RCSwitch mySwitch;

public:
	ShutterSwitch() {};

public:
	ShutterSwitch(int upButtonPin,
		int downButtonPin,
		int upRelayPin,
		int downRelayPin,
		int upCCode,
		int downCCode,
		int goUpRfDuration,
		int goDownRfDuration,
		int switchingDuration
	)
	{
		Relay upRelay(upRelayPin);
		Relay downRelay(downRelayPin);
		RfCommand upCommand(upCCode, goUpRfDuration, upRelay);
		RfCommand downCommand(downCCode, goDownRfDuration, downRelay);
		commandCoordinator = CommandCoordinator(upCommand, downCommand, switchingDuration);
		buttonCoordinator = ButtonCoordinator(upButtonPin, downButtonPin, upRelay, downRelay, switchingDuration);
		mySwitch = RCSwitch();
		Serial.begin(9600);
		mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
	}

	int rfCodeInput = 0;

	void setup()
	{
		buttonCoordinator.setup();
	}

	void Loop()
	{
		// Reset every time. If switch is not available, the rfCode stays the same.
		rfCodeInput = 0;

		if (mySwitch.available()) {
			rfCodeInput = mySwitch.getReceivedValue();
			//Serial.println(mySwitch.getReceivedValue());
			mySwitch.resetAvailable();
		}

		if (buttonCoordinator.Update() == true)
		{
			commandCoordinator.resetDuration();
		}
		else
		{
			commandCoordinator.update(rfCodeInput);
		}
	}
};

ShutterSwitch shutterSwitch;

void setup() {

	int upBtnPin = 5;
	int downBtnPin = 6;
	int upRelPin = 12;
	int downRelPin = 10;

	int _upCCode = 5588308;
	int _downCCode = 5588309;
	int _goUpRfDuration = 5000;
	int _goDownRfDuration = 5000;



	shutterSwitch = ShutterSwitch(upBtnPin, downBtnPin, upRelPin, downRelPin, _upCCode, _downCCode, _goUpRfDuration, _goDownRfDuration, 1200);
	shutterSwitch.setup();
}

void loop() {
	shutterSwitch.Loop();
}