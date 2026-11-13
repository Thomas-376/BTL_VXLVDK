
#include "scheduler.h"
#include "stm32f1xx.h"

/* Cau truc cua mot task */
typedef struct {
    void (*pTask)(void);
    uint32_t Delay;
    uint32_t Period;
    uint32_t TaskID;
} sTask;

/* Danh sach task */
static sTask SCH_tasks_G[SCH_MAX_TASKS];

static uint8_t SCH_count = 0;
static uint32_t current_task_id = 1;


/* Bao ve danh sach task khi ISR co the can thiep */
static uint32_t SCH_Enter_Critical(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

static void SCH_Exit_Critical(uint32_t primask) {
    __set_PRIMASK(primask);
}


/* Tim mot TaskID chua duoc su dung */
static uint32_t SCH_Get_New_TaskID(void) {
    uint32_t id;
    uint8_t found;

    do {
        id = current_task_id++;

        if (current_task_id == SCH_INVALID_TASK_ID) {
            current_task_id = 1;
        }

        found = 0;

        for (uint8_t i = 0; i < SCH_count; i++) {
            if (SCH_tasks_G[i].TaskID == id) {
                found = 1;
                break;
            }
        }

    } while (found || id == SCH_INVALID_TASK_ID || id == 0);

    return id;
}


/* Xoa task tai vi tri index */
static void SCH_Remove_Task(uint8_t index) {

    /* Cong Delay cua task bi xoa vao task ke tiep */
    if (index + 1 < SCH_count) {
        SCH_tasks_G[index + 1].Delay +=
            SCH_tasks_G[index].Delay;
    }

    /* Dich cac task phia sau len mot vi tri */
    for (uint8_t i = index; i < SCH_count - 1; i++) {
        SCH_tasks_G[i] = SCH_tasks_G[i + 1];
    }

    SCH_count--;

    SCH_tasks_G[SCH_count].pTask = 0;
    SCH_tasks_G[SCH_count].Delay = 0;
    SCH_tasks_G[SCH_count].Period = 0;
    SCH_tasks_G[SCH_count].TaskID = 0;
}


/* Khoi tao Scheduler */
void SCH_Init(void) {

    uint32_t primask = SCH_Enter_Critical();

    SCH_count = 0;
    current_task_id = 1;

    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].TaskID = 0;
    }

    SCH_Exit_Critical(primask);
}


/* Them task vao Delta List */
uint32_t SCH_Add_Task(
    void (*pFunc)(void),
    uint32_t DELAY,
    uint32_t PERIOD
) {

    if (pFunc == 0) {
        return SCH_INVALID_TASK_ID;
    }

    uint32_t primask = SCH_Enter_Critical();

    if (SCH_count >= SCH_MAX_TASKS) {
        SCH_Exit_Critical(primask);
        return SCH_INVALID_TASK_ID;
    }

    uint32_t newTaskID = SCH_Get_New_TaskID();
    uint32_t newDelay = DELAY;
    uint8_t insertIndex = 0;

    /* Tim vi tri chen */
    while (
        insertIndex < SCH_count &&
        newDelay >= SCH_tasks_G[insertIndex].Delay
    ) {
        newDelay -= SCH_tasks_G[insertIndex].Delay;
        insertIndex++;
    }

    /* Dich cac task phia sau */
    for (uint8_t i = SCH_count; i > insertIndex; i--) {
        SCH_tasks_G[i] = SCH_tasks_G[i - 1];
    }

    /* Chen task moi */
    SCH_tasks_G[insertIndex].pTask = pFunc;
    SCH_tasks_G[insertIndex].Delay = newDelay;
    SCH_tasks_G[insertIndex].Period = PERIOD;
    SCH_tasks_G[insertIndex].TaskID = newTaskID;

    /* Dieu chinh Delay cua task dung sau */
    if (insertIndex < SCH_count) {
        SCH_tasks_G[insertIndex + 1].Delay -= newDelay;
    }

    SCH_count++;

    SCH_Exit_Critical(primask);

    return newTaskID;
}


/* Xoa task theo TaskID */
uint8_t SCH_Delete_Task(uint32_t taskID) {

    uint32_t primask = SCH_Enter_Critical();

    for (uint8_t i = 0; i < SCH_count; i++) {

        if (SCH_tasks_G[i].TaskID == taskID) {

            SCH_Remove_Task(i);

            SCH_Exit_Critical(primask);
            return 1;
        }
    }

    SCH_Exit_Critical(primask);
    return 0;
}


/* Cap nhat thoi gian - O(1) */
void SCH_Update(void) {

    if (SCH_count == 0) {
        return;
    }

    if (SCH_tasks_G[0].Delay > 0) {
        SCH_tasks_G[0].Delay--;
    }
}


/* Thuc thi task da den han */
void SCH_Dispatch_Tasks(void) {

    void (*pFunc)(void) = 0;
    uint32_t period = 0;

    uint32_t primask = SCH_Enter_Critical();

    if (
        SCH_count > 0 &&
        SCH_tasks_G[0].Delay == 0
    ) {

        pFunc = SCH_tasks_G[0].pTask;
        period = SCH_tasks_G[0].Period;

        /* Xoa task khoi danh sach truoc khi goi callback */
        SCH_Remove_Task(0);
    }

    SCH_Exit_Critical(primask);

    if (pFunc != 0) {

        /* Thuc thi callback khi khong khoa interrupt */
        pFunc();

        /* Dang ky lai task neu la periodic task */
        if (period > 0) {
            SCH_Add_Task(pFunc, period, period);
        }
    }
}
