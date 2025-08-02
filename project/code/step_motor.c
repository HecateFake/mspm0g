#include "zf_common_headfile.h"

int16_t step_motor_speed[2] = {0};
uint32_t step_motor_clk[2] = {0};
float step_motor_angle[2] = {0};
uint8_t step_motor_dir[2] = {0};

void step_motor_init(MOTOR_enum motor) {
  if (motor == STEP_MOTOR_1) {
    uart_init(STEP_MOTOR1, STEP_MOTOR1_baudrate, STEP_MOTOR1_TX,
              STEP_MOTOR1_RX);
  } else if (motor == STEP_MOTOR_2) {
    uart_init(STEP_MOTOR2, STEP_MOTOR2_baudrate, STEP_MOTOR2_TX,
              STEP_MOTOR2_RX);
  }
}

/**
 * @brief    使能信号控制
 * @param    state ：使能状态     ，true为使能电机，false为关闭电机
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_enable(MOTOR_enum motor, bool state) {
  uint8_t cmd[16] = {0};

  cmd[0] = 0x01; // 电机地址
  cmd[1] = 0xF3;
  cmd[2] = 0xAB;
  cmd[3] = (uint8_t)state;
  cmd[4] = false;
  cmd[5] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 6);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 6);
  }
}

/**
 * @brief    立即停止（所有控制模式都通用）
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_stop(MOTOR_enum motor) {
  uint8_t cmd[16] = {0};

  cmd[0] = 0x01;
  cmd[1] = 0xFE;
  cmd[2] = 0x98;
  cmd[3] = false;
  cmd[4] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 5);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 5);
  }
}

/**
 * @brief    速度模式
 * @param    dir ：方向       ，0为CW，其余值为CCW
 * @param    speed ：速度       ，范围0 - 5000RPM
 * @param    acc ：加速度     ，范围0 - 255，注意：0是直接启动
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_set_speed(MOTOR_enum motor, uint8_t dir, uint16_t speed,
                          uint8_t acc) {
  uint8_t cmd[16] = {0};

  cmd[0] = 0x01;
  cmd[1] = 0xF6;
  cmd[2] = dir;
  cmd[3] = (uint8_t)(speed >> 8);
  cmd[4] = (uint8_t)(speed >> 0);
  cmd[5] = acc;
  cmd[6] = false;
  cmd[7] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 8);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 8);
  }
}
/**
 * @brief    位置模式
 * @param    dir ：方向        ，0为CW，其余值为CCW
 * @param    speed ：速度(RPM)   ，范围0 - 5000RPM
 * @param    acc ：加速度      ，范围0 - 255，注意：0是直接启动
 * @param    clk ：脉冲数      ，范围0- (2^32 - 1)个
 * @param    raF ：相位/绝对标志，false为相对运动，true为绝对值运动
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_set_pos(MOTOR_enum motor, uint8_t dir, uint16_t speed,
                        uint8_t acc, uint32_t clk, bool raF) {
  uint8_t cmd[16] = {0};

  cmd[0] = 0x01;
  cmd[1] = 0xFD;
  cmd[2] = dir;
  cmd[3] = (uint8_t)(speed >> 8);
  cmd[4] = (uint8_t)(speed >> 0);
  cmd[5] = acc;
  cmd[6] = (uint8_t)(clk >> 24);
  cmd[7] = (uint8_t)(clk >> 16);
  cmd[8] = (uint8_t)(clk >> 8);
  cmd[9] = (uint8_t)(clk >> 0);
  cmd[10] = raF;
  cmd[11] = false;
  cmd[12] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 13);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 13);
  }
}

/**
 * @brief    设置单圈回零的零点位置
 * @param    svF   ：是否存储标志，false为不存储，true为存储
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_set_zero(MOTOR_enum motor, bool svF) {
  uint8_t cmd[16] = {0};

  cmd[0] = 0x01;
  cmd[1] = 0x93;
  cmd[2] = 0x88;
  cmd[3] = svF;
  cmd[4] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 5);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 5);
  }
}

/**
 * @brief    修改回零参数
 * @param    svF   ：是否存储标志，false为不存储，true为存储
 * @param    o_mode
 * ：回零模式，0为单圈就近回零，1为单圈方向回零，2为多圈无限位碰撞回零，3为多圈有限位开关回零
 * @param    o_dir  ：回零方向，0为CW，其余值为CCW
 * @param    o_speed  ：回零速度，单位：RPM（转/分钟）
 * @param    o_tm   ：回零超时时间，单位：毫秒
 * @param    sl_vel ：无限位碰撞回零检测转速，单位：RPM（转/分钟）
 * @param    sl_ma  ：无限位碰撞回零检测电流，单位：Ma（毫安）
 * @param    sl_ms  ：无限位碰撞回零检测时间，单位：Ms（毫秒）
 * @param    potF   ：上电自动触发回零，false为不使能，true为使能
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_set_zero_para(MOTOR_enum motor, bool svF, uint8_t o_mode,
                              uint8_t o_dir, uint16_t o_speed, uint32_t o_tm,
                              uint16_t sl_vel, uint16_t sl_ma, uint16_t sl_ms,
                              bool potF) {
  uint8_t cmd[32] = {0};

  cmd[0] = 0x01;
  cmd[1] = 0x4C;
  cmd[2] = 0xAE;
  cmd[3] = svF;
  cmd[4] = o_mode;
  cmd[5] = o_dir;
  cmd[6] = (uint8_t)(o_speed >> 8);
  cmd[7] = (uint8_t)(o_speed >> 0);
  cmd[8] = (uint8_t)(o_tm >> 24);
  cmd[9] = (uint8_t)(o_tm >> 16);
  cmd[10] = (uint8_t)(o_tm >> 8);
  cmd[11] = (uint8_t)(o_tm >> 0);
  cmd[12] = (uint8_t)(sl_vel >> 8);
  cmd[13] = (uint8_t)(sl_vel >> 0);
  cmd[14] = (uint8_t)(sl_ma >> 8);
  cmd[15] = (uint8_t)(sl_ma >> 0);
  cmd[16] = (uint8_t)(sl_ms >> 8);
  cmd[17] = (uint8_t)(sl_ms >> 0);
  cmd[18] = potF;
  cmd[19] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 20);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 20);
  }
}

/**
 * @brief    触发回零
 * @param    o_mode
 * ：回零模式，0为单圈就近回零，1为单圈方向回零，2为多圈无限位碰撞回零，3为多圈有限位开关回零
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_return_zero(MOTOR_enum motor, uint8_t o_mode) {
  uint8_t cmd[16] = {0};

  cmd[0] = 0x01;
  cmd[1] = 0x9A;
  cmd[2] = o_mode;
  cmd[3] = false;
  cmd[4] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 5);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 5);
  }
}
/**
 * @brief    位置角度清零(将当前位置角度、位置误差、脉冲数等全部清零)
 * @retval   地址 + 功能码 + 命令状态 + 校验字节
 */
void step_motor_clear_pos(MOTOR_enum motor) {
  uint8_t cmd[16] = {0};

  // 装载命令
  cmd[0] = 0x01;
  cmd[1] = 0x0A;
  cmd[2] = 0x6D;
  cmd[3] = 0x6B;
  if (motor == STEP_MOTOR_1) {
    uart_write_buffer(STEP_MOTOR1, cmd, 4);
  } else if (motor == STEP_MOTOR_2) {
    uart_write_buffer(STEP_MOTOR2, cmd, 4);
  }
}
