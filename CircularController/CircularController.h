/*******************************************************************************
 * Author: Adam Phelps
 * License: Create Commons Attribution-Non-Commercial
 * Copyright: 2017
 ******************************************************************************/

#ifndef CIRCULARCONTROLLER_CIRCULARCONTROLLER_H
#define CIRCULARCONTROLLER_CIRCULARCONTROLLER_H


#ifndef HMTL_FIRE_CONTROL_H
#define HMTL_FIRE_CONTROL_H

#include "Socket.h"
#include "RFM69Socket.h"
#include "MPR121.h"
#include "PixelUtil.h"

/*
 * Definitions for the different controllers that work with this code
 */
#define OBJECT_TYPE_CIRCULAR_CONTROLLER 6
#define OBJECT_TYPE OBJECT_TYPE_CIRCULAR_CONTROLLER


#define BAUD 57600

// Pixels used for under lighting
extern PixelUtil pixels;

extern config_hdr_t config;
extern output_hdr_t *outputs[HMTL_MAX_OUTPUTS];
extern void *objects[HMTL_MAX_OUTPUTS];

/***** Poofer bullshit ********************************************************/
void update_poofers();

/***** Sensor info ************************************************************/

/* All sensor info is recorded in a bit mask */
extern uint32_t sensor_state;

// XXX - Sensor macros go here?
#define SWITCH_PIN_1 5
#define SWITCH_PIN_2 6
#define SWITCH_PIN_3 9
#define SWITCH_PIN_4 10

void initialize_switches();
void sensor_switches();
void calculate_pulse();

extern MPR121 touch_sensor;
void sensor_cap();

void handle_sensors();

// Capacitive sensors

#if OBJECT_TYPE == OBJECT_TYPE_CIRCULAR_CONTROLLER

// Sensors
#define POOFER1_POOF2_QUICK_SENSOR    0
#define POOFER1_POOF2_LONG_SENSOR     1
#define POOFER1_MODE2_SENSOR          2
#define POOFER1_MODE1_SENSOR          3
#define POOFER1_POOF1_LONG_SENSOR     4
#define POOFER1_POOF1_QUICK_SENSOR    5

#define POOFER2_POOF2_QUICK_SENSOR    6
#define POOFER2_POOF2_LONG_SENSOR     7
#define POOFER2_MODE2_SENSOR          8
#define POOFER2_MODE1_SENSOR          9
#define POOFER2_POOF1_LONG_SENSOR    10
#define POOFER2_POOF1_QUICK_SENSOR   11

// TODO: Delete me
#define POOFER_IGNITER_SWITCH 0
#define POOFER_PILOT_SWITCH   1
#define POOFER_ENABLE_SWITCH  2
#define LIGHTS_ON_SWITCH  3


#else
#error OBJECT_TYPE must be set
#endif


/***** Connectivity ***********************************************************/


extern Socket *sockets[];
extern RFM69Socket rfm69;
extern uint16_t my_address;
extern byte *send_buffer;

#define SEND_BUFFER_SIZE RF69_MAX_DATA_LEN
#define SEND_DATA_SIZE RFM69_DATA_LENGTH(RF69_MAX_DATA_LEN)



// Poofer definitions
#ifndef POOFER1_ADDRESS
#define POOFER1_ADDRESS  0x40
#endif
#define POOFER1_IGNITER  0x0
#define POOFER1_PILOT    0x1
#define POOFER1_POOF1    0x2
#define POOFER1_POOF2    0x3

#ifndef POOFER2_ADDRESS
#define POOFER2_ADDRESS  0x41
#endif
#define POOFER2_IGNITER  0x0
#define POOFER2_PILOT    0x1
#define POOFER2_POOF1    0x2
#define POOFER2_POOF2    0x3


#ifndef LIGHTS_ADDRESS
#define LIGHTS_ADDRESS  0x45
#endif
#define LIGHTS1 0x0
#define LIGHTS2 0x1

extern uint16_t poofer1_address;
extern uint16_t poofer2_address;
extern uint16_t lights_address;



void initialize_connect();

void sendHMTLValue(uint16_t address, uint8_t output, int value);
void sendHMTLTimedChange(uint16_t address, uint8_t output,
                         uint32_t change_period,
                         uint32_t start_color,
                         uint32_t stop_color);
void sendHMTLCancel(uint16_t address, uint8_t output);
void sendHMTLBlink(uint16_t address, uint8_t output,
                   uint16_t onperiod, uint32_t oncolor,
                   uint16_t offperiod, uint32_t offcolor);
#endif


#endif //CIRCULARCONTROLLER_CIRCULARCONTROLLER_H
