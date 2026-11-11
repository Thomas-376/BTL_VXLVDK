#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "main.h"

#define SCH_MAX_TASKS 40

typedef struct {
    void (*pTask)(void);
    uint32_t Delay;
    uint32_t Period;
    uint8_t  RunMe;
    uint32_t TaskID;
} sTask;

extern uint8_t SCH_count;

void SCH_Init(void);
uint32_t SCH_Add_Task(void (*pFunc)(void), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);

#endif
