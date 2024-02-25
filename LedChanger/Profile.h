#pragma once
#include <string>
#include <stdint.h>


class Profile
{
public:
	std::string color1;
	std::string color2;
	uint8_t lednumber;
	enum FlashMode
	{
		Off,
		Constantly,
		BlinkSlow,
		BlinkFast,
		BlinkUltraFast
	};
	enum ColorMode {
		C1,
		C2,
		C1C2,
		C2C1,
		C1andC2
	};
};

