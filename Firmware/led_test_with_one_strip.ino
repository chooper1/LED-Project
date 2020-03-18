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
   setup runs once and sets up the pixels strip
*/
void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.clear();
}

/*
   loop repeats forever
*/
void loop() {
  for (int i = 0; i < NUMBER_PER_STRIP; i++) {
    pixels.clear();
    led_on(i);
    delay(DELAYVAL);
  }
}

/*
 * support functions for turning LEDs on and off based on picker ID
 */
void led_on(int PICKER_ID)
{
  int led_number = PICKER_ID;
  pixels.setPixelColor(led_number, pixels.Color(colors[PICKER_ID][0],
                       colors[PICKER_ID][1], colors[PICKER_ID][2]));
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
