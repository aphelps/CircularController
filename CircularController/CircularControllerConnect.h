/*******************************************************************************
 * 1st edition wireless wooden box controller
 *
 * Author: Adam Phelps
 * License: Create Commons Attribution-Non-Commercial
 * Copyright: 2017
 ******************************************************************************/

#ifndef CIRCULARCONTROLLER_CIRCULARCONTROLLERCONNECT_H
#define CIRCULARCONTROLLER_CIRCULARCONTROLLERCONNECT_H

#include "RFM69Socket.h"

extern RFM69Socket rfm69;

void configure_radio();

bool handle_radio();

/******************************************************************************/

#endif //CIRCULARCONTROLLER_CIRCULARCONTROLLERCONNECT_H
