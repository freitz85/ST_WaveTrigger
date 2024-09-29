#include "serial.h"
#include "RTOS.h"

#include <stdint.h>

static OS_TASK         	SerialTaskCB;
static OS_STACKPTR int 	SerialStack[512];

OS_MAILBOX      		RxMailboxCB;
static char            	RxMailboxBuffer[128];
static OS_MAILBOX      	TxMailboxCB;
static char             TxMailboxBuffer[64];

static void SerialTask(void);

void StartSerialTask(void)
{
	OS_TASK_Create(&SerialTaskCB, "SerialTask", 100, SerialTask, SerialStack, sizeof(SerialStack), 2);
	OS_MAILBOX_Create(&RxMailboxCB, 1, 128, &RxMailboxBuffer);
	OS_MAILBOX_Create(&TxMailboxCB, 1, 64,  &TxMailboxBuffer);
}

void SerialTask(void) {
	while(1)
	{
		uint8_t msg;
		OS_MAILBOX_GetBlocked1(&RxMailboxCB, &msg);
	}
}
