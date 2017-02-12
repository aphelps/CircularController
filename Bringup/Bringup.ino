/*
 *
 */

#include "FastLED.h"

#define DEBUG_LEVEL DEBUG_HIGH
#include "Debug.h"

#include "MPR121.h"
#include <Wire.h>

#include "Bringup.h"

MPR121 touch;


// Define the array of leds
CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(57600);
  Serial.println("*** Circular Controller Bringup Script ***");

  Wire.begin();

  touch = MPR121(
          IRQ_PIN,       // triggered/interupt pin
          false,         // interrupt mode?
          I2C_ADDRESS,   // START_ADDRESS = 0x5A
          true,          // use touch times
          false          // use auto config and reconfig
  );
  touch.setThresholds(15,2);

  Serial.println(F("MPR121 sensor initialized"));


#ifdef PIXELS_APA102_12_8
  FastLED.addLeds<APA102, 12, 8, BGR>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
#endif
#ifdef PIXELS_APA102_20_21
  FastLED.addLeds<APA102, 20, 21, BGR>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
#endif
  LEDS.setBrightness(64);

  pinMode(DEBUG_LED_PIN, OUTPUT);

  configure_radio();

  DEBUG1_PRINTLN("*** Initializtion complete ***");
}

byte sensor_to_led(byte sensor) {
  /*
   * Sensor  LED
   * 10      10
   * 9       11
   * 8       0
   * 7       1
   * 6       2
   */
  return (20 - sensor) % 12;
}

byte current_led = 0;
boolean debug_led_state = false;
void loop() {

  current_led++;

  leds[(current_led - 3 + NUM_LEDS_IN_CIRCLE) % NUM_LEDS_IN_CIRCLE] = CRGB(0,0,0);
  leds[(current_led - 2 + NUM_LEDS_IN_CIRCLE) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(16);
  leds[(current_led - 2 + NUM_LEDS_IN_CIRCLE) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(32);
  leds[(current_led - 2 + NUM_LEDS_IN_CIRCLE) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(64);
  leds[(current_led - 1 + NUM_LEDS_IN_CIRCLE) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(128);
  leds[current_led % NUM_LEDS_IN_CIRCLE] = CHSV(current_led, 255,255);
  leds[(current_led + 1) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(128);;
  leds[(current_led + 2) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(64);
  leds[(current_led + 2) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(32);
  leds[(current_led + 2) % NUM_LEDS_IN_CIRCLE] = CRGB(CHSV(current_led, 255,255)).nscale8(16);

//  for (byte led = NUM_LEDS_IN_CIRCLE; led < NUM_LEDS; led++) {
//    leds[led] = CHSV(current_led-128, 255,255);
//  }

  touch.readTouchInputs();
  for (int i = 0; i < 12; i++) {
    if (touch.changed(i)) {
      if (touch.touched(i)) {
        DEBUG3_VALUELN("Touched:", i);
      } else {
        DEBUG3_VALUELN("Released:", i);
        byte led = sensor_to_led(i);
        if (leds[led] == CRGB(255, 255, 255)) {
          leds[led] = 0;
        }
      }
    }

    if (touch.touched(i)) {
      leds[sensor_to_led(i)] = CRGB(255, 255, 255);
    }
  }

  /* Check the radio and possible send a heartbeat message */
  handle_radio();

  FastLED.show();

  digitalWrite(DEBUG_LED_PIN, debug_led_state);
  debug_led_state = !debug_led_state;

  delay(50);
}