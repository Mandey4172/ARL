#ifndef Rumcajs_TSegmentDisplay_h
#define Rumcajs_TSegmentDisplay_h

namespace Rumcajs
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

	template <typename PinType, const uint8_t digitPinCount, const uint8_t segmentPinCount>
	class TMultiSegmentDisplayBase
	{
	public:
		TMultiSegmentDisplayBase(PinType digitPins[digitPinCount], PinType segmentPins[segmentPinCount]) : _digitPins(digitPins), _segmentPins(segmentPins)
		{
			for (bool& state : _dotStates)
			{
				state = false;
			}
		}

		static constexpr uint8_t getSegmentPinsCount()
		{
			return segmentPinCount;
		}

		static constexpr uint8_t getDigitPinsCount()
		{
			return digitPinCount;
		}

		virtual void setActiveDigit(uint8_t digit) = 0;
		virtual void setSegmentBuffer(uint8_t digit) = 0;

	protected:

		PinType* _segmentPins;
		PinType* _digitPins;

		bool _dotStates[digitPinCount];

		static constexpr uint8_t _lowState = LOW;
		static constexpr uint8_t _hightState = HIGH;
	};

	template <const uint8_t digitPinCount, const uint8_t segmentPinCount>
	class TMultiSegmentDisplay : public TMultiSegmentDisplayBase<DigitalOutputPin, digitPinCount, segmentPinCount>
	{
	public:
		TMultiSegmentDisplay(DigitalOutputPin digitPins[digitPinCount], DigitalOutputPin segmentPins[segmentPinCount]) 
			: TMultiSegmentDisplayBase<DigitalOutputPin, digitPinCount, segmentPinCount>(digitPins,segmentPins)
		{}
	};


}

#endif


