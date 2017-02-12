
#define DEBUG_LEVEL DEBUG_HIGH
#include "Debug.h"

#include <RFM69.h>
#include <RFM69_ATC.h>
#include <SPI.h>

#include "Bringup.h"

#ifdef ENABLE_ATC
RFM69_ATC radio;
#else
RFM69 radio;
#endif

void configure_radio() {
  DEBUG1_PRINTLN("*** Configuring radio");

#if FREQUENCY==RF69_433MHZ
  DEBUG2_PRINTLN("* Frequency: 433");
#endif
#if FREQUENCY==RF69_868MHZ
  DEBUG2_PRINTLN("* Frequency: 868");
#endif
#if FREQUENCY==RF69_915MHZ
  DEBUG2_PRINTLN("* Frequency: 915");
#endif

  DEBUG2_VALUE("* NodeID:", NODEID);
  DEBUG2_VALUELN(" NetworkID:", NETWORKID);
  radio.initialize(FREQUENCY, NODEID, NETWORKID);

#ifdef IS_RFM69HW
  DEBUG2_PRINTLN("*** High power mode enabled");
  radio.setHighPower(); //only for RFM69HW!
#endif

#ifdef ENABLE_ATC
  DEBUG2_PRINTLN("*** RFM69_ATC Enabled (Auto Transmission Control)");
#endif

  radio.encrypt(ENCRYPTKEY);
  radio.promiscuous(false);

#ifdef ENABLE_ATC
  radio.enableAutoPower(-70);
#endif
}


/*
 * Heartbeat structure
 */
#define HEARTBEAT_MAGIC 0xefbeadde
typedef struct {
  uint32_t magic;
  uint32_t sequence;
  byte payload[32 - sizeof (char)*4 + sizeof (uint32_t)];
} heartbeat_msg_t;
heartbeat_msg_t heartbeat;
unsigned int heartbeat_period_ms = 2000;  // Period between heartbeats

#define LED_FLASH_MS 200
unsigned long last_heartbeat_ms = 0;
unsigned long last_receive_ms = 0;

boolean handle_radio() {
  unsigned long now = millis();

  // Check for any received data
  if (radio.receiveDone())
  {
    DEBUG1_VALUE("Received from:", radio.SENDERID);
    DEBUG1_VALUE(" to:", radio.TARGETID);
    DEBUG1_VALUE(" size:", radio.DATALEN);
    DEBUG1_VALUE(" rssi:", radio.RSSI);

    // Check if the received data is a heartbeat message
    heartbeat_msg_t *msg = (heartbeat_msg_t *) radio.DATA;
    boolean is_heartbeat = false;
    if ((radio.DATALEN == sizeof (heartbeat_msg_t)) &&
        (msg->magic == HEARTBEAT_MAGIC)) {
      DEBUG1_VALUE(" HB:", msg->sequence);
      is_heartbeat = true;
    } else {
      DEBUG1_PRINT(" Not HB");
    }

    for (byte i = 0; i < radio.DATALEN; i++) {
      DEBUG1_HEXVAL(" ", (byte)radio.DATA[i]);
    }

    DEBUG_ENDLN();

    last_receive_ms = now;
  }

  // Send a new heartbeat message if the period has elapsed
  if (now - last_heartbeat_ms >= heartbeat_period_ms) {
    static uint32_t heartbeat_sequence = 0;

    last_heartbeat_ms = now;

    heartbeat.magic = HEARTBEAT_MAGIC;
    heartbeat.sequence = heartbeat_sequence++;

    DEBUG1_VALUE("Heartbeat size:", sizeof(heartbeat_msg_t));
    DEBUG1_VALUE(" seq:", heartbeat.sequence);
    DEBUG1_PRINT(" data: ");
    for (byte i = 0; i < sizeof(heartbeat_msg_t); i++)
    DEBUG1_HEXVAL(" ", ((byte *)&heartbeat)[i]);

    radio.send(RF69_BROADCAST_ADDR, &heartbeat, sizeof(heartbeat_msg_t));
  }

  if (now - last_receive_ms <= LED_FLASH_MS) {
    leds[RECV_LED] = CRGB(255, 0, 0);
  }  else {
    leds[RECV_LED] = CRGB(0, 0, 0);
  }

  if (now - last_heartbeat_ms <= LED_FLASH_MS) {
    leds[XMIT_LED] = CRGB(0, 0, 255);
  }  else {
    leds[XMIT_LED] = CRGB(0, 0, 0);
  }
}