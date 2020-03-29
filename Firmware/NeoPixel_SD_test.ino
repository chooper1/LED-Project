#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

/*
   setting up the pin you're connected to on the arduino,
   the number of leds (8 normally, more if you're using daisy chain)
   then initializing the neopixels connected to that pin
*/

#define DELAYVAL 15
#define LED_COUNT 8
#define PIN 6

const int maxBright = 100;

Adafruit_NeoPixel pixels(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
int colors[8][3] = {
    {0, maxBright, 0},  //green
    {maxBright, 0, 0},  //red
    {0, 0, maxBright},  //blue
    {4*maxBright/5, 3*maxBright/5, 0},  //yellow
    {0, maxBright, maxBright},  //turquoise
    {maxBright, 0, maxBright},  //magenta
    {maxBright, 1*maxBright/5, 0}, //orange
    {maxBright, maxBright, maxBright} //white
};

/*
   setup runs once and sets up the pixels strip
*/
void setup() 
{
  delay(2000);
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

  int storLocs[] = {237,365,462,601,739,852,913,1024}; //random storage locations
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
}

/*
   loop repeats forever
*/
void loop()
{
  Serial.println("Enter storage location:");  //user inputs storage location
  String storLoc;
  while(!Serial.available());
  if(Serial.available())
  {
    storLoc = Serial.readString();  //read user input
    Serial.println(storLoc);
  }

  Serial.println("Enter picker ID:"); //user inputs picker ID
  String pickerID;
  while(!Serial.available());
  if(Serial.available())
  {
    pickerID = Serial.readString(); //read user input
    Serial.println(pickerID);
  }

  //convert inputs to integers
  int LEDIndex = storLoc_to_LED(storLoc.toInt()); 
  int colorIndex = pickerID_to_RGB(pickerID.toInt());

  //update NeoPixel if storage location and picker ID are valid
  Serial.println(LEDIndex);
  Serial.println(colorIndex);
  if ((LEDIndex != -1) && (colorIndex != -1))
  {
    pixels.clear();
    led_on(LEDIndex, colorIndex);
  }
  //turn off all LEDs  if storage location or picker ID is invalid
  else
  {
    pixels.fill();
    pixels.show();
  }
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
void led_off(int PICKER_ID)
{
  int led_number = PICKER_ID;
  pixels.setPixelColor(led_number, pixels.Color(0, 0, 0));
  pixels.show();
  delay(5);
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
    updateFile.println(newData); //write new data into file
    updateFile.close();
    Serial.println("file updated succesfully");
  }
  else 
    Serial.println("file not updated");
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
