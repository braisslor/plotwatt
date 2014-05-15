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
#include <PlotWatt.h>

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
#define APIKEY64BIT  "TlRObVlXSTVOVFpoTmpZMDo=" 	// YOUR API KEY GOES HERE
#define METERNUMBER1	906646						// YOUR METER NUMBER GOES HERE 
#define METERNUMBER2	906647						// YOUR METER NUMBER GOES HERE
#define SIZEOFARRAY 10 //1 Samples per second

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


// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xAB, 0xED };
//IPAddress server(192,168,10,109);

unsigned long lastConnectionTime = 0;                // last time we connected to Cosm
float voltage = 123.5;

EthernetClient client;
PlotWatt myPlot(client, APIKEY64BIT, SIZEOFARRAY, METERNUMBER1, METERNUMBER2);

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
  randomSeed(analogRead(3));
  
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
  
  //myPlot.listMeters();
}

void loop() {
  // main program loop
  lastConnectionTime = millis() - lastConnectionTime; //Calculate how many millis it takes to measure A and B
  double Irms1 = emonA.calcIrms(1480);  // Calculate Irms only
  //power1[arrayCounter] = Irms1 * voltage; //Set apparent power in power1
  power1[arrayCounter] = (float)random(500); //Set apparent power in power1
  timeArray1[arrayCounter] = now();  
  double Irms2 = emonB.calcIrms(1480); //Calculate Irms Only
  //power2[arrayCounter] = Irms2 * voltage;
  power2[arrayCounter] = (float)random(500); //Set apparent power in power1
  timeArray2[arrayCounter] = now();
  while((long)power1[arrayCounter] > 50) {
     power1[arrayCounter]/=10.0; 
  }
  while((long)power2[arrayCounter] > 50) {
     power2[arrayCounter]/=10.0; 
  }
  arrayCounter ++;
  
  if (arrayCounter == SIZEOFARRAY) {
	myPlot.pushData(power1, timeArray1, power2, timeArray2);
	arrayCounter = 0; 
  }  
  delay(1000);
  

}

time_t getTime() {
    sendNTPpacket(timeServer);
    delay(1000);
    
    if (Udp.parsePacket() ) {

      Udp.read(packetBuffer, NTP_PACKET_SIZE );
      
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;     
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;  
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



