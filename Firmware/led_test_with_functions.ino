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
#define NUMBER_PER_STRIP 8
#define NUMPIXELS 16 /* number of strips times number per strip */
#define PIN 6

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int part;
int not_part;
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
   setup runs once and sets up the pixels strip
*/
void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.clear();
  part = 0;
  not_part = 1;
}

/*
   loop repeats forever, and loops through two strips of leds in an alternating pattern
*/
void loop() {
  for (int i = 0; i < NUMBER_PER_STRIP; i++) {
    if (part == 0) {
      part = 1;
      not_part = 0;
    } else {
      part = 0;
      not_part = 1;
    }
    pixels.clear();
    led_on(i, part);
    delay(DELAYVAL);
  }
  if (part == 0) {
    part = 1;
    not_part = 0;
  } else {
    part = 0;
    not_part = 1;
  }
}

/*
 * support functions for turning LEDs on and off based on picker ID and item number
 */
void led_on(int PICKER_ID, int ITEM_ID)
{
  int led_number = 8 * ITEM_ID + PICKER_ID;
  pixels.setPixelColor(led_number, pixels.Color(colors[PICKER_ID][0],
                       colors[PICKER_ID][1], colors[PICKER_ID][2]));
  pixels.show();
  delay(5);
}

/*
 * the led_off function allows a specific led to be turned off without clearing all the other leds
 */
void led_off(int PICKER_ID, int ITEM_ID)
{
  int led_number = 8 * ITEM_ID + PICKER_ID;
  pixels.setPixelColor(led_number, pixels.Color(0, 0, 0));
  pixels.show();
  delay(5);
}
