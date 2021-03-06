/*******************************************************************************
 * Author: Adam Phelps
 * License: Create Commons Attribution-Non-Commercial
 * Copyright: 2017
 *
 * Message handling and output modes
 ******************************************************************************/

#ifndef CIRCULARCONTROLLER_CIRCULARCONTROLLERMODES_H
#define CIRCULARCONTROLLER_CIRCULARCONTROLLERMODES_H


#include <HMTLPrograms.h>

/* Initialize the message and mode handlers */
void init_modes(Socket **sockets, byte num_sockets);

/* Check for messages and handle program modes */
boolean messages_and_modes(void);

void setSparkle();
void setBlink();
void setCancel();


boolean followup_actions();

/*******************************************************************************
 * Control box specific modes
 */

#endif //CIRCULARCONTROLLER_CIRCULARCONTROLLERMODES_H
