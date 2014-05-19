#include "PlotWatt.h"
#include <Ethernet.h>
<<<<<<< HEAD
//This is a special DEBUG script I use. Delete to compile.
//#define DEBUG
#include "DebugUtils.h" 
//Delete between here
=======

>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else

#include "WProgram.h"

#endif


PlotWatt::PlotWatt(EthernetClient& aClient, const char* apiKey, int sizeOfArray, long meterNumber1, long meterNumber2) {
	//Take the User Defined inputs from the Arduino sketch and assign to GLOBAL variables
	_client = aClient;
	_apiKey = apiKey;
	_meterNumber1 = meterNumber1;
	_meterNumber2 = meterNumber2;
	_sizeOfArray = sizeOfArray;
}

PlotWatt::PlotWatt(EthernetClient& aClient, const char* apiKey) {
	//Take the User Defined inputs from the Arduino sketch and assign to GLOBAL variables
	_client = aClient;
	_apiKey = apiKey;
}

void PlotWatt::listMeters() {	
	//1. Connect to plotwatt.com
	if (_client.connect(SERVERNAME,80)) {
		//2. Send Initial GET text
<<<<<<< HEAD
		_client.print(LISTMETERS);
		//3. Send Repetitive text
		_client.print(AUTHORIZATION1);
		//4. Send APIKEY 
        _client.print(_apiKey);
		//5. Send remaining text
        _client.print(AUTHORIZATION2);		
=======
		_client.print(LISTMETERS1);
		//3. Send APIKEY 
        _client.print(_apiKey);
		//4. Send remaining text
        _client.print(LISTMETERS2);		
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
		
        readDelay(1); /* wait for a response */  

  } else {
<<<<<<< HEAD
    DEBUG_PRINTLN("Connection Failed in List Meters");
=======
    Serial.println("F1");
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
  }
  _client.stop();

}

void PlotWatt::readDelay(byte delayTimes) {
	int success = 0;
	int isListing = 0;
  if (_client.connected()) {
	for ( byte z = 0; z < delayTimes; z++) {
		unsigned long readDelayTimer = millis();
		while (!_client.available() && success != 'o'+'k') {
		//Do nothing until we receive a response
		  if ( (millis() - readDelayTimer) > 250) {
			break;
		  }
		}
		while (_client.available()) {
			char c = _client.read();
<<<<<<< HEAD
			if ((isListing == '[') && (c != ']')) { DEBUG_PRINT(c); }
			//DEBUG_PRINT(c);
=======
			if ((isListing == '[') && (c != ']')) { Serial.print(c); }
			//Serial.print(c);
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
			//This will check for 'ok' at the end of a successful upload
			if ( (c == 'o') && (success == 0)) {
				success = 'o';
			} else if (( c == 'k' ) && (success == 'o')) {
				success += 'k';
<<<<<<< HEAD
				DEBUG_PRINTLN("Transmission Complete");
=======
				Serial.println("Transmission Complete");
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
			} else if (c == '[' ) {
				isListing = '[';			
			} else if (c == ']') {
				isListing = 0;
			} else {
				success = 0;			
			} 
		}
	}
  }
  else {
<<<<<<< HEAD
	DEBUG_PRINTLN("Connection Failed in ReadDelay");
  }
}

void PlotWatt::pushData(unsigned long *_timeArray, float *_power1, float *_power2) {
=======
	Serial.println("Connection Failed in ReadDelay");
  }
}

void PlotWatt::pushData(float *_power1, unsigned long *_timeArray1,float *_power2, unsigned long *_timeArray2) {
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
	int length = 0;
	//1 Calculate content-length
	length += calculateLength(_meterNumber1) * _sizeOfArray;
	length += calculateLength(_power1);
<<<<<<< HEAD
	length = length + (calculateLength(_timeArray)*2);
	length += calculateLength(_meterNumber2) * _sizeOfArray;
	length += calculateLength(_power2);
=======
	length += calculateLength(_timeArray1);
	length += calculateLength(_meterNumber2) * _sizeOfArray;
	length += calculateLength(_power2);
	length += calculateLength(_timeArray2);
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
 	
	//2. Connect to plotwatt.com
	if (_client.connect(SERVERNAME,80)) {
        //3. Send initial POST command
<<<<<<< HEAD
		_client.print(POSTREADINGS);
		//4. Send Repetitive text
		_client.print(AUTHORIZATION1);
		//5. Send APIKEY 
        _client.print(_apiKey);
		//6. Send remaining text
        _client.print(AUTHORIZATION2);
 		_client.print(length);
		_client.print("\r\n\r\n");		
		//7. Print Meter 1 Data
=======
		_client.print(POSTREADINGS1);
        _client.print(_apiKey);
		_client.print(POSTREADINGS2);
 		_client.print(length);
		_client.print("\r\n\r\n");		
		//4. Print Meter 1 Data
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
		for (int xCounter = 0; xCounter < _sizeOfArray; xCounter ++) {
			_client.print(_meterNumber1);
			_client.print(",");
			_client.print(_power1[xCounter]);
			_client.print(",");
<<<<<<< HEAD
			_client.print(_timeArray[xCounter]);
			_client.print(",");		
		}
		//8. Print Meter 2 Data
=======
			_client.print(_timeArray1[xCounter]);
			_client.print(",");		
		}
		//5. Print Meter 2 Data
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
		for (int xCounter = 0; xCounter < _sizeOfArray; xCounter ++) {
			_client.print(_meterNumber2);
			_client.print(",");
			_client.print(_power2[xCounter]);
			_client.print(",");
<<<<<<< HEAD
			_client.print(_timeArray[xCounter]);
			_client.print(",");
		}	
		_client.print("\r\n");
        readDelay(1); /* wait for a response */  

  } else {
    DEBUG_PRINTLN("Connection Failed in pushData");
=======
			_client.print(_timeArray2[xCounter]);
			_client.print(",");
		}	
		_client.print("\r\n");
        readDelay(4); /* wait for a response */  

  } else {
    Serial.println("Connection Failed in pushData");
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
  }
  _client.stop();

}

