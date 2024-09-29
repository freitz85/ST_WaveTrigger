#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include "RTOS.h"

extern OS_MAILBOX RxMailboxCB;

void StartSerialTask(void);

#endif /* INC_SERIAL_H_ */
