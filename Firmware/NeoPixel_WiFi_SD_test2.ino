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

const int maxBright = 20;

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
const char* ssid = "NIOT";    //  your network SSID (name)
const char* password = "F62733E70793EBA609F20D5";  // your network password

/*
   setup runs once and sets up the pixels strip
*/
void setup() 
{
  delay(2000);
  
  //WiFi and UDP setup

  boolean wifiSuccess = false;
  IPAddress ip_NULL(0,0,0,0);
  while(!wifiSuccess)
  {
    Serial.print("connecting to ");
    Serial.println(ssid);
    //WiFi.config(ip); // Setting static IP address for the WiFi connection to board
    wifiSuccess = WiFi.begin(ssid, password); // initialize wifi
    if (wifiSuccess && WiFi.localIP() != ip_NULL)
       break;
    Serial.println("failed to connect");
  }
  Serial.println("connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
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

  if (SD.exists("pick.txt"))
    SD.remove("pick.txt");
/*
  int storLocs[] = {5,36,59,72,305,613,2547,8321}; //random storage locations
  int pickerIDs[] = {2,5,7,11,13,17,22,24}; //random picker IDs

  //update SD card and provide array lengths to functions
  updateSD("led.txt", updateString(storLocs, sizeof(storLocs)/sizeof(storLocs[0])));
  updateSD("pick.txt", updateString(pickerIDs, sizeof(pickerIDs)/sizeof(pickerIDs[0])));
*/
 
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
    int colorIndex = pickerID_to_RGB(pickerID.toInt(), on);
  
    //update NeoPixel if storage location and picker ID are valid
    if (LEDIndex != -1)
    {
      if (on)
      {
        if (colorIndex != -1)
          led_on(LEDIndex, colorIndex); //turn LED at index on with certain color
      }
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
int pickerID_to_RGB(int pickerID, bool on)
{
  int colorIndex = -1; //if pickerID does not match an entry, function returns -1
  int recNum = 0; //counter for how many records are present

  //update record counter
  File pickID2RGB = SD.open("pick.txt"); //open file containing database
  while(pickID2RGB.available())
  {
    char nextChar = '\0';
    while(nextChar != '\n' && nextChar != 255) //end String for one entry at a newline
      nextChar = pickID2RGB.read(); //find next character
    recNum++;
  }
  pickID2RGB.close();

  //if recNum is nonzero, check to see if new pickerID is already present
  //if present, light up LED with specified color
  if (recNum > 0)
  {
    pickID2RGB = SD.open("pick.txt");
    String entry;
  
    while(pickID2RGB.available())
    {
      char nextChar = '\0';
      while(nextChar != '\n' && nextChar != 255) //end String for one entry at a newline
      {
        entry += (String) nextChar; //add next character to existing String
        nextChar = pickID2RGB.read(); //find next character
      }
      entry += '\n';
      
      //if a String matches the pickerID, get color index
      if (entry.substring(0, entry.indexOf('\t')) == (String) pickerID)
      {
        colorIndex = entry.substring(entry.indexOf('\t') + 1,entry.indexOf('\n')).toInt(); //color index is btwn tab and newline
        break; //no need to go through the rest of the file
      }
      entry = "";
    }
    pickID2RGB.close();
  }

  //if either file is empty or new pickerID is not present in the database, assign new picker new color
  if (colorIndex == -1 && on)
  {
    //if the database is not at capacity, append new entry to the database
    if (recNum < 8)
    {
      pickID2RGB = SD.open("pick.txt");
      String updateString;
      while(pickID2RGB.available())
      {
        String entry;
        char nextChar = '\0';
        while(nextChar != '\n' && nextChar != 255) //end String for one entry at a newline
        {
          entry += (String) nextChar; //add next character to existing String
          nextChar = pickID2RGB.read(); //find next character
        }
        entry += '\n';
        updateString += entry;
      }
      pickID2RGB.close();
      updateString += (String) pickerID + '\t' + (String) (recNum) + '\n';  //assign picker a color index one above the number of entries
      colorIndex = recNum; //set LED output color to assigned color
      updateSD("pick.txt", updateString);
    }
    //if database is too full for new entry, cycle out least recent entry
    else if (recNum >= 8)
    {
      pickID2RGB = SD.open("pick.txt");
      String wholeString;
      String entry;
      //get whole database
      while(pickID2RGB.available())
      {
        char nextChar = '\0';
        while(nextChar != '\n' && nextChar != 255) //end String for one entry at a newline
        {
          entry += (String) nextChar; //add next character to existing String
          nextChar = pickID2RGB.read(); //find next character
        }
        entry += "\n";
        wholeString += entry;
        entry = "";
      }
      pickID2RGB.close();
      
      int lastTab = wholeString.lastIndexOf('\t') + 1;  //index of last tab
      int prevIndex = wholeString.substring(lastTab, wholeString.indexOf('\n',lastTab)).toInt();  //last color index in database
      colorIndex = (prevIndex + 1) % 8; //assign picker to the next color index, returning to index 0 after index 7
      String updateString = wholeString.substring(wholeString.indexOf('\n')+1) + (String) pickerID + '\t' + (String) colorIndex;
      updateSD("pick.txt", updateString); //send new string to SD card
    }
  }
  return colorIndex;
}