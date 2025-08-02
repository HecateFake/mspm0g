#include "zf_common_headfile.h"

trace_mode_enum trace_mode;
int duty = 0;
int turn_flag = 0;
int turn_num = 0;
int trace_round_num = 5;

uint8 a1;
uint8 a2;
uint8 a3;
uint8 a4;
uint8 a5;
int a;
int b;
int c;
int d;
void trace_get_duty() {

  if (turn_flag == 0) {
    switch (trace_mode) {
    case 0:
      if (a1 == 1 && a2 == 1 && a3 == 1)
        turn_flag = 1;
      break;
    case 1:
      if (a3 == 1 && a4 == 1 && a5 == 1)
        turn_flag = 1;
      break;
    }
    /*  if	( a1 == 1 && a2 == 0 && a3 == 0 ) duty = -2000+500 ;
  else if	( a1 == 1 && a2 == 1 && a3 == 0 ) duty = -1400+400 ;
  else if	( a1 == 0 && a2 == 1 && a3 == 0 ) duty = -1000+300 ;
  else if	( a1 == 0 && a2 == 1 && a3 == 1 ) duty = -200+50 ;
  else if	( a3 == 1 && a4 == 1 && a5 == 0 ) duty = 200-50 ;
  else if	( a3 == 0 && a4 == 1 && a5 == 0 ) duty = 1000-300 ;
  else if	( a3 == 0 && a4 == 1 && a5 == 1 ) duty = 1400-400 ;
  else if	( a3 == 0 && a4 == 0 && a5 == 1 ) duty = 2000-500 ;*/
    if (a1 == 0 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0)
      duty = -b;
    else if (a1 == 1 && a2 == 0 && a3 == 0)
      duty = -a;
    else if (a1 == 1 && a2 == 1 && a3 == 0)
      duty = -520;
    else if (a1 == 0 && a2 == 1 && a3 == 0)
      duty = -c;
    else if (a1 == 0 && a2 == 1 && a3 == 1)
      duty = -d * 0.8;

    else if (a1 == 0 && a2 == 0 && a3 == 1 && a4 == 0 && a5 == 0) {
      duty = -50;
    }

    else if (a3 == 1 && a4 == 1 && a5 == 0)
      duty = d * 0.6;
    else if (a3 == 0 && a4 == 1 && a5 == 0)
      duty = c * 0.8;
    else if (a3 == 0 && a4 == 1 && a5 == 1)
      duty = 520 * 0.8;
    else if (a3 == 0 && a4 == 0 && a5 == 1)
      duty = a * 0.8;
    else
      duty = 0;
  }

  else if (turn_flag == 1) {

    if (turn_num == (trace_round_num * 4) - 1)
      car_state = CAR_EDITING;
    else {
      turn_num++;
      turn_flag = 2;
    }

  } else if (turn_flag == 2) {
    if (fabs(imu.yaw) >= (imu.tar_angle)) {
      turn_flag = 0;
    }
  }
}

void show_sense_level() {
  a1 = gpio_get_level(B18);
  a2 = gpio_get_level(B17);
  a3 = gpio_get_level(B27);
  a4 = gpio_get_level(A25);
  a5 = gpio_get_level(A26);
  tft180_show_int(0, 0, a1, 4);
  tft180_show_int(20, 0, a2, 4);
  tft180_show_int(40, 0, a3, 4);
  tft180_show_int(60, 0, a4, 4);
  tft180_show_int(80, 0, a5, 4);
  tft180_show_int(0, 20, turn_flag, 3);
  tft180_show_int(30, 20, duty, 4);
  tft180_show_int(60, 20, turn_num, 2);
}
void trace_control() {
  trace_get_duty();
  switch (trace_mode) {
  case CLOCKWISE:
    switch (turn_flag) {
    case 0:
      pwm_set_duty(MOTOR_PWMA, 2615 - 1.3f * duty);
      pwm_set_duty(MOTOR_PWMB, 2585 + 1.3f * duty);
      gpio_set_level(MOTOR_GPIO_BIN1, 0);
      gpio_set_level(MOTOR_GPIO_BIN2, 1);
      break;
    case 1:
      pwm_set_duty(MOTOR_PWMA, 1950);
      pwm_set_duty(MOTOR_PWMB, 1900);
      gpio_set_level(MOTOR_GPIO_BIN1, 0);
      gpio_set_level(MOTOR_GPIO_BIN2, 1);
      break;
    case 2:
      pwm_set_duty(MOTOR_PWMA, 4000);
      pwm_set_duty(MOTOR_PWMB, 2000);
      gpio_set_level(MOTOR_GPIO_BIN1, 1);
      gpio_set_level(MOTOR_GPIO_BIN2, 0);
      break;
    }
    break;
  case ANTICLOCE:
    switch (turn_flag) {
    case 0:
      pwm_set_duty(MOTOR_PWMA, 2595 + duty);
      pwm_set_duty(MOTOR_PWMB, 2605 - duty);
      gpio_set_level(MOTOR_GPIO_AIN1, 0);
      gpio_set_level(MOTOR_GPIO_AIN2, 1);
      break;
    case 1:
      pwm_set_duty(MOTOR_PWMA, 1900);
      pwm_set_duty(MOTOR_PWMB, 1950);
      gpio_set_level(MOTOR_GPIO_AIN1, 0);
      gpio_set_level(MOTOR_GPIO_AIN2, 1);
      break;
    case 2:
      pwm_set_duty(MOTOR_PWMA, 2000);
      pwm_set_duty(MOTOR_PWMB, 4000);
      gpio_set_level(MOTOR_GPIO_AIN1, 1);
      gpio_set_level(MOTOR_GPIO_AIN2, 0);
      break;
    }
    break;
  }
}

void trace_init() {
  gpio_init(GPIO_PIN_1, GPI, GPIO_HIGH, GPI_PULL_UP);
  gpio_init(GPIO_PIN_2, GPI, GPIO_HIGH, GPI_PULL_UP);
  gpio_init(GPIO_PIN_3, GPI, GPIO_HIGH, GPI_PULL_UP);
  gpio_init(GPIO_PIN_4, GPI, GPIO_HIGH, GPI_PULL_UP);
  gpio_init(GPIO_PIN_5, GPI, GPIO_HIGH, GPI_PULL_UP);
  pwm_init(MOTOR_PWMA, 17000, 0);
  pwm_init(MOTOR_PWMB, 17000, 0);
  gpio_init(MOTOR_GPIO_AIN1, GPO, 0, GPO_PUSH_PULL);
  gpio_init(MOTOR_GPIO_AIN2, GPO, 1, GPO_PUSH_PULL);
  gpio_init(MOTOR_GPIO_BIN1, GPO, 0, GPO_PUSH_PULL);
  gpio_init(MOTOR_GPIO_BIN2, GPO, 1, GPO_PUSH_PULL);
  trace_round_num = menu1[1].value;
  trace_mode = menu1[2].value;
  imu.tar_angle = menu1[3].value;
  a = menu1[4].value;
  b = menu1[5].value;
  c = menu1[6].value;
  d = menu1[7].value;
}
