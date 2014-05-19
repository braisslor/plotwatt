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
<<<<<<< HEAD
#define SERVERNAME "plotwatt.com"
#define POSTREADINGS "POST /api/v2/push_readings"
#define LISTMETERS "GET /api/v2/list_meters"
#define DELETEMETER1 "PUT /api/v2/delete_meter"
#define DELETEMETER3 "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nmeter_id="
#define ADDMETERS "POST /api/v2/new_meters"
#define AUTHORIZATION1 " HTTP/1.1\r\nAuthorization: Basic "
#define AUTHORIZATION2 "\r\nHost: plotwatt.com\r\nContent-Length: "
=======
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
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
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
<<<<<<< HEAD
		void pushData(unsigned long *_timeArray, float *_power1,  float *_power2);
=======
		void pushData(float *_power1, unsigned long *_timeArray1, float *_power2, unsigned long *_timeArray2);
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
		

		
	private:

		int calculateLength(unsigned long *_object);
		int calculateLength(long _meter);
		int calculateLength( float *_object);
<<<<<<< HEAD
=======
		char serverName[];
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
		void readDelay(byte delayTimes);
		//Create a EthernetClient that will perform telnet operations
		EthernetClient _client;	
		const char* _apiKey;
		long _meterNumber1;
		long _meterNumber2;
		int _sizeOfArray;
};




#endif