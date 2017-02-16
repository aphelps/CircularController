/*******************************************************************************
 * 1st edition wireless wooden box controller
 *
 * Author: Adam Phelps
 * License: Create Commons Attribution-Non-Commercial
 * Copyright: 2017
 ******************************************************************************/

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_HIGH
#endif
#include "Debug.h"

#include "EEPROM.h"
#include <RS485_non_blocking.h>
#include <SoftwareSerial.h>
#include "LiquidCrystal.h"

#include "SPI.h"
#include "FastLED.h"

#include "GeneralUtils.h"
#include "EEPromUtils.h"
#include "HMTLTypes.h"
#include "PixelUtil.h"
#include "Wire.h"
#include "MPR121.h"
#include "SerialCLI.h"

#include "Socket.h"
#include "RS485Utils.h"

#include "HMTLProtocol.h"
#include "HMTLMessaging.h"
#include "HMTLPrograms.h"
#include "TimeSync.h"
#include "ProgramManager.h"
#include "MessageHandler.h"

#include "CircularController.h"
#include "CircularControllerModes.h"
#include "CircularControllerSensors.h"

/* Auto update build number */
#define CIRCULAR_CONTROLLER_BUILD 7 // %META INCR

#include "RFM69.h"
#include "RFM69Socket.h"

// TODO: This should be in the configuration
#define NETWORK 100

#ifndef IRQ_PIN
#define IRQ_PIN 2
#endif

#ifndef IS_RFM69HW
#define IS_RFM69HW true
#endif
// TODO: End of stuff for configuration

RFM69Socket rfm69;
byte databuffer[SEND_BUFFER_SIZE];



MPR121 touch_sensor; // MPR121 must be initialized after Wire.begin();
uint16_t my_address = 0;

/******/

config_hdr_t config;
output_hdr_t *outputs[HMTL_MAX_OUTPUTS];
config_max_t readoutputs[HMTL_MAX_OUTPUTS];
void *objects[HMTL_MAX_OUTPUTS];

config_value_t value_output;

PixelUtil pixels;

// XXX: RFM69

#define MAX_SOCKETS 2
Socket *sockets[MAX_SOCKETS] = { NULL, NULL };

void setup() {
  Serial.begin(BAUD);
  DEBUG2_PRINTLN("*** HMTL Fire Control Initializing ***");
  DEBUG2_VALUELN("* Baud is ", BAUD);

  /* Initialize random see by reading from an unconnected analog pin */
  randomSeed(analogRead(0) + analogRead(2) + micros());

  int configOffset = -1;
  int32_t outputs_found = hmtl_setup(&config, readoutputs,
                                     outputs, objects, HMTL_MAX_OUTPUTS,
                                     NULL,
                                     NULL,          // XBee
                                     &pixels,
                                     &touch_sensor, // MPR121
                                     NULL,          // RGB
                                     &value_output, // Value
                                     &configOffset);

  DEBUG4_VALUE("Config size:", configOffset - HMTL_CONFIG_ADDR);
  DEBUG4_VALUELN(" end:", configOffset);

  if (!(outputs_found & (1 << HMTL_OUTPUT_PIXELS))) {
    DEBUG_ERR("No pixels config found");
    DEBUG_ERR_STATE(2);
  }

  if (!(outputs_found & (1 << HMTL_OUTPUT_MPR121))) {
    DEBUG_ERR("No mpr121 config found");
    DEBUG_ERR_STATE(3);
  }

  /* Setup communication devices */
  byte num_sockets = 0;

  /* Setup the RFM69 radio */
  if (true) {
    // XXX: RFM69!
    rfm69.init(config.address, NETWORK, IRQ_PIN, IS_RFM69HW, RF69_915MHZ); // TODO: Put this in config
    rfm69.setup();
    rfm69.initBuffer(databuffer, SEND_DATA_SIZE);
    sockets[num_sockets++] = &rfm69;
  }

  if (num_sockets == 0) {
    DEBUG_ERR("No sockets configured");
    DEBUG_ERR_STATE(2);
  }

  init_modes(sockets, num_sockets);

  touch_sensor.setThresholds(3, 1);

  /* Setup the sensors */
  initialize_switches();

  DEBUG2_PRINTLN("* Wickerman Fire Control Initialized *");
  DEBUG2_VALUELN(" Build=", CIRCULAR_CONTROLLER_BUILD);
  DEBUG_MEMORY(DEBUG_HIGH);


  DEBUG2_VALUE("POOF1_ADDRESS=", poofer1_address);
  DEBUG2_VALUE("POOF2_ADDRESS=", poofer2_address);
  DEBUG2_VALUELN("LIGHTS_ADDRESS=", lights_address);

  // Send the ready signal to the serial port
  Serial.println(F(HMTL_READY));
}

void loop() {
  /* Check the sensor values */
  sensor_cap();

  sensor_switches();

  handle_sensors();

  // XXX: Do stuff here

  //update_poofers();

  /*
   * Check for messages and handle output states
   */
  boolean updated = messages_and_modes();
  if (updated) {

  }
}

