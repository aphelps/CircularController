//
// Created by Adam Phelps on 2/16/17.
//



#ifdef DEBUG_LEVEL_CONNECT
#define DEBUG_LEVEL DEBUG_LEVEL_CONNECT
#endif
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_HIGH
#endif
#include "Debug.h"

#include <RFM69.h>
#include <RFM69_ATC.h>
#include <SPI.h>

#include "GeneralUtils.h"
#include "EEPromUtils.h"
#include "HMTLTypes.h"
#include <HMTLProtocol.h>
#include "HMTLMessaging.h"
#include "HMTLPrograms.h"

#include "CircularControllerConnect.h"
#include "CircularController.h"

void sendHMTLValue(uint16_t address, uint8_t output, int value) {
  DEBUG3_VALUE("sendValue:", value);
  DEBUG3_VALUE(" a:", address);
  DEBUG3_VALUELN(" o:", output);
  hmtl_send_value(&rfm69, rfm69.send_buffer, SEND_BUFFER_SIZE,
                  address, output, value);
}

void sendHMTLTimedChange(uint16_t address, uint8_t output,
                         uint32_t change_period,
                         uint32_t start_color,
                         uint32_t stop_color) {
  DEBUG3_VALUE("sendTimed:", change_period);
  DEBUG3_VALUE(" a:", address);
  DEBUG3_VALUELN(" o:", output);

  hmtl_send_timed_change(&rfm69, rfm69.send_buffer, SEND_BUFFER_SIZE,
                         address, output,
                         change_period,
                         start_color,
                         stop_color);
}

void sendHMTLCancel(uint16_t address, uint8_t output) {
  DEBUG3_VALUE("sendCancel: a:", address);
  DEBUG3_VALUELN(" o:", output);

  hmtl_send_cancel(&rfm69, rfm69.send_buffer, SEND_BUFFER_SIZE,
                   address, output);
}

void sendHMTLBlink(uint16_t address, uint8_t output,
                   uint16_t onperiod, uint32_t oncolor,
                   uint16_t offperiod, uint32_t offcolor) {
  DEBUG3_VALUE("sendBlink:", onperiod);
  DEBUG3_VALUE(",", offperiod);
  DEBUG3_VALUE(" a:", address);
  DEBUG3_VALUELN(" o:", output);

  hmtl_send_blink(&rfm69, rfm69.send_buffer, SEND_BUFFER_SIZE,
                  address, output,
                  onperiod, oncolor,
                  offperiod, offcolor);
}