#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

#define NUM_BUTTONS 4

int isButtonPressed(int index);
int isButtonLongPressed(int index);
void button_reading(void);

#endif
