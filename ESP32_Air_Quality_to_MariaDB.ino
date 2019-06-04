//GOT THE BOARD SETUP FILE FROM https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md
//KEEP THE BOOT BUTTON PRESSED WHILE UPLOADING WITH THE ARDUINO IDE
//BOARD IS A SeeKool ESP32 Dev Board CP2012
/*
  MySQL Connector/Arduino Example : connect by wifi

  This example demonstrates how to connect to a MySQL server from an
  Arduino using an Arduino-compatible Wifi shield. Note that "compatible"
  means it must conform to the Ethernet class library or be a derivative
  thereof. See the documentation located in the /docs folder for more
  details.

  INSTRUCTIONS FOR USE

  1) Change the address of the server to the IP address of the MySQL server
  2) Change the user and password to a valid MySQL user and password
  3) Change the SSID and pass to match your WiFi network
  4) Connect a USB cable to your Arduino
  5) Select the correct board and port
  6) Compile and upload the sketch to your Arduino
  7) Once uploaded, open Serial Monitor (use 115200 speed) and observe

  If you do not see messages indicating you have a connection, refer to the
  manual for troubleshooting tips. The most common issues are the server is
  not accessible from the network or the user name and password is incorrect.

  Created by: Dr. Charles A. Bell
*/

#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>       // Use this for WiFi instead of Ethernet.h
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include "DHT.h"


#define DHTTYPE DHT22
uint8_t DHTPin = 4;
DHT dht(DHTPin, DHTTYPE);  

IPAddress server_addr(192,168,1,198);  // IP of the MySQL *server* here
char user[] = "xxxxxx";              // MySQL user login username
char password[] = "xxxxxxx";        // MySQL user login password



// WiFi card example
char ssid[] = "xxxxxxx";         // your SSID
char pass[] = "xxxxxxxx";     // your SSID Password

WiFiClient client;                 // Use this for WiFi instead of EthernetClient
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

   // Sample query
char INSERT_DATA[] = "INSERT INTO Home_Data.airQuality VALUES (NULL,%s,%s,%s,NULL,NOW(),'1');";
char query[128];
char temp[10];
char humid[10];
char hindex[10];
// create MySQL cursor object
MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

void setup()
{
 Serial.begin(115200);
 pinMode(DHTPin, INPUT);
 dht.begin();  

 // Begin WiFi section
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // print out info about the connection:
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3306, user, password))
    Serial.println("OK.");
  else
    Serial.println("FAILED.");
                
}
 
void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
 
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Heat index: ");
  Serial.println(hic);

if (conn.connected())

dtostrf(t, 5, 2, temp);
dtostrf(h, 5, 2, humid);
dtostrf(hic, 5, 2, hindex);
sprintf(query, INSERT_DATA, temp, humid, hindex); 
cur_mem->execute(query);

                                            //     Serial.println(query); //to see what's actually requested to the db
            
  delay(10000);
 
}
