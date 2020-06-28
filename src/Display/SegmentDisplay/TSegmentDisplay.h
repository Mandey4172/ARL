#ifndef ARL_TSegmentDisplay_h
#define ARL_TSegmentDisplay_h

#include <src/Display/Core/TDisplay.h>

namespace ARL
{
	template<const uint8_t segmentsCount, const uint8_t displayPinCount>
	class TSegmentDisplay : public TDisplay<DigitalOutputPin, displayPinCount>
	{
	//protected:

	//	constexpr static uint8_t _symbolCount = 10;
	//	uint8_t numberPinSetting[_symbolCount] = {
	//		0b00111111,
	//		0b00000110,
	//		0b01011011,
	//		0b01001111,
	//		0b01100110,
	//		0b01101101,
	//		0b01111101,
	//		0b00000111,
	//		0b01111111,
	//		0b01101111,
	//	};

	public:
		TSegmentDisplay(DigitalOutputPin pins[displayPinCount]) : TDisplay<DigitalOutputPin, displayPinCount>(pins), 
			_segmentsCount(segmentsCount), _displayPinCount(displayPinCount)
		{}

		const uint8_t getSegmentsCount()
		{
			return segmentsCount;
		}

		const uint8_t _segmentsCount;
		const uint8_t _displayPinCount;


	};

	template <typename PinType, const uint8_t digitPinCount, const uint8_t inputPinsCount>
	class TMultiSegmentDisplayBase
	{
	public:

		//static constexpr uint8_t _lowState = LOW;
		//static constexpr uint8_t _hightState = HIGH;
		//static constexpr uint8_t _onState = _lowState;
		//static constexpr uint8_t _offState = _hightState;

	protected:

		PinType* _inputPins;
		PinType* _digitPins;

		bool _dotStates[digitPinCount];

	public:

		TMultiSegmentDisplayBase(PinType digitPins[digitPinCount], PinType segmentPins[inputPinsCount]) : _digitPins(digitPins), _inputPins(segmentPins)
		{
			for (bool& state : _dotStates)
			{
				state = false;
			}
		}

		//virtual void setActiveDigit(uint8_t digit) = 0;
		//virtual void setInputBuffer(uint8_t buffer) = 0;

		static constexpr uint8_t getInputPinsCount()
		{
			return inputPinsCount;
		}

		static constexpr uint8_t getDigitPinsCount()
		{
			return digitPinCount;
		}

		void clearDotState()
		{
			constexpr uint8_t controlPinsCount = getDigitPinsCount();

			for (uint8_t index = 0; index < controlPinsCount; index++)
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
			if (index < getInputPinsCount())
			{
				if (state)
					_inputPins[getInputPinsCount() - 1].write(PinStates::_hightState);
				else
					_inputPins[getInputPinsCount() - 1].write(PinStates::_lowState);
			}
		}

	protected:

		void cleanInputBuffer()
		{
			for (uint8_t i = 0; i < getInputPinsCount(); i++)
				_inputPins[i].write(PinStates::_lowState);
		}

		void cleanDigitBuffer()
		{
			for (int i = 0; i < getDigitPinsCount(); i++)
				_digitPins[i].write(DisplayControlStates::_offState);
		}

	};

	template <const uint8_t digitPinCount, const uint8_t inputPinsCount>
	class TMultiSegmentDisplay : public TMultiSegmentDisplayBase<DigitalOutputPin, digitPinCount, inputPinsCount>
	{
	public:
		TMultiSegmentDisplay(DigitalOutputPin digitPins[digitPinCount], DigitalOutputPin segmentPins[inputPinsCount])
			: TMultiSegmentDisplayBase<DigitalOutputPin, digitPinCount, inputPinsCount>(digitPins, segmentPins)
		{}

	protected:

		void setActiveDigit(uint8_t index)
		{
			this->cleanInputBuffer();										  // won't compile if witout this (Arduino 1.8.10)		
			this->cleanDigitBuffer();

			if (index <= this->getDigitPinsCount())
				this->_digitPins[index].write(DisplayControlStates::_onState);

			delay(2);
		}

		void setInputBuffer(uint8_t input)
		{
			uint8_t mask = 0b00000001;

			for (uint8_t i = 0; i < this->getInputPinsCount(); i++)
			{
				const uint8_t bitValue = (input & mask) >> i;
				this->_inputPins[i].write(bitValue);
				mask = mask << 1;
			}
		}
	};

}

#endif


