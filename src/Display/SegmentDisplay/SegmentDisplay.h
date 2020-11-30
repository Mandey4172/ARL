#ifndef ARL_SegmentDisplay_h
#define ARL_SegmentDisplay_h

#include <Arduino.h>
#include <stdio.h>

#include <src/Util.h>

#include <src/Pin.h>
#include <src/Display/SegmentDisplay/TSegmentDisplay.h>

namespace ARL
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
				uint8_t mask = 0b00000001;

				for (uint8_t i = 0; i < getPinCount(); i++)
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

	class FourDigitSevenSegmentDisplay : public TMultiSegmentDisplay<4, 8>
	{
		static constexpr uint8_t _digitCount = 4;

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
			0b01101111 };

	public:

		FourDigitSevenSegmentDisplay(DigitalOutputPin controlPins[4], DigitalOutputPin inputPins[8])
			: TMultiSegmentDisplay<4, 8>(controlPins, inputPins)
		{}


		void displayDigit(uint8_t digit, uint8_t input)
		{
			setActiveDigit(digit);
			if (input < _symbolCount)
			{
				setInputBuffer(numberPinSetting[input]);
				if(_dotStates[digit])
					setDotBufferState(digit, true);
			}
			delay(2);
		}

		void displayNumber(unsigned int number)
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();
			for (uint8_t i = 0; i < controlPinsCount; i++)
			{
				uint8_t digit = static_cast<uint8_t>(stripDigit(number, i));
				if (digit != -1)
					displayDigit(i, digit);
			}
		}

		void displayFloatNumber(float number, uint8_t dotPosition = 1)
		{
			const int max = pow(10, _digitCount - dotPosition) - 1;
			if (number > 0 && number < max)
			{
				const int multiplier = pow(10, dotPosition);
				const float multipliedNumber = number * multiplier;
				const int realNumber = floor(multipliedNumber);

				clearDotState();
				setDotState(dotPosition, true);
				displayNumber(realNumber);
			}
		}

	protected:

		unsigned int stripDigit(unsigned int number, uint8_t digit)
		{
			digit = min(digit, getDigitPinsCount());

			return performStripDigit(number, digit);
		}

		unsigned int performStripDigit(unsigned int number, uint8_t digit)
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();
			if (digit < controlPinsCount)
			{
				for (size_t i = 0; i < digit; i++)
					number /= 10;
				unsigned int value = number % 10;

				return value;
			}
			return -1;
		}

	};

	class BinaryFourDigitSevenSegmentDisplay : public TMultiSegmentDisplay<4, 5>
	{
	public:

		constexpr static char _symbolCount = 10;
		uint8_t numberPinSetting[_symbolCount] = {
			0b0000,
			0b0001,
			0b0010,
			0b0011,
			0b0100,
			0b0101,
			0b0110,
			0b0111,
			0b1000,
			0b1001 };

		BinaryFourDigitSevenSegmentDisplay(DigitalOutputPin controlPins[4], DigitalOutputPin inputPins[5])
			: TMultiSegmentDisplay<4, 5>(controlPins, inputPins)
		{}


		void displayDigit(uint8_t digit, uint8_t input)
		{
			setActiveDigit(digit);
			if (input < _symbolCount)
			{
				setInputBuffer(numberPinSetting[input]);
				if (_dotStates[digit])
					setDotBufferState(digit, true);
			}
			delay(2);
		}
	};
}


#endif