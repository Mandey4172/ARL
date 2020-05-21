#ifndef ARL_SegmentDisplay_h
#define ARL_SegmentDisplay_h

#include <Arduino.h>
#include <stdio.h>

#include <src/Util.h>

#include <src/Pin.h>
#include <src/Display/Core/TDisplay.h>
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

	class FourDigitSevenSegmentDisplay : public TMultiSegmentDisplay<4, 8>
	{
		static constexpr uint8_t _digitCount = 4;
		static constexpr uint8_t _onState = _lowState;
		static constexpr uint8_t _offState = _hightState;

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

		FourDigitSevenSegmentDisplay(DigitalOutputPin controlPins[4], DigitalOutputPin digitPins[8])
			: TMultiSegmentDisplay<4, 8>(controlPins, digitPins)
		{}

		void display(uint8_t digit, uint8_t input)
		{
			setActiveDigit(digit);
			setSegmentBuffer(input);
		}

		void displayDigit(uint8_t digit, uint8_t input)
		{
			setActiveDigit(digit);
			if (input < _symbolCount)
			{
				setSegmentBuffer(numberPinSetting[input]);
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
				const int reforgedNumber = floor(multipliedNumber);

				clearDotState();
				setDotState(dotPosition, true);
				displayNumber(reforgedNumber);
			}
		}

		void clearDotState()
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();

			for(uint8_t index = 0; index < controlPinsCount; index++)
				setDotState(index, false);
		}

		void setDotState(uint8_t index, bool state)
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();
			if (index < controlPinsCount)
				_dotStates[index] = state;
		}

		void setDotBufferState(uint8_t index, bool state)
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();
			constexpr uint8_t segmentPinCount = getSegmentPinsCount();
			if (index < controlPinsCount)
			{
				if (state)
				{
					_segmentPins[segmentPinCount -1].write(_hightState);
				}
				else
				{
					_segmentPins[segmentPinCount - 1].write(_lowState);
				}
			}
		}

	protected:

		void setActiveDigit(uint8_t index) override
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();

			cleanSegmentBuffer();
			cleanDigitBuffer();

			if (index <= controlPinsCount)
			{
				_digitPins[index].write(_onState);
			}

			delay(2);
		}

		void setSegmentBuffer(uint8_t input) override
		{
			constexpr uint8_t segmentPinCount = getSegmentPinsCount();
			uint8_t mask = 0b00000001;

			for (uint8_t i = 0; i < segmentPinCount; i++)
			{
				const uint8_t bitValue = (input & mask) >> i;
				_segmentPins[i].write(bitValue);
				mask = mask << 1;
			}
		}

		void cleanSegmentBuffer() 
		{
			constexpr uint8_t segmentPinCount = getSegmentPinsCount();

			for (uint8_t i = 0; i < segmentPinCount; i++)
			{
				_segmentPins[i].write(_lowState);
			}
		}

		void cleanDigitBuffer()
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();

			for (int i = 0; i < controlPinsCount; i++)
				_digitPins[i].write(_offState);
		}

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
}


#endif