#ifndef Rumcajs_OneDigitDisplay_h
#define Rumcajs_OneDigitDisplay_h

#include <Arduino.h>
#include <stdio.h>
#include <src/Pin.h>
#include <src/Display/Core/TDisplay.h>

namespace Rumcajs
{
	template<const uint8_t segmentsCount, const uint8_t displayPinCount>
	class TSegmentDisplay : public TDisplay<DigitalOutputPin, displayPinCount>
	{
	public:
		TSegmentDisplay(DigitalOutputPin pins[displayPinCount]) : TDisplay<DigitalOutputPin, displayPinCount>(pins)
		{
		}

		const uint8_t getSegmentsCount()
		{
			return segmentsCount;
		}
	};

	class SevenSegmentDisplay : public TSegmentDisplay<7, 8>
	{
	private:
		constexpr static uint8_t _symbolCount = 10;
		uint8_t numberPinSetting[_symbolCount] = {
			0b00111111,
			0b00000110,
			0b01011011,
			0b01010111,
			0b01100110,
			0b01110101,
			0b01111101,
			0b00000111,
			0b01111111,
			0b01110111,
		};

	public:
		SevenSegmentDisplay(DigitalOutputPin pins[8]) : TSegmentDisplay<7, 8>(pins) {
			Serial.begin(9600);
			Serial.print(_symbolCount);
		}
		//SevenSegmentDisplayt DigitalOutputPin pins[8]) 
			//: TDisplay<DigitalOutputPin, 8>(pins)
		//{}

		void displayDigit(uint8_t number)
		{
			////static_assert(number >= _symbolCount);
			if (number < _symbolCount)
			{
				const uint8_t pinCount = getPinCount();
				uint8_t mask = 0b00000001;

				for (uint8_t i = 0; i < pinCount; i++)
				{
					const uint8_t bitValue = (numberPinSetting[number] & mask) >> i;
					mask = mask << 1;
					_pins[i].write(bitValue);
				}
			}
		}

		void display(uint8_t input)
		{
			const uint8_t pinCount = getPinCount();
			uint8_t mask = 0b00000001;

			for (uint8_t i = 0; i < pinCount; i++)
			{
				const uint8_t bitValue = (input & mask) >> i;
				mask = mask << 1;
				_pins[i].write(bitValue);
			}
		}

	};
}


#endif