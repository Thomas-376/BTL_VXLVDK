#include "button.h"

GPIO_TypeDef* button_ports[NUM_BUTTONS] = {BUTTON1_PIN1_GPIO_Port, BUTTON1_PIN2_GPIO_Port, BUTTON2_PIN1_GPIO_Port, BUTTON2_PIN2_GPIO_Port};
uint16_t button_pins[NUM_BUTTONS] = {BUTTON1_PIN1_Pin, BUTTON1_PIN2_Pin, BUTTON2_PIN1_Pin, BUTTON2_PIN2_Pin};

static GPIO_PinState KeyReg0[NUM_BUTTONS];
static GPIO_PinState KeyReg1[NUM_BUTTONS];
static GPIO_PinState KeyReg2[NUM_BUTTONS];
static GPIO_PinState KeyReg3[NUM_BUTTONS];

static int TimeOutForKeyPress[NUM_BUTTONS];
static int button_pressed[NUM_BUTTONS] = {0, 0, 0, 0};
static int button_long_pressed[NUM_BUTTONS] = {0, 0, 0, 0};

void button_reading(void) {
    for(int i = 0; i < NUM_BUTTONS; i++) {
        KeyReg0[i] = KeyReg1[i];
        KeyReg1[i] = KeyReg2[i];
        KeyReg2[i] = HAL_GPIO_ReadPin(button_ports[i], button_pins[i]);

        if ((KeyReg0[i] == KeyReg1[i]) && (KeyReg1[i] == KeyReg2[i])) {
            if (KeyReg3[i] != KeyReg2[i]) {
                KeyReg3[i] = KeyReg2[i];
                if (KeyReg2[i] == GPIO_PIN_RESET) {
                    button_pressed[i] = 1;
                    TimeOutForKeyPress[i] = 200;
                }
            } else {
                if (KeyReg2[i] == GPIO_PIN_RESET) {
                    TimeOutForKeyPress[i]--;
                    if (TimeOutForKeyPress[i] == 0) {
                        button_long_pressed[i] = 1;
                        TimeOutForKeyPress[i] = 50;
                    }
                }
            }
        }
    }
}

int isButtonPressed(int index) {
    if(button_pressed[index] == 1) {
        button_pressed[index] = 0;
        return 1;
    }
    return 0;
}

int isButtonLongPressed(int index) {
    if(button_long_pressed[index] == 1) {
        button_long_pressed[index] = 0;
        return 1;
    }
    return 0;
}
