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
#define SERVERNAME "www.plotwatt.com"
#define POSTREADINGS1 "POST /api/v2/push_readings HTTP/1.1\r\nAuthorization: Basic "
#define POSTREADINGS2 "\r\nHost: plotwatt.com\r\nContent-Length: "
#define LISTMETERS1 "GET /api/v2/list_meters HTTP/1.1\r\nAuthorization: Basic "
#define LISTMETERS2 "\r\nHost: plotwatt.com\r\n\r\n"
#define DELETEMETER1 "PUT /api/v2/delete_meter HTTP/1.1\r\nAuthorization: Basic "
#define DELETEMETER2 "\r\nHost: plotwatt.com\r\nContent-Length: "
#define DELETEMETER3 "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nmeter_id="
#define ADDMETERS1 "POST /api/v2/new_meters HTTP/1.1\r\nAuthorization: Basic "
#define ADDMETERS2 "\r\nHost: plotwatt.com\r\nContent-Length: "
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
		void pushData(float *_power1, unsigned long *_timeArray1, float *_power2, unsigned long *_timeArray2);
		

		
	private:

		int calculateLength(unsigned long *_object);
		int calculateLength(long _meter);
		int calculateLength( float *_object);
		char serverName[];
		void readDelay(byte delayTimes);
		//Create a EthernetClient that will perform telnet operations
		EthernetClient _client;	
		const char* _apiKey;
		long _meterNumber1;
		long _meterNumber2;
		int _sizeOfArray;
};




#endif