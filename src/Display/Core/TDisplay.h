#ifndef ARL_TDisplay_h
#define ARL_TDisplay_h

#include <Arduino.h>
#include <src/Pin.h>

namespace ARL
{
	class DisplayControlStates
	{
	public:
		static constexpr uint8_t _onState = PinStates:: _lowState;
		static constexpr uint8_t _offState = PinStates::_hightState;
	};

	template <typename PinType, const uint8_t pinCount>
	class TDisplay
	{
	public:
		TDisplay(PinType pins[pinCount]) : _pins(pins)
		{
		}

		static constexpr uint8_t getPinCount()
		{
			return pinCount;
		}

	protected:
		PinType * _pins;
	};

}

#endif