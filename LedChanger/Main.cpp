#include <iostream>
#include <string>
#include "include\hidapi.h"

#define REPORT_ID 0x59 //VKB report ID for led control
#define REPORT_COMMAND_PART_ONE 0xA5 //first part of led change command
#define REPORT_COMMAND_PART_TWO 0x0A //second part of led change command
#define RESERVE_HEX 0x00 //for reserved or filler hex
#define NUMBER_OF_LEDS 0x01 //how many leds to configure at once. currently only configured for one at a time
#define LED_NUMBER 0x0A //for choosing the led number to change
#define LED_DATA_1 0x00 //part 1 of 3 for the led data
#define LED_DATA_2 0x06 //part 2 of 3 for the led data
#define LED_DATA_3 0x24 //part 3 of 3 for the led data

int main(){
	std::cout << "Hello World!" << std::endl;
	hid_init();
	hid_device_info* firstpointer = hid_enumerate(0x231D, 0);
	hid_device_info firstdevice = *firstpointer;
	hid_device* device1 = hid_open(0x231D,firstdevice.product_id,NULL);
	unsigned char report[12];
	report[0] = REPORT_ID;
	report[1] = REPORT_COMMAND_PART_ONE;
	report[2] = REPORT_COMMAND_PART_TWO;
	report[3] = RESERVE_HEX;
	report[4] = RESERVE_HEX;
	report[5] = RESERVE_HEX;
	report[6] = RESERVE_HEX;
	report[7] = NUMBER_OF_LEDS; //end of header for one led chunk
	report[8] = LED_NUMBER; //led number
	report[9] = LED_DATA_1;
	report[10] = LED_DATA_2;
	report[11] = LED_DATA_3;
	hid_send_feature_report(device1, report, sizeof(report));

	//std::string a = "59A50ABBA2260C0100000624";
	//hid_write(device1,reinterpret_cast<const unsigned char*>(a.c_str()), 12);
	std::cout << std::hex << firstdevice.product_id << std::endl;
	hid_close(device1);
	hid_exit();
	return 0;
}