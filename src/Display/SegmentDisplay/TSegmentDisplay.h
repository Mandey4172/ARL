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
}

#endif


