 /**
 * Plot Watt Arduino List Meters client example.
 *
 * This sketch demonstrates connecting an Arduino to plotwatt.com
 * using ethernet print commands that will list all existing meters with 
 * your plotwatt API KEY that has been converted into a 64 BIT encoded key.
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
#define APIKEY64BIT   "TlRObVlXSTVOVFpoTmpZMDo=" 	// YOUR API KEY GOES HERE


long metersToDelete[] = { 907711,907712,907713,907714,907715,907716,907717,907718,907719,907720,907722,907723,907724,907725,907726,907728,907729,907730,907731,907732,907733,907734,907735,907736,907737,907738,907740,907741,907742,907743,907744,907745};
// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xAB, 0xED };
//IPAddress server(192,168,10,109);


EthernetClient client;
PlotWatt myPlot(client, APIKEY64BIT);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }

}

void loop() {
	Serial.print("The meters are: ");
	myPlot.listMeters();
	for ( int counter = 0; counter < sizeof(metersToDelete) -1; counter ++ ) {
		myPlot.deleteMeter(metersToDelete[counter]);
		Serial.println();
	}
	myPlot.listMeters();
	while(1);
}
