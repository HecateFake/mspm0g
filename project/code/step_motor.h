#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

#define STEP_MOTOR1 UART_2
#define STEP_MOTOR1_TX UART2_TX_B15
#define STEP_MOTOR1_RX UART2_RX_B16
#define STEP_MOTOR1_baudrate 115200

#define STEP_MOTOR2 UART_0
#define STEP_MOTOR2_TX UART0_TX_A0
#define STEP_MOTOR2_RX UART0_RX_A1
#define STEP_MOTOR2_baudrate 115200
    
typedef enum
{
    STEP_MOTOR_1 = 0,
    STEP_MOTOR_2 = 1,
}MOTOR_enum;

extern int16_t step_motor_speed[2];
extern uint32_t step_motor_clk[2];
extern float step_motor_angle[2];
extern uint8_t step_motor_dir[2];

void step_motor_init(MOTOR_enum motor);
void step_motor_enable(MOTOR_enum motor, bool state);
void step_motor_stop(MOTOR_enum motor);
void step_motor_set_speed(MOTOR_enum motor,uint8_t dir, uint16_t speed, uint8_t acc);
void step_motor_set_pos(MOTOR_enum motor, uint8_t dir, uint16_t speed, uint8_t acc, uint32_t clk, bool raF);
void step_motor_set_zero(MOTOR_enum motor, bool svF);
void step_motor_set_zero_para(MOTOR_enum motor, bool svF, uint8_t o_mode, uint8_t o_dir, uint16_t o_speed, uint32_t o_tm, uint16_t sl_vel, uint16_t sl_ma, uint16_t sl_ms, bool potF);
void step_motor_return_zero(MOTOR_enum motor, uint8_t o_mode);
void step_motor_clear_pos(MOTOR_enum motor);

#endif
