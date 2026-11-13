
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define SCH_MAX_TASKS       40U
#define SCH_INVALID_TASK_ID UINT32_MAX

void SCH_Init(void);

uint32_t SCH_Add_Task(
    void (*pFunc)(void),
    uint32_t DELAY,
    uint32_t PERIOD
);

uint8_t SCH_Delete_Task(uint32_t taskID);

void SCH_Update(void);

void SCH_Dispatch_Tasks(void);

#endif