//This calculates the total length for all time values
int PlotWatt::calculateLength( unsigned long *_object) {
	int _length = 0;
	for(int yCounter = 0; yCounter < _sizeOfArray; yCounter ++ ) {
		_length += calculateLength(_object[yCounter]);	
	}
	return _length;
}

//This is the primary length calculator. The others simply shape their data
//for this calculation
int PlotWatt::calculateLength(long _meter) {
  int _length = 0;
	if	( _meter > 999999999 )	{ _length +=11; }
	else if	( _meter > 99999999 )	{ _length +=10; }
	else if	( _meter > 9999999 )	{ _length +=9; }
	else if	( _meter > 999999 ) 	{ _length +=8; }
	else if	( _meter > 99999 ) 	{ _length +=7; }
	else if	( _meter > 9999 ) 	{ _length +=6; }
	else if	( _meter > 999 )	{ _length +=5; }
	else if	( _meter > 99 ) 	{ _length +=4; }
	else if	( _meter > 9 ) 		{ _length +=3; }
	else				{ _length +=2; }
	return _length;
}
//This calculates the total length for all kw values
int PlotWatt::calculateLength( float *_object) {
	//I expect emon to return 2 decimal places for a value ie 868.05,523.39,
	//I'm not sure if it will always return 2... ie 7.0 or 7.00
	//I'm going to assume it will always be 2... 7.00
	int _length =0; 
	for(int yCounter = 0; yCounter < _sizeOfArray; yCounter ++ ) {
        _length +=3; //the 2 decimal places and period.
		_length += calculateLength((long)_object[yCounter]);		
	}
        
	return _length;
}

void PlotWatt::deleteMeter(long _meter) {
    int length = 8; // Counts meter_id
	//1. Calculate length
	length+=calculateLength(_meter);
	//2. Connect to plotwatt.com
	if (_client.connect(SERVERNAME,80)) {
	//3. Send text
<<<<<<< HEAD
		_client.print(DELETEMETER1);	
		_client.print(AUTHORIZATION1);
		_client.print(_apiKey);
		_client.print(AUTHORIZATION2);
		_client.print(length);
		_client.print(DELETEMETER3);
		_client.print(_meter);
		_client.print("\r\n");     
        readDelay(1); /* wait for a response */  

  } else {
    DEBUG_PRINTLN("Connection Failed in deleteMeter");
=======
	  _client.print(DELETEMETER1);
          _client.print(_apiKey);
          _client.print(DELETEMETER2);
          _client.print(length);
          _client.print(DELETEMETER3);
		  _client.print(_meter);
		  _client.print("\r\n");     
        readDelay(1); /* wait for a response */  

  } else {
    Serial.println("Connection Failed in deleteMeter");
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
  }
  _client.stop();  
}
void PlotWatt::addMeter(int _meters) {
    int length = 22; // Char count for cmd... will either be 22 or 23
	//1. Calculate Length
	if (_meters > 9) { length ++; }
	//2. Connect to plotwatt.com
	if (_client.connect(SERVERNAME,80)) {
		//3. Send Text
<<<<<<< HEAD
		_client.print(ADDMETERS);
		//4. Send Repetitive text
		_client.print(AUTHORIZATION1);
		//5. Send APIKEY 
        _client.print(_apiKey);
		//6. Send remaining text
        _client.print(AUTHORIZATION2);
=======
		_client.print(ADDMETERS1);
          _client.print(_apiKey);
          _client.print(ADDMETERS2);
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
          _client.print(length);
          _client.print("\r\n\r\n");	
		  _client.print("number_of_new_meters=");
		  _client.print(_meters);
		  _client.print("\r\n");
        readDelay(1); /* wait for a response */  

  } else {
<<<<<<< HEAD
    DEBUG_PRINTLN("Connection Failed in addMeter");
=======
    Serial.println("Connection Failed in addMeter");
>>>>>>> 6f37366bda9a4a533640d6a9fd291813c2bec754
  }
  _client.stop();  
}