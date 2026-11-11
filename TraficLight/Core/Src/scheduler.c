#include "scheduler.h"

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t SCH_count = 0;
uint32_t current_task_id = 0;

void SCH_Init(void) {
    SCH_count = 0;
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = 0;
    }
}

void SCH_Update(void) {
    if (SCH_count == 0) return;
    if (SCH_tasks_G[0].Delay > 0) {
        SCH_tasks_G[0].Delay--;
    }
    if (SCH_tasks_G[0].Delay == 0) {
        SCH_tasks_G[0].RunMe = 1;
    }
}

uint32_t SCH_Add_Task(void (*pFunc)(void), uint32_t DELAY, uint32_t PERIOD) {
    if (SCH_count >= SCH_MAX_TASKS) return SCH_MAX_TASKS;

    uint32_t newDelay = DELAY;
    uint8_t insertIndex = 0;

    while (insertIndex < SCH_count && newDelay >= SCH_tasks_G[insertIndex].Delay) {
        newDelay -= SCH_tasks_G[insertIndex].Delay;
        insertIndex++;
    }

    for (int i = SCH_count; i > insertIndex; i--) {
        SCH_tasks_G[i] = SCH_tasks_G[i - 1];
    }

    SCH_tasks_G[insertIndex].pTask = pFunc;
    SCH_tasks_G[insertIndex].Delay = newDelay;
    SCH_tasks_G[insertIndex].Period = PERIOD;
    SCH_tasks_G[insertIndex].RunMe = 0;
    SCH_tasks_G[insertIndex].TaskID = current_task_id++;

    if (insertIndex + 1 <= SCH_count) {
        SCH_tasks_G[insertIndex + 1].Delay -= newDelay;
    }

    SCH_count++;
    return SCH_tasks_G[insertIndex].TaskID;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    for (int i = 0; i < SCH_count; i++) {
        if (SCH_tasks_G[i].TaskID == taskID) {
            if (i + 1 < SCH_count) {
                SCH_tasks_G[i + 1].Delay += SCH_tasks_G[i].Delay;
            }
            for (int j = i; j < SCH_count - 1; j++) {
                SCH_tasks_G[j] = SCH_tasks_G[j + 1];
            }
            SCH_count--;
            SCH_tasks_G[SCH_count].pTask = 0;
            return 1;
        }
    }
    return 0;
}

void SCH_Dispatch_Tasks(void) {
    if (SCH_count == 0) return;

    if (SCH_tasks_G[0].RunMe == 1) {
        (*SCH_tasks_G[0].pTask)();

        void (*pFunc)(void) = SCH_tasks_G[0].pTask;
        uint32_t period = SCH_tasks_G[0].Period;
        uint32_t id = SCH_tasks_G[0].TaskID;

        SCH_Delete_Task(id);

        if (period > 0) {
            SCH_Add_Task(pFunc, period, period);
        }
    }
}
