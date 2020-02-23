#ifndef Rumcajs_TDigitDisplay_h
#define Rumcajs_TDigitDisplay_h

#include <Arduino.h>
#include <src/Pin.h>

namespace Rumcajs
{
	template <typename PinType, const uint8_t pinCount>
	class TDisplay
	{
	public:
		TDisplay(PinType pins[pinCount]) : _pins(pins)
		{
		}

		const uint8_t getPinCount()
		{
			return pinCount;
		}

	protected:
		PinType * _pins;
		const uint8_t _lowState = LOW;
		const uint8_t _hightState = HIGH;
	};
}



#endif