#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H

#define MAX_TIMER 4

extern int timer_flag[MAX_TIMER];

#define TIMER_CYCLE 10

void setTimer(int index, int duration);
void timerRun(void);

#endif
