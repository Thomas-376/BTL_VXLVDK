#include "traffic_light.h"
#include "global.h"

// =================== TRAFFIC LIGHT 1 ===================
void set_Traffic_1_Red(void) {
    HAL_GPIO_WritePin(TRAFFIC1_PIN1_GPIO_Port, TRAFFIC1_PIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TRAFFIC1_PIN2_GPIO_Port, TRAFFIC1_PIN2_Pin, GPIO_PIN_SET);
}

void set_Traffic_1_Green(void) {
    HAL_GPIO_WritePin(TRAFFIC1_PIN1_GPIO_Port, TRAFFIC1_PIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TRAFFIC1_PIN2_GPIO_Port, TRAFFIC1_PIN2_Pin, GPIO_PIN_RESET);
}

void set_Traffic_1_Yellow(void) {
    HAL_GPIO_WritePin(TRAFFIC1_PIN1_GPIO_Port, TRAFFIC1_PIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TRAFFIC1_PIN2_GPIO_Port, TRAFFIC1_PIN2_Pin, GPIO_PIN_SET);
}

// =================== TRAFFIC LIGHT 2 ===================
void set_Traffic_2_Red(void) {
    HAL_GPIO_WritePin(TRAFFIC2_PIN1_GPIO_Port, TRAFFIC2_PIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TRAFFIC2_PIN2_GPIO_Port, TRAFFIC2_PIN2_Pin, GPIO_PIN_SET);
}

void set_Traffic_2_Green(void) {
    HAL_GPIO_WritePin(TRAFFIC2_PIN1_GPIO_Port, TRAFFIC2_PIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TRAFFIC2_PIN2_GPIO_Port, TRAFFIC2_PIN2_Pin, GPIO_PIN_RESET);
}

void set_Traffic_2_Yellow(void) {
    HAL_GPIO_WritePin(TRAFFIC2_PIN1_GPIO_Port, TRAFFIC2_PIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TRAFFIC2_PIN2_GPIO_Port, TRAFFIC2_PIN2_Pin, GPIO_PIN_SET);
}

void update_Traffic_Light(int state) {
    switch (state) {
        case RED_GREEN:
            set_Traffic_1_Red();
            set_Traffic_2_Green();
            break;
        case RED_YELLOW:
            set_Traffic_1_Red();
            set_Traffic_2_Yellow();
            break;
        case GREEN_RED:
            set_Traffic_1_Green();
            set_Traffic_2_Red();
            break;
        case YELLOW_RED:
            set_Traffic_1_Yellow();
            set_Traffic_2_Red();
            break;
        default:
            HAL_GPIO_WritePin(TRAFFIC1_PIN1_GPIO_Port, TRAFFIC1_PIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(TRAFFIC1_PIN2_GPIO_Port, TRAFFIC1_PIN2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(TRAFFIC2_PIN1_GPIO_Port, TRAFFIC2_PIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(TRAFFIC2_PIN2_GPIO_Port, TRAFFIC2_PIN2_Pin, GPIO_PIN_RESET);
            break;
    }
}
