#pragma once
#include <string>
#include <cstdint>

#define Data_Size 4 //temporary size for LED Profile of 4 bytes
#define LEDDATA_Byte_4 3 //fourth array byte for leddata
#define ColorMode_Bit_Wipe 0x1F //for an and bitmask wipe of the first three bits
#define ColorMode_Bit_Shift 5 //shift bitmask by 5 bits to right location


class Profile
{
public:
	enum FlashMode : uint8_t
	{
		Off,
		Constantly,
		BlinkSlow,
		BlinkFast,
		BlinkUltraFast
	};
	enum ColorMode : uint8_t {
		C1,
		C2,
		C1C2,
		C2C1,
		C1andC2
	};
	uint32_t getprofile() {

	}
	int SetColorMode(ColorMode Modeset);
	int SetFlashMode(FlashMode Flashset);
	int SetColor1(std::string Color1);
	int SetColor2(std::string Color2);
	int SetLedNumber(uint8_t LEDNumber);
private:
	uint8_t LEDData[Data_Size];
};

