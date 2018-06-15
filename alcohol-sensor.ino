#include "WiFiEsp.h"
#include "connection_info.h"
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = WIFI_SSID;            // your network SSID (name)
char pass[] = WIFI_PSWD;        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "172.30.1.2";
char url[] = "/api/alcohol";

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 3000)) {
    Serial.println("Connected to server");
    // Make a HTTP request
  }
}

void loop()
{
  
  // if there are incoming bytes available
  // from the server, read them and print them
  int alcohol = readAlcohol();
  
  client.print("GET ");
  client.print(url);
  client.print("/");
  client.print(String(alcohol).c_str());
  client.println(" HTTP/1.1");
  client.println();
   
    
   delay(50);
   while(client.available() <= 20);
   while(client.available()) client.read();


  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    // do nothing forevermore
    while (true);
  }
}

int readAlcohol(){
  int val;
  val = analogRead(0);
  Serial.print("alcohol value: ");
  Serial.println(val, DEC);

  return val;
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


