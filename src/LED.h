#ifndef Rumcajs_LED_h
#define Rumcajs_LED_h

#include <Arduino.h>
#include "Pin.h"

namespace Rumcajs
{

	class TBaseLED
	{
	public:
		virtual void turnOn() = 0;
		virtual void turnOff() = 0;
	protected:
		const uint8_t _lowState = LOW;
		const uint8_t _hightState = HIGH;
	};

	template <class PinType = DigitalOutputPin>
	class TLED : TBaseLED
	{
	public:
		TLED(const PinType& pin) : _pin(pin) {}

		void turnOn() override
		{
			static_cast<DigitalOutputPin>(_pin).write(_hightState);
		}

		void turnOff() override
		{
			static_cast<DigitalOutputPin>(_pin).write(_lowState);
		}

	private:
		PinType _pin;
	};

	using LED = TLED<DigitalOutputPin>;

	template <class PinType = AnalogOutputPin>
	class TRGBLED : TBaseLED
	{
	public:
		TRGBLED(PinType pins[3]) : _pins(pins) {}

		void turnOn() override
		{
			for(uint8_t i = 0; i < 3; i++)
				static_cast<AnalogOutputPin>(_pins[i]).write(_color[i]);
		}

		void turnOff() override
		{
			for (uint8_t i = 0; i < 3; i++)
				static_cast<AnalogOutputPin>(_pins[i]).write(_lowState);
		}

		void setColor(uint8_t red, uint8_t green, uint8_t blue)
		{
			_color[_red] = red;
			_color[_green] = green;
			_color[_blue] = blue;
		}

	private:
		PinType * _pins;
		uint8_t _color[3];

		const uint8_t _red = 0;
		const uint8_t _green = 1;
		const uint8_t _blue = 2;
	};

	using RGBLED = TRGBLED<AnalogOutputPin>;
}

#endif