#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include<stdio.h>
#include<strsafe.h>
#include "include\hidapi.h"
#include "SimConnect.h"

#define VID 0x231D //vendor id of vkb usb devices
#define PID1 0x3201 //Product id of vkb left version
#define PID2 0x0204 //Product ID of vkb usb controller in question right version
#define REPORT_ID 0x59 //VKB report ID for led control
#define REPORT_COMMAND_PART_ONE 0xA5 //first part of led change command
#define REPORT_COMMAND_PART_TWO 0x0A //second part of led change command
#define RESERVE_HEX 0x00 //for reserved or filler hex
#define NUMBER_OF_LEDS 0x01 //how many leds to configure at once. currently only configured for one at a time
#define LED_NUMBER 0x0A //for choosing the led number to change
#define LED_LEFT_GEAR 0x11 //address of the left gear led
#define LED_MIDDLE_GEAR 0x12 //address of the middle gear led
#define LED_RIGHT_GEAR 0x13 //address of the right gear led
#define RED_LED_DATA_1 0x00//part 1 of 3 for the led data
#define RED_LED_DATA_2 0x06 //part 2 of 3 for the led data
#define RED_LED_DATA_3 0x24 //part 3 of 3 for the led data

#define GREEN_LED_DATA_1 0x38//part 1 of 3 for the led data
#define GREEN_LED_DATA_2 0x00 //part 2 of 3 for the led data
#define GREEN_LED_DATA_3 0x04 //part 3 of 3 for the led data

#define LED_DATA_1 0x38//part 1 of 3 for the led data
#define LED_DATA_2 0x0E //part 2 of 3 for the led data
#define LED_DATA_3 0x4C //part 3 of 3 for the led data



HANDLE hSimConnect = NULL;
int     quit = 0;


struct Struct1
{
	char    title[256];
	double  kohlsmann;
	double  altitude;
	double	gear;
	double  latitude;
	double  longitude;
};

static enum DATA_DEFINE_ID {
	DEFINITION_1,
};


static enum EVENT_ID {
	EVENT_SIM_START,
};


static enum DATA_REQUEST_ID {
	REQUEST_1,
};

void testcode(char data1, char data2, char data3, char lednum);

void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	HRESULT hr;

	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_EVENT:
	{
		SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;

		switch (evt->uEventID)
		{
		case EVENT_SIM_START:

			// Now the sim is running, request information on the user aircraft
			hr = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);

			break;

		default:
			break;
		}
		break;
	}

	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;

		switch (pObjData->dwRequestID)
		{
		case REQUEST_1:
		{
			DWORD ObjectID = pObjData->dwObjectID;
			Struct1* pS = (Struct1*)&pObjData->dwData;
			if (SUCCEEDED(StringCbLengthA(&pS->title[0], sizeof(pS->title), NULL))) // security check
			{
				printf("\nObjectID=%d  Title=\"%s\"\nLat=%f  Lon=%f  Alt=%f  Kohlsman=%.2f Gear Position=%.2f", ObjectID, pS->title, pS->latitude, pS->longitude, pS->altitude, pS->kohlsmann, pS->gear);
				std::cout << std::endl << pS->gear << std::endl;
				if (pS->gear == 1) {
					testcode(GREEN_LED_DATA_1, GREEN_LED_DATA_2, GREEN_LED_DATA_3, LED_LEFT_GEAR);
				}
				else if (pS->gear == 0) {
					testcode(0, 0, 0, LED_LEFT_GEAR);
				}
				else {
					testcode(RED_LED_DATA_1, RED_LED_DATA_2, RED_LED_DATA_3, LED_LEFT_GEAR);
				}
			}
			break;
		}

		default:
			break;
		}
		break;
	}


	case SIMCONNECT_RECV_ID_QUIT:
	{
		quit = 1;
		break;
	}

	default:
		printf("\nReceived:%d", pData->dwID);
		break;
	}
}

void testcode(char data1,char data2, char data3, char lednum) {
	hid_init();
	hid_device_info* firstpointer = hid_enumerate(VID, 0);
	hid_device_info firstdevice = *firstpointer;
	hid_device* device1 = hid_open(VID, PID2, NULL);
	unsigned char report[12];
	report[0] = REPORT_ID;
	report[1] = REPORT_COMMAND_PART_ONE;
	report[2] = REPORT_COMMAND_PART_TWO;
	report[3] = RESERVE_HEX;
	report[4] = RESERVE_HEX;
	report[5] = RESERVE_HEX;
	report[6] = RESERVE_HEX;
	report[7] = NUMBER_OF_LEDS; //end of header for one led chunk
	report[8] = lednum; //led number
	report[9] = data1;
	report[10] = data2;
	report[11] = data3;
	hid_send_feature_report(device1, report, sizeof(report));

	//std::string a = "59A50ABBA2260C0100000624";
	//hid_write(device1,reinterpret_cast<const unsigned char*>(a.c_str()), 12);
	std::cout << std::hex << firstdevice.product_id << std::endl;
	hid_close(device1);
	hid_exit();
	return;
}


void testDataRequest() {
	HRESULT hr;
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Request Data", NULL, 0, 0, 0)))
	{
		printf("\nConnected to Flight Simulator!");

		// Set up the data definition, but do not yet do anything with it
		hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
		hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Kohlsman setting hg", "inHg");
		hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Altitude", "feet");
		hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Gear Position:1","Percent Over 100");
		hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Latitude", "degrees");
		hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Longitude", "degrees");

		// Request an event when the simulation starts
		hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "6Hz");

		printf("\nLaunch a flight.");

		while (0 == quit)
		{
			SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, NULL);
			Sleep(1);
		}

		hr = SimConnect_Close(hSimConnect);
	}

}



int main() {
	std::cout << "Hello World!" << std::endl;
	//testcode(GREEN_LED_DATA_1, GREEN_LED_DATA_2, GREEN_LED_DATA_3, LED_LEFT_GEAR);
	testDataRequest();
	return 0;
}