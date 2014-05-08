 /**
 * Plott Watt Arduino sensor client example.
 *
 * This sketch demonstrates connecting an Arduino to plottwatt.com
 * using ethernet print commands that are equivalent to curl POST 
 *
 * Requirements
 *   * Arduino with Ethernet shield or Arduino Ethernet 
 *   * Arduino software with version >= 1.0
 *   * An account at plottwatt
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

#define APIKEY		**YOUR API KEY GOES HERE***
#define METERNUMBER	**YOUR METER NUMBER GOES HERE***

#define SIZEOFARRAY 30 //1 Samples per second

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
XivelyClient xivelyclient(client);
float powerA[SIZEOFARRAY] = 0.0;
float powerB[SIZEOFARRAY] = 0.0;
unsigned long timeArrayA[SIZEOFARRAY] = 0;
unsigned long timeArrayB[SIZEOFARRAY] = 0;
int arrayCounter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  emonA.current(3, 28.96);             // Current: input pin, calibration.
  emonB.current(4, 28.82);
  
  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }

  Serial.println("Network initialized");
  Serial.println("First Number");
}

void loop() {
  // main program loop
  lastConnectionTime = millis() - lastConnectionTime; //Calculate how many millis it takes to measure A and B
  double IrmsA = emonA.calcIrms(1480);  // Calculate Irms only
  powerA[arrayCounter] = IrmsA * voltage; //Set apparent power in powerA
  timeArrayA[arrayCounter] = now();  
  double IrmsB = emonB.calcIrms(1480); //Calculate Irms Only
  powerB[arrayCounter] = IrmsB * voltage;
  timeArrayB[arrayCounter] = now();
  arrayCounter ++;
  Serial.print("Connection: ");
  Serial.println(lastConnectionTime);
  
  if (arrayCounter == SIZEOFARRAY) {
	arrayCounter = 0;
	sendBatch();
	arrayCounter = 0;
  
  }
  

}

void sendBatch() {
	

}



    
