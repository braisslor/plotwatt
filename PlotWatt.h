/*
  PlotWatt.h
  
  This library will include ethernet functionality, emon lib functionality, and
  the structure for posting data to plottwatt. I did not write very much of this code myself.
  I've compiled it from different resources. I've attempted to leave their copyright 
  notices alone as best I can.
  
*/
#include <Ethernet.h>
#ifndef PlotWatt_h
#define PlotWatt_h




//These are the raw commands for communication
#define SERVERNAME "plotwatt.com"
#define POSTREADINGS "POST /api/v2/push_readings"
#define LISTMETERS "GET /api/v2/list_meters"
#define DELETEMETER1 "PUT /api/v2/delete_meter"
#define DELETEMETER3 "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nmeter_id="
#define ADDMETERS "POST /api/v2/new_meters"
#define AUTHORIZATION1 " HTTP/1.1\r\nAuthorization: Basic "
#define AUTHORIZATION2 "\r\nHost: plotwatt.com\r\nContent-Length: "
#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else

#include "WProgram.h"

#endif

class PlotWatt {
	public:
		PlotWatt(EthernetClient& aClient, const char* apiKey, int sizeOfArray, long _meterNumber1, long _meterNumber2);	
		PlotWatt(EthernetClient& aClient, const char* apiKey); //Used for listMeters
		void listMeters();
		void addMeter(int _meters);
		void deleteMeter(long _meter);
		void pushData(unsigned long *_timeArray, float *_power1,  float *_power2);
		

		
	private:

		int calculateLength(unsigned long *_object);
		int calculateLength(long _meter);
		int calculateLength( float *_object);
		void readDelay(byte delayTimes);
		//Create a EthernetClient that will perform telnet operations
		EthernetClient _client;	
		const char* _apiKey;
		long _meterNumber1;
		long _meterNumber2;
		int _sizeOfArray;
};




#endif