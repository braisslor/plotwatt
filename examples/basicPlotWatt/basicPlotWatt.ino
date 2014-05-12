 /**
 * Plot Watt Arduino sensor client example.
 *
 * This sketch demonstrates connecting an Arduino to plotwatt.com
 * using ethernet print commands that are equivalent to curl POST 
 *
 * Requirements
 *   * Arduino with Ethernet shield or Arduino Ethernet 
 *   * Arduino software with version >= 1.0
 *   * An account at plotwatt
 *   * This sketch requires the Time library from http://www.pjrc.com/teensy/td_libs_Time.html 
 *	 * An analog sensor connected to measure current
 *
 * Optional
 *   * An analog sensor connected
 *   * I'm using a powerline ethernet adapter
 *
 */

#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <EmonLib.h>                   // Include Emon Library
#include <Time.h>
#include <EthernetUdp.h> //for UDP NTP client
#include <MemoryFree.h>

/***************************************
Your api key is from www.plotwatt.com. 
1. Go to https://plotwatt.com/docs/api
2. Your api key is listed there. EX: NTNmYWI5NTZhNjY0
3. Change prefered language bullet to "Low-level details with Curl examples"
4. Copy the text string between 'http://' and the '@plotwatt.com/api/v2/new_meters' 
	- http://NTNmYWI5NTZhNjY0:@plotwatt.com/api/v2/new_meters 
	- String is: NTNmYWI5NTZhNjY0:
5. Now go to http://www.base64encode.org/ and paste the string into the 64 bit encoder.
6. This is the APIKEY64BIT for the listed api key. TlRObVlXSTVOVFpoTmpZMDo=


Your Meter Numbers can be found viewing the Trends page on plotwatt.com
Where the page allows you to choose a Meter, that number is your meter number.
 **************************************/
 
#define APIKEY64BIT		TlRObVlXSTVOVFpoTmpZMDo= 	// YOUR API KEY GOES HERE
#define METERNUMBER1	904090						// YOUR METER NUMBER GOES HERE 
#define METERNUMBER2	904111						// YOUR METER NUMBER GOES HERE
#define SIZEOFARRAY 30 //1 Samples per second


//For UDP NTP Client
const unsigned int LOCALPORT = 8888;      // local port to listen for UDP packets

IPAddress timeServer(192,53,103,108); // time.nist.gov NTP server

const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
unsigned long UDPTime = 0;
//End the UDP Client Packets

EnergyMonitor emonA;                   // Create an instance for current sensor A
EnergyMonitor emonB;                   // Create an instance for current sensor B

unsigned long xTime= 0;

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xAB, 0xED };
//IPAddress server(192,168,10,109);

unsigned long lastConnectionTime = 0;                // last time we connected to Cosm
unsigned long doorOpenedTime = 0;                  //This is the time the garage door goes open
const unsigned long connectionInterval = 500;      // delay between connecting to Cosm in milliseconds
float voltage = 123.5;

EthernetClient client;
byte plotWattIP[] = {192, 168, 10, 5};
float power1[SIZEOFARRAY];
float power2[SIZEOFARRAY] ;
unsigned long timeArray1[SIZEOFARRAY];
unsigned long timeArray2[SIZEOFARRAY];
int arrayCounter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  emonA.current(3, 28.96);             // Current: input pin, calibration.
  emonB.current(4, 28.82);
  
  for (arrayCounter = 0; arrayCounter < SIZEOFARRAY; arrayCounter ++ ) {
	power1[arrayCounter] = 0.0;
	power2[arrayCounter] = 0.0;
	timeArray1[arrayCounter] = 0.0;
	timeArray2[arrayCounter] = 0.0;
  }
  arrayCounter = 0;
  
  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }

  Serial.println("Network initialized");
  //This is obviously for the UDP NTP
  Udp.begin(LOCALPORT);
  setSyncProvider( getTime );
  Serial.print("Set Sync: ");
  Serial.println(getTime());
  digitalClockDisplay();
}

void loop() {
  // main program loop
  lastConnectionTime = millis() - lastConnectionTime; //Calculate how many millis it takes to measure A and B
  double Irms1 = emonA.calcIrms(1480);  // Calculate Irms only
  power1[arrayCounter] = Irms1 * voltage; //Set apparent power in power1
  timeArray1[arrayCounter] = now();  
  double Irms2 = emonB.calcIrms(1480); //Calculate Irms Only
  power2[arrayCounter] = Irms2 * voltage;
  timeArray2[arrayCounter] = now();
  arrayCounter ++;
  Serial.print("Connection: ");
  Serial.println(lastConnectionTime);
  
  if (arrayCounter == SIZEOFARRAY) {
	arrayCounter = 0;
	sendBatch(SIZEOFARRAY, (long)METERNUMBER1, power1, timeArray1, (long)METERNUMBER2, power2, timeArray2);
	arrayCounter = 0;
  
  }  
  

}

