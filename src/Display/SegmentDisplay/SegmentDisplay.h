#ifndef Rumcajs_SegmentDisplay_h
#define Rumcajs_SegmentDisplay_h

#include <Arduino.h>
#include <stdio.h>
#include <src/Pin.h>
#include <src/Display/Core/TDisplay.h>
#include <src/Display/SegmentDisplay/TSegmentDisplay.h>

namespace Rumcajs
{
	class SevenSegmentDisplay : public TSegmentDisplay<7, 8>
	{
	protected:

		constexpr static uint8_t _symbolCount = 10;
		uint8_t numberPinSetting[_symbolCount] = {
			0b00111111,
			0b00000110,
			0b01011011,
			0b01001111,
			0b01100110,
			0b01101101,
			0b01111101,
			0b00000111,
			0b01111111,
			0b01101111,
		};

	public:
		SevenSegmentDisplay(DigitalOutputPin pins[8]) : TSegmentDisplay<7, 8>(pins) {}

		void displayDigit(uint8_t number)
		{
			if (number < _symbolCount)
			{
				const uint8_t pinCount = getPinCount();
				uint8_t mask = 0b00000001;

				for (uint8_t i = 0; i < pinCount; i++)
				{
					const uint8_t bitValue = (numberPinSetting[number] & mask) >> i;
					_pins[i].write(bitValue);
					mask = mask << 1;
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
				_pins[i].write(bitValue);
				mask = mask << 1;
			}
		}

	};

	class FourDigitSevenSegmentDisplay : public SevenSegmentDisplay
	{
		static constexpr uint8_t _onState = _lowState;
		static constexpr uint8_t _offState = _hightState;
		static constexpr uint8_t _controlPinsCount = 4;

	public:
		FourDigitSevenSegmentDisplay(DigitalOutputPin controlPins[_controlPinsCount], DigitalOutputPin digitPins[8])
			: _controlPins(controlPins), SevenSegmentDisplay(digitPins)
		{}

		void displayDigitOn(uint8_t digit, uint8_t number)
		{
			setActualDigitSetup(digit);
			displayDigit(number);
		}

		void displayOn(uint8_t digit, uint8_t input)
		{
			setActualDigitSetup(digit);
			display(input);
		}

	private:

		void setActualDigitSetup(uint8_t index)
		{
			if (index <= _controlPinsCount)
			{
				for (int i = 0; i < _controlPinsCount; i++)
					_controlPins[i].write(_offState);

				_controlPins[index].write(_onState);
			}
		}

		DigitalOutputPin * _controlPins;
	};
}


#endif