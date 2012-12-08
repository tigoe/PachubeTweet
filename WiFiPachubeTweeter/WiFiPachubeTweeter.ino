/*
  WiFi Pachube twitter client
 
 This sketch connects an to Pachube (http://www.pachube.com)
 using an Arduino WiFi shield. It sends a datastream called tweet to
 Pachube so that Pachube will send the tweet on to Twitter.
 
 Circuit:
 * WiFi shield attached to pins 10, 11, 12, 13
 
 created 8 Dec 2012
 by Tom Igoe 
 
 http://www.tigoe.com/pcomp/code/?p=1135
 
 CC-BY-SA Tom Igoe.
 */


// include all Libraries needed:
#include <SPI.h>
#include <WiFi.h>

#define APIKEY         "YOUR_API_KEY_HERE" // your cosm api key
#define FEEDID         00000 // your feed ID
#define USERAGENT      "Pachube Twitterer" // user agent is the project name

char ssid[] = "yourNetwork";      //  your network SSID (name) 
char pass[] = "password";         // your network password

int status = WL_IDLE_STATUS;      // initial status for the WiFi shield
WiFiClient client;                // instance of the Wifi client

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(173,203,98,29);    // numeric IP for api.pachube.com
char server[] = "api.pachube.com";    // name address for pachube API

boolean sendTweet = false;        // whether or not you're ready to send a tweet
String tweet = "";                // String to hold the tweet to be sent
const int tweetMaxLength = 140;   // max length of a tweet

void setup() {
  // start serial port:
  Serial.begin(9600);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 

  // you're connected now, so print out the status:
  printWifiStatus();   
}

void loop() {

  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's incoming serial data, use it to fomulate a tweet.
  // max 140 chars, or when a newline comes in:
  while (Serial.available() > 0) {
    char incomingChar = Serial.read();    // get new bytes

    if (incomingChar == '\n') {           // if you get a newline, send what you have
      sendTweet = true;                    
      break;                              // break out of the while loop
    }

    if (tweet.length() < tweetMaxLength) {   // make sure tweet isn't too long
      tweet += incomingChar;                 // add incoming char to tweet
    } 
    else {                                   // if the tweet's 140 chars:
      sendTweet = true;                      // mark it for sending
    }
  }

  if (sendTweet) {                      // if you're ready to send,
    Serial.println(tweet);              // print so you can see it
    sendData(tweet);                    // send it
    tweet = "";                         // reset the tweet string
    sendTweet = false;                  // you're not ready to send again
  }
}


// this method makes a HTTP connection to the server:
void sendData(String thisData) {
  // JSON string formatted how Pachube likes it:
  String jsonString = "{\"id\":FEEDID,\"datastreams\":[{\"current_value\":\"TWEET\",\"id\":\"tweet\"}]}";

  // replace feed ID and tweet placeholders with actual values:
  jsonString.replace("FEEDID", String(FEEDID));
  jsonString.replace("TWEET", thisData);

  // if you're connected to the server, disconnect:
  if (client.connected()) client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("PUT /v2/feeds/");
    client.print(FEEDID);
    client.println(".json HTTP/1.1");
    client.println("Host: api.cosm.com");
    client.print("X-ApiKey: ");
    client.println(APIKEY);
    client.print("User-Agent: ");
    client.println(USERAGENT);
    client.println("Content-Type:application/x-json");

    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.println(jsonString);
    } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
