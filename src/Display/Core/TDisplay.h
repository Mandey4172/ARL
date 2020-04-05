#ifndef Rumcajs_TDisplay_h
#define Rumcajs_TDisplay_h

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

		static constexpr uint8_t getPinCount()
		{
			return pinCount;
		}

	protected:
		PinType * _pins;
		static constexpr uint8_t _lowState = LOW;
		static constexpr uint8_t _hightState = HIGH;
	};

	template <typename PinType, const uint8_t digitpinCount, const uint8_t segmentPinCount>
	class TMultiSegmentDisplay
	{
	public:
		TMultiSegmentDisplay(PinType digitPins[digitpinCount], PinType segmentPins[segmentPinCount]) : _digitPins(digitPins), _segmentPins(segmentPins)
		{
		}

		static constexpr uint8_t getSegmentPinsCount()
		{
			return segmentPinCount;
		}

		static constexpr uint8_t getDigitPinsCount()
		{
			return digitpinCount;
		}

		virtual void SetActualDigit(uint8_t digit) = 0;
		virtual void SetActualDigitSegments(uint8_t digit) = 0;

	protected:

		PinType* _segmentPins;
		PinType* _digitPins;

		static constexpr uint8_t _lowState = LOW;
		static constexpr uint8_t _hightState = HIGH;
	};
}

#endif