void sendBatch(const int _SIZEOFARRAY, long _meterNumber1, float *_power1, unsigned long *_timeArray1, long _meterNumber2, float *_power2, unsigned long *_timeArray2) {
	int length = 0;
	//1.0 Calculate content-length
	length += calculateLength(_meterNumber1) * _SIZEOFARRAY;
	length += calculateLength(_power1);
	length += calculateLength(_timeArray1);
	length += calculateLength(_meterNumber2) * _SIZEOFARRAY;
	length += calculateLength(_power2);
	length += calculateLength(_timeArray2);	
	length --; //Subtract 1 for the last comma that was counted but not there. 
	
	
	//2. Connect to plotwatt.com
	Serial.println("connecting...");
	if (client.connect(plotWattIP,80)) {
        
        client.print(POSTFANDATA);
        readDelay(4); /* wait for a response */  
		client.print(FANON);
		readDelay(4); /* wait for a response */
    
  } else {
    Serial.println("F1");
  }
  _client.stop();
  if ( success == 255) {
		return -1;
	} else if ( success < 255) {
	byte tempSuccess = success;
	success = 255;
	return tempSuccess;
	}
	//2. Send initial POST command	-> 
	
	// Example Post command 
	//POST /api/v2/push_readings HTTP/1.1
	//Authorization: Basic TlRObVlXSTVOVFpoTmpZMDo=
	//User-Agent: curl/7.30.0
	//Host: plotwatt.com
	//Accept: */*
	//Content-Length: 46
	//Content-Type: application/x-www-form-urlencoded
	
	//904090,10.5,1399654557,904090,40.45,1399654597
	

}

time_t getTime() {
    sendNTPpacket(timeServer);
    //Serial.println("Delaying in Sync");
    delay(1000);
    
    if (Udp.parsePacket() ) {
      //Serial.println("UDP Parsed");
      Udp.read(packetBuffer, NTP_PACKET_SIZE );
      
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    //Serial.print("Seconds since Jan 1 1900 = " );
    //Serial.println(secsSince1900);               

    // now convert NTP time into everyday time:
    //Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;     
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;  
    // print Unix time:
    //Serial.println(epoch);
    UDPTime = millis();
    return epoch;
  }
}

unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp: 		   
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" freeMemory()=");
  Serial.println(freeMemory()); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10) {
    Serial.print('0');
  }
    Serial.print(digits);
  
}

int calculateLength( unsigned long *_object) {
	int _length = 0;
	for (arrayCounter = 0; arrayCounter < SIZEOFARRAY; arrayCounter ++ ) {
		if		( _object[arrayCounter] > 999999999 )	{ _length +=11; }
		else if	( _object[arrayCounter] > 99999999 )	{ _length +=10; }
		else if	( _object[arrayCounter] > 9999999 )		{ _length +=9; }
		else if	( _object[arrayCounter] > 999999 ) 		{ _length +=8; }
		else if	( _object[arrayCounter] > 99999 ) 		{ _length +=7; }
		else if	( _object[arrayCounter] > 9999 ) 		{ _length +=6; }
		else if	( _object[arrayCounter] > 999 )			{ _length +=5; }
		else if	( _object[arrayCounter] > 99 ) 			{ _length +=4; }
		else if	( _object[arrayCounter] > 9 ) 			{ _length +=3; }
		else											{ _length +=2; }			
	}
	return _length;
}
int calculateLength(long _meter) {
		if		( _meter > 999999999 )	{ _length +=11; }
		else if	( _meter > 99999999 )	{ _length +=10; }
		else if	( _meter > 9999999 )	{ _length +=9; }
		else if	( _meter > 999999 ) 	{ _length +=8; }
		else if	( _meter > 99999 ) 		{ _length +=7; }
		else if	( _meter > 9999 ) 		{ _length +=6; }
		else if	( _meter > 999 )		{ _length +=5; }
		else if	( _meter > 99 ) 		{ _length +=4; }
		else if	( _meter > 9 ) 			{ _length +=3; }
		else							{ _length +=2; }
	}
	return _length;
}

int calculateLength( float *_object) {

}