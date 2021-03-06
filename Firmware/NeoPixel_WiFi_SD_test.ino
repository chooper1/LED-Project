#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <WiFiSSLClient.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

/*
   setting up the pin you're connected to on the arduino,
   the number of leds (8 normally, more if you're using daisy chain)
   then initializing the neopixels connected to that pin
*/

#define DELAYVAL 10
#define LED_COUNT 8
#define PIN 6

const int maxBright = 50;

Adafruit_NeoPixel pixels(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
int colors[8][3] = {
    {0, maxBright, 0},  //green
    {maxBright, 0, 0},  //red
    {0, 0, maxBright},  //blue
    {4*maxBright/5, maxBright/2, 0},  //yellow
    {0, maxBright, maxBright},  //turquoise
    {maxBright, 0, maxBright},  //magenta
    {maxBright, 1*maxBright/5, 0}, //orange
    {maxBright, maxBright, 3*maxBright/5} //white
};

//WiFi constants
IPAddress ip(192, 168, 5, 27);// assign IP address to arduino of 192,168,1,88
unsigned int localPort = 5000; // assign a port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //array to hold info to transfer back and forth
WiFiUDP Udp; // define UDP object 
const char* ssid = SSID;    //  your network SSID (name)
const char* password = PASSWORD;  // your network password

/*
   setup runs once and sets up the pixels strip
*/
void setup() 
{
  delay(2000);

  //WiFi and UDP setup
  Serial.print("connecting to ");
  Serial.println(ssid);
  //WiFi.config(ip); // Setting static IP address for the WiFi connection to board
  bool wifiSuccess = WiFi.begin(ssid, password); // initialize wifi
  if (wifiSuccess)
  {
    Serial.println("connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
    Serial.println("failed to connect");
  bool UDPsuccess = Udp.begin(localPort); //initialize Udp
  if (UDPsuccess)
    Serial.println("UDP initialized");
  else
    Serial.println("no socket available");
  delay(1500);
  
  //SD card setup
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
   
  if (!SD.begin(4)) 
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  int storLocs[] = {5,36,59,72,305,613,2547,8321}; //random storage locations
  int pickerIDs[] = {2,5,7,11,13,17,22,24}; //random picker IDs

  //update SD card and provide array lengths to functions
  updateSD("led.txt", updateString(storLocs, sizeof(storLocs)/sizeof(storLocs[0])));
  updateSD("pick.txt", updateString(pickerIDs, sizeof(pickerIDs)/sizeof(pickerIDs[0])));
  
  //NeoPixel setup
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
    pixels.begin();
    pixels.clear();
    pixels.show();
}

/*
   loop repeats forever
*/
void loop()
{
  int packetSize = Udp.parsePacket(); //read the packet size
  
  if(packetSize > 0) //check to see if a request is present
  {
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE); // read the data request on the Udp
    String data(packetBuffer); //convert packet buffer array to string dataReq
    Serial.println(data);
    int comma1 = data.indexOf(',');
    String storLoc = data.substring(0,comma1); //storLoc is first value in comma-separated string
    String pickerID = data.substring(comma1 + 1, data.lastIndexOf(',')); //picker ID is the middle value
    bool on = (data.indexOf("on") > 0); //on is true if string contains "on"
    Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
    Udp.print("Storage location: " + storLoc + '\n' + "Picker ID: " + pickerID + '\n' + "ON: " + on); //Send string back to client
    Udp.endPacket(); // Packet has been sent 

    memset(packetBuffer,0,UDP_TX_PACKET_MAX_SIZE);

    //convert inputs to integers
    int LEDIndex = storLoc_to_LED(storLoc.toInt()); 
    int colorIndex = pickerID_to_RGB(pickerID.toInt());
  
    //update NeoPixel if storage location and picker ID are valid
    if ((LEDIndex != -1) && (colorIndex != -1))
    {
      if (on)
        led_on(LEDIndex, colorIndex); //turn LED at index on with certain color
      else
        led_off(LEDIndex); //turn LED at index off
    }
  }
  delay(DELAYVAL);
}

/*
 * support functions for turning LEDs on and off based on picker ID
 */
void led_on(int LED_IND, int PICKER_COLOR)
{
  pixels.setPixelColor(LED_IND, pixels.Color(colors[PICKER_COLOR][0],
                       colors[PICKER_COLOR][1], colors[PICKER_COLOR][2]));
  pixels.show();
}

/*
 * the led_off function allows a specific led to be turned off without clearing all the other leds
 */
void led_off(int LED_ID)
{
  int led_number = LED_ID;
  pixels.setPixelColor(led_number, pixels.Color(0, 0, 0));
  pixels.show();
}

/*
 * SD card functions
 */

//update a file on the SD card
void updateSD(String file, String newData)
{
  if (SD.exists(file))
    SD.remove(file);
  File updateFile = SD.open(file, FILE_WRITE);
  if (updateFile)
  {
    updateFile.print(newData); //write new data into file
    updateFile.close();
    Serial.println(file + " updated succesfully");
  }
  else 
    Serial.println(file + " not updated");
}

//convert input data array to string to be placed into a file on the SD card
String updateString(int newDataArr[], int arrSize)
{
  String dataString = "";

  //insert tab between storage location and LED index and insert newline between each entry
  for (int i = 0; i < arrSize; i++)
    dataString += (String) newDataArr[i] + '\t' + (String) i + '\n';

  return dataString;
}

//take storage location input and transfer to corresponding LED
int storLoc_to_LED(int storLoc)
{
  File strLoc2LED = SD.open("led.txt"); //open file containing database

  int ledIndex = -1; //if storLoc does not match an entry, function returns -1
  String entry;

  while(strLoc2LED.available())
  {
    char nextChar = '\0';
    while(nextChar != '\n') //end String for one entry at a newline
    {
      entry += (String) nextChar; //add next character to existing String
      nextChar = strLoc2LED.read(); //find next character
    }

    //if a String matches the desired storage location, get the LED index
    if (entry.substring(0, entry.indexOf('\t')) == (String) storLoc)
    {
      ledIndex = entry.substring(entry.indexOf('\t') + 1,entry.indexOf('\n')).toInt(); //LED index is btwn tab and newline
      break; //no need to go through the rest of the file
    }
    entry = "";
  }

  return ledIndex;
}

//take picker ID and convert to colors as defined in NeoPixel object
int pickerID_to_RGB(int pickerID)
{
  File pickID2RGB = SD.open("pick.txt"); //open file containing database

  int colorIndex = -1; //if pickerID does not match an entry, function returns -1
  String entry;

  while(pickID2RGB.available())
  {
    char nextChar = '\0';
    while(nextChar != '\n') //end String for one entry at a newline
    {
      entry += (String) nextChar; //add next character to existing String
      nextChar = pickID2RGB.read(); //find next character
    }

    //if a String matches the pickerID, get color index
    if (entry.substring(0, entry.indexOf('\t')) == (String) pickerID)
    {
      colorIndex = entry.substring(entry.indexOf('\t') + 1,entry.indexOf('\n')).toInt(); //color index is btwn tab and newline
      break; //no need to go through the rest of the file
    }
    entry = "";
  }

  return colorIndex;
}