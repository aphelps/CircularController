//
// Created by Adam Phelps on 2/5/17.
//

#ifndef HMTL_FIRE_CONTROL_BRINGUP_H_H
#define HMTL_FIRE_CONTROL_BRINGUP_H_H

#include "FastLED.h"
#include <RFM69.h>
#include <RFM69_ATC.h>
#include <MPR121.h>

/*******************************************************************************
 * LED configuration
 */
#define NUM_LEDS 16
extern CRGB leds[NUM_LEDS];

#define DEBUG_LED_PIN 15

#define NUM_LEDS_IN_CIRCLE 12

#define XMIT_LED 12
#define RECV_LED 13

/*******************************************************************************
 * MPR121
 */
extern MPR121 touch;

#define I2C_ADDRESS 0x5A // 0x5A - 0x5D
#define IRQ_PIN 2

/*******************************************************************************
 * Radio configurable settings, set here or via compiler flags
 */
#ifndef NODEID
#define NODEID        128
#endif

#ifndef NETWORKID
#define NETWORKID     100
#endif

#ifndef IS_RFM69HW
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#endif

#ifndef FREQUENCY
#define FREQUENCY     RF69_915MHZ // RF69_433MHZ,RF69_868MHZ,RF69_915MHZ
#endif

#ifndef ENCRYPTKEY
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#endif

#ifndef ENABLE_ATC
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
#endif

#ifdef ENABLE_ATC
extern RFM69_ATC radio;
#else
extern RFM69 radio;
#endif

void configure_radio();

boolean handle_radio();

/******************************************************************************/

#endif //HMTL_FIRE_CONTROL_BRINGUP_H_H
