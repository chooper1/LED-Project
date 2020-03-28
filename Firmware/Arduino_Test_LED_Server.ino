#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
/*
 * Setting up WiFi libraries so that we can receive input from the Python client
 */
#include <SPI.h>
#include <WiFiNINA.h>
# include <WiFiUdp.h>
#include<WiFiSSLClient.h>
#include<arduino_secrets.h>
/*
   setting up the pin you're connected to on the arduino,
   the number of leds (8 normally, more if you're using daisy chain)
   then initializing the neopixels connected to that pin
*/
#define DELAYVAL 500
#define NUMBER_PER_STRIP 8
#define PIN 6

Adafruit_NeoPixel pixels(NUMBER_PER_STRIP, PIN, NEO_GRB + NEO_KHZ800);
int colors[8][3] = {
    {0, 0, 150},
    {0, 150, 0},
    {150, 0, 0},
    {0, 150, 150},
    {150, 0, 150},
    {150, 150, 0},
    {150, 75, 0},
    {0, 150, 75}
};
/*
 * Set up variables related to WiFi communication
 */
IPAddress ip(192, 168, 1, 88);// assign IP address to arduino of 192,168,1,88
unsigned int localPort = 5000; // assign a port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //array to hold info to transfer back and forth
String dataReq; //string for our data
int packetSize; //size of packet
WiFiUDP Udp; // define UDP object 

const char* ssid = SECRET_SSID;    //  your network SSID (name)
const char* password = SECRET_PSWD;  // your network password

void setup() {
  Serial.begin(9600); //turn on serial port
  
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.config(ip); // Setting static IP address for the WiFi connection to board
  WiFi.begin(ssid, password); // initialize wifi
  Udp.begin(localPort); //initialize Udp
  delay(1500);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
    pixels.begin();
    pixels.clear();
}

void loop() {
  packetSize = Udp.parsePacket(); //read the packet size

  if(packetSize>0){ //check to see if a request is present
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE); // read the data request on the Udp
    String dataReq(packetBuffer); //convert packet buffer array to string dataReq

      if (dataReq == "1"){// see if index 1 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 1"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(0);
    }
      if (dataReq == "2"){// see if index 2 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 2"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(1);
    }
      if (dataReq == "3"){// see if index 3 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 3"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(2);
    }
      if (dataReq == "4"){// see if index 4 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 4"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(3);
    }
      if (dataReq == "5"){// see if index 5 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 5"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(4);
    }
      if (dataReq == "6"){// see if index 6 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 6"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(5);
    }
      if (dataReq == "7"){// see if index 7 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 7"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(6);
    }
      if (dataReq == "8"){// see if index 8 was requested
      Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
      Udp.print("You want to turn on address 8"); //Send string back to client
      Udp.endPacket(); // Packet has been sent
        for (int i = 0; i < NUMBER_PER_STRIP; i++) {
        pixels.clear();
        }
      led_on(7);
    }
    memset(packetBuffer,0,UDP_TX_PACKET_MAX_SIZE);
    }
}

/*
 * support functions for turning LEDs on and off based on picker ID
 */
void led_on(int PICKER_ID)
{
  int led_number = PICKER_ID;
  pixels.setPixelColor(led_number, pixels.Color(colors[2][0],
                       colors[2][1], colors[2][2]));
  pixels.show();
  delay(5);
}

/*
 * the led_off function allows a specific led to be turned off without clearing all the other leds
 */
void led_off(int PICKER_ID)
{
  int led_number = PICKER_ID;
  pixels.setPixelColor(led_number, pixels.Color(0, 0, 0));
  pixels.show();
  delay(5);
}
