#ifndef CODE_TRACE_H_
#define CODE_TRACE_H_
#include "zf_common_headfile.h"
#include <stdint.h>

typedef enum {
  CLOCKWISE,
  ANTICLOCE,
} trace_mode_enum;

extern trace_mode_enum trace_mode;
extern int duty;
extern int turn_flag;
extern int trace_round_num;
extern int turn_num;

#define GPIO_PIN_1 B18
#define GPIO_PIN_2 B17
#define GPIO_PIN_3 B27
#define GPIO_PIN_4 A25
#define GPIO_PIN_5 A26
#define MOTOR_PWMA PWM_TIM_A0_CH0_B8
#define MOTOR_GPIO_AIN1 B13
#define MOTOR_GPIO_AIN2 B9
#define MOTOR_PWMB PWM_TIM_G7_CH1_A27 // B left
#define MOTOR_GPIO_BIN1 A29
#define MOTOR_GPIO_BIN2 A28
#define key_1 A30
#define key_2 A31
#define key_3 B0
#define key_4 B1

extern uint8 a1;
extern uint8 a2;
extern uint8 a3;
extern uint8 a4;
extern uint8 a5;
extern int a;
extern int b;
extern int c;
extern int d;
void show_sense_level();
void trace_get_duty();
void trace_control();
void trace_init();
#endif /*CODE_TRACE_H_*/