#ifndef Rumcajs_Pin_h
#define Rumcajs_Pin_h

#include <Arduino.h>

namespace Rumcajs
{
	enum class PinMode : uint8_t
	{
		Input = INPUT,
		Output = OUTPUT,
	};

	/*
	 *
	 */
	class Pin
	{
	public:
		Pin(const uint8_t port, PinMode mode) : _port(port), _mode(mode)
		{
			pinMode(_port, static_cast<uint8_t>(_mode));
		}

		const uint8_t portNumber()
		{
			return _port;
		}

	protected:
		const uint8_t _port;
		PinMode _mode;
	};

	/*
	 *
	 */
	template<typename writeFunctionType, const writeFunctionType writeFunction>
	class OutputPin : public Pin
	{
	public:
		OutputPin(uint8_t _port) : Pin(_port, PinMode::Output), _writeFunction(writeFunction) {}

		OutputPin& operator=(const Pin& other)
		{
			if (this != &other)
				* this = OutputPin(other);
			return *this;
		}

		void write(uint8_t value)
		{
			_writeFunction(_port, value);
		}

	private:
		const writeFunctionType _writeFunction;
	};

	/*
	 *
	 */
	template<typename readFunctionType, const readFunctionType readFunction>
	class InputPin : public Pin
	{
	public:
		InputPin(uint8_t _port) : Pin(_port, PinMode::Input), _readFunction(readFunction) {}

		uint8_t read()
		{
			return _readFunction(_port);
		}

	private:
		const readFunctionType _readFunction;
	};

	using analogWriteFunctionPtr = void(*)(uint8_t, int);
	class AnalogOutputPin : public OutputPin<analogWriteFunctionPtr,analogWrite>
	{
	public:
		AnalogOutputPin(uint8_t _port) : OutputPin<analogWriteFunctionPtr,analogWrite>(_port) {}
	};

	using digitalWriteFunctionPtr = void(*)(uint8_t, uint8_t);
	class DigitalOutputPin : public OutputPin<digitalWriteFunctionPtr,digitalWrite>
	{
	public:
		DigitalOutputPin(uint8_t _port) : OutputPin<digitalWriteFunctionPtr,digitalWrite>(_port) {}
	};

	using analogReadFunctionPtr = int(*)(uint8_t);
	class AnalogInputPin : public InputPin<analogReadFunctionPtr, analogRead>
	{
	public:
		AnalogInputPin(uint8_t _port) : InputPin<analogReadFunctionPtr, analogRead>(_port) {}
	};

	using digitalReadFunctionPtr = int(*)(uint8_t);
	class DigitalInputPin : public InputPin<digitalReadFunctionPtr,digitalRead>
	{
	public:
		DigitalInputPin(uint8_t _port) : InputPin<digitalReadFunctionPtr,digitalRead>(_port) {}
	};

}

#endif
