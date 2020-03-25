#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

/*
   setting up the pin you're connected to on the arduino,
   the number of leds (8 normally, more if you're using daisy chain)
   then initializing the neopixels connected to that pin
*/

#define DELAYVAL 500
#define NUMBER_PER_STRIP 1 /* setting this to 1 allows us to test multiple locations even with only 1 strip */
#define NUMPIXELS 16 /* number of strips times number per strip */
#define PIN 6

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int[3] color = {0, 0, 0};

/*
   setup runs once and sets up the pixels strip
*/
void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.clear();
  // sd card and wifi connection will also need to be set up here!
}

/*
   loop repeats forever, and loops through all the LEDs
*/
void loop() {
  /* firmware should block here waiting for input over wifi
  location, turn_on, RFID = input_over_wifi;
  if(turn_on) {
      led_on_rfid(RFID, location);
  } else {
      led_off_rfid(RFID, location);
  }
  */
  delay(DELAYVAL);
}

/*
 * Support function for getting the LED index using the location
 */
translateLocationToID(int location) 
{
  int led_index = ; // access from the product table on the sd card using location
  return led_index;
}

/*
 * Support function for getting the LED color based on RFID
 * The color r,g,b values are stored in the global variable color
 * To get around not being able to return more than one value 
 * Alternatively, a pointer to an array of ints could be passed in
 */
getRFIDColor(int RFID) 
{
  int red_val = ; // access from the colors table on the sd card using RFID
  int green_val = ; // access from the colors table on the sd card using RFID
  int blue_val = ; // access from the colors table on the sd card using RFID
  color[0] = red_val;
  color[1] = green_val;
  color[2] = blue_val;
}

/*
 * functions for turning LEDs on and off based and item number (no picker ID)
 */
void led_on(int location)
{
  int ITEM_ID = translateLocationToID(location);
  int led_number = NUMBER_PER_STRIP * ITEM_ID;
  pixels.setPixelColor(led_number, pixels.Color(150, 0, 0));
  pixels.show();
  delay(5);
}

/*
 * the led_off function allows a specific led to be turned off without clearing all the other leds
 */
void led_off(int location)
{
  int ITEM_ID = translateLocationToID(location);
  int led_number = NUMBER_PER_STRIP * ITEM_ID;
  pixels.setPixelColor(led_number, pixels.Color(0, 0, 0));
  pixels.show();
  delay(5);
}

/*
 * functions for turning LEDs on and off based on RFID and item number
 */
void led_on_rfid(int RFID, int location)
{
  int ITEM_ID = translateLocationToID(location); 
  int led_number = NUMBER_PER_STRIP * ITEM_ID;
  getRFIDColor(RFID); 
  pixels.setPixelColor(led_number, pixels.Color(color[RFID], color[RFID], color[RFID]));
  pixels.show();
  delay(5);
}

/*
 * the led_off_rfid function allows a specific led to be turned off without clearing all the other leds
 */
void led_off_rfid(int RFID, int location)
{
  int ITEM_ID = translateLocationToID(location);
  int led_number = NUMBER_PER_STRIP * ITEM_ID;
  pixels.setPixelColor(led_number, pixels.Color(0, 0, 0));
  pixels.show();
  delay(5);
}
