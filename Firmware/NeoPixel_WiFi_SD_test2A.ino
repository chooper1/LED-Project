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
int colors[9][3] = {
    {0, maxBright, 0},  //green
    {maxBright, 0, 0},  //red
    {0, 0, maxBright},  //blue
    {4*maxBright/5, maxBright/2, 0},  //yellow
    {0, maxBright, maxBright},  //turquoise
    {maxBright, 0, maxBright},  //magenta
    {maxBright, maxBright/5, 0}, //orange
    {maxBright, maxBright, 3*maxBright/5} //white
};

//WiFi constants
IPAddress ip(192, 168, 5, 127);// assign IP address to arduino of 192,168,5,127
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

  if (SD.exists("pick.txt"))
    SD.remove("pick.txt");
  
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
    int comma2 = data.indexOf(',', comma1 + 1);
    String storLoc = data.substring(0,comma1);
    String pickerID = data.substring(comma1 + 1, comma2);
    String nextStorLoc = data.substring(comma2 + 1, data.indexOf(',',comma2 + 1));
    bool on = (data.indexOf("on") > 0);
    Udp.beginPacket(Udp.remoteIP(),Udp.remotePort()); //Initialize packet Where we are sending it. Probably don't need this for my code
    Udp.print("Storage location: " + storLoc + '\n' + "Picker ID: " + pickerID + '\n' + "Next storage location: " + nextStorLoc + '\n' + "ON: " + on); //Send string back to client
    Udp.endPacket(); // Packet has been sent 

    memset(packetBuffer,0,UDP_TX_PACKET_MAX_SIZE);

    //convert inputs to integers
    int LEDIndex = storLoc_to_LED(storLoc.toInt()); 
    int colorIndex;
    if (LEDIndex >= 0)
      colorIndex = pickerID_to_RGB(pickerID.toInt(), nextStorLoc.toInt(), on);
  
    //update NeoPixel if storage location and picker ID are valid
    if ((LEDIndex != -1) && (colorIndex != -1))
    {
      if (on)
        led_on(LEDIndex, colorIndex);
      else
        led_off(LEDIndex);
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
  strLoc2LED.close(); //close the database file
  return ledIndex;
}

//take picker ID and convert to colors as defined in NeoPixel object
int pickerID_to_RGB(int pickerID, int nextStorLoc, bool on)
{
  int colorIndex = -1;
  File pickID2RGB = SD.open("pick.txt"); //open file containing database
  String entry;

  //CASE 1: Picker already in microcontroller shelving domain
  while(pickID2RGB.available()) //run loop while next byte available
  {
    char nextChar = '\0';
    while(nextChar != '\n') //end String for one entry at a newline
    {
      entry += (String) nextChar; //add next character to existing String
      nextChar = pickID2RGB.read(); //find next character
    }

    //if a String matches the pickerID, get color index
    if (entry.substring(0, entry.indexOf('\t')).equals(String(pickerID)))
    {
      colorIndex = entry.substring(entry.indexOf('\t') + 1,entry.indexOf('\n')).toInt(); //color index is btwn tab and newline
      break; //no need to go through the rest of the file
    }
    entry = "";
  }
  
  //CASE 2: Picker entering microcontroller shelving domain
  //if ON commanded and picker not yet in database, assign color to picker
  bool assigned = false;  //indicator that picker has been assigned a color
  if (colorIndex == -1 && on) //if CASE 1 was not satisfied and we want to turn an LED on, must be CASE 2
  {
    pickID2RGB = SD.open("pick.txt"); //open file containing database
    String newPickerUpdate; //String to upload to SD card
    int prevColorIndex = -1;
    int newColorIndex;

    //if file is blank, add new entry as normal
    if(!pickID2RGB.available())
    {
      newPickerUpdate += (String) pickerID + '\t' + "0" + '\n';
      colorIndex = 0;
    }
    //if file already has entries, add new entry that assigns new picker to first available color
    else
    {
      while(pickID2RGB.available())
      {
        char nextChar = '\0';
        while(nextChar != '\n') //end String for one entry at a newline
        {
          entry += (String) nextChar; //add next character to existing String
          nextChar = pickID2RGB.read(); //find next character
        }
        entry += '\n';
        
        newColorIndex = entry.substring(entry.indexOf('\t')+1,entry.indexOf('\n')).toInt(); //color index contained by the next database entry
        
        //if no available spot, append entry to String to update SD card
        if (newColorIndex == prevColorIndex + 1)
          newPickerUpdate += entry;
        //if there is an available spot, append new picker's entry first and then append following picker's entry
        else if (newColorIndex > prevColorIndex + 1)
        {
          //if already assigned, we don't want to fill in additional spots
          //otherwise fill in space with new picker and copy following entry
          if (!assigned)
          {
            newPickerUpdate += ((String) pickerID + '\t' + (String) (prevColorIndex + 1) + '\n');
            newPickerUpdate += entry;
            colorIndex = prevColorIndex + 1;
            assigned = true;
          }
          //if already assigned, need to include entries that are after available spots
          else
            newPickerUpdate += entry;
        }

        //if the only available spot is at the end of the database, append the new entry there
        if (pickID2RGB.peek() == -1 && !assigned)
        {
          newPickerUpdate += ((String) pickerID + '\t' + (String) (newColorIndex + 1) + '\n');
          colorIndex = newColorIndex + 1;
        }
        
        prevColorIndex = newColorIndex; //reset previous color index for next loop iteration
        entry = ""; //reset entry for next loop iteration
      }
    }
    Serial.println(newPickerUpdate);
    pickID2RGB.close(); //close database file
    updateSD("pick.txt", newPickerUpdate); //update file with new String
  }
  
  //CASE 3: Picker exits microcontroller shelving domain
  //if OFF commanded and next pick is outside microcontroller's shelving domain, remove picker from database
  if (!on && storLoc_to_LED(nextStorLoc) == -1)
  {
    File pickID2RGB = SD.open("pick.txt"); //open file containing database
    String entry;
    String exitPickerUpdate; //String to upload to SD card
    
    while(pickID2RGB.available())
    {
      char nextChar = '\0';
      while(nextChar != '\n') //end String for one entry at a newline
      {
        entry += (String) nextChar; //add next character to existing String
        nextChar = pickID2RGB.read(); //find next character
      }
      entry += '\n';

      //add the line to the new string if it doesn't have the pickerID of the exiting picker
      if (entry.substring(0, entry.indexOf('\t')) != (String) pickerID)
        exitPickerUpdate += entry;
        
      entry = "";
    }
    Serial.println(exitPickerUpdate);
    pickID2RGB.close(); //close database file
    updateSD("pick.txt", exitPickerUpdate); //update file with new String
  }
  return colorIndex;
}