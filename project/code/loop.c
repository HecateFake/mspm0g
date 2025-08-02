#include "camera.h"
#include "zf_common_headfile.h"
#include "zf_common_typedef.h"
#include <stdint.h>
uint8_t state = 3;
float openLoopAng = 0.0f;

VERR vErr;

PID yawPid;
PID pitPid;

control_mode_enum controlMode = pos;
float pitSpeed = 0.0f;
float yawSpeed = 0.0f;
float pitPos = 0.0f;
float yawPos = 0.0f;

void controlInit(void) {
  pidInit(1.0f, &yawPid, 0.0f, 0.25f, 0.0000f, 0.4);
  pidInit(1.0f, &pitPid, 0.0f, 0.2f, 0.0f, 0.4f);
}

uint8_t laserOn = 0;

float last_xErr = 0;
float last_yErr = 0;
uint8_t trueCount = 0;
void TIMA1_PIT(void) {
  if (state == 0) {
    if (openLoopAng == 0.0f) {
      state = 1;
    }

    else if (openLoopAng != 0) {
      step_motor_set_pos(STEP_MOTOR_2, !SIGN(openLoopAng), 5000, 0,
                         (uint32_t)fabs((openLoopAng / 90.0f) * 800.0f), 1);
      state = 1;
    }

  }

  else if (state == 1) {
    if (laserOn) {

      if (target_data.status && laser_data.status) {
        controlMode = pos;
        // 计算误差
        vErr.xErr = -(float)(target_data.x - laser_data.x);
        vErr.yErr = -(float)(target_data.y - laser_data.y);
        last_xErr = vErr.xErr;
        last_yErr = vErr.yErr;
        pidProcess(&yawPid, vErr.xErr);
        pidProcess(&pitPid, vErr.yErr);
        yawPos = yawPid.controlValue;
        pitPos = pitPid.controlValue;
      }

      else {
        controlMode = pos;
        pidProcess(&yawPid, last_xErr);
        pidProcess(&pitPid, last_yErr);
        yawPos = yawPid.controlValue;
        pitPos = pitPid.controlValue;
      }

      // if (vErr.xErr <= 10 && vErr.yErr <= 10) {
      //   controlMode = sto;
      // }
    }

    else if (!laserOn) {
      if (fabs(vErr.xErr) <= 10 && fabs(last_xErr) <= 10) {
        trueCount++;
        if (trueCount >= 15) {
          trueCount = 0;
          laserOn = 1;
          gpio_set_level(B5, GPIO_HIGH);
        }
      }

      else if (fabs(vErr.xErr) > 10 || fabs(last_xErr) > 10) {
        trueCount = 0;
        laserOn = 0;
        gpio_set_level(B5, GPIO_LOW);
      }

      if (target_data.status) {
        controlMode = pos;
        // 计算误差
        vErr.xErr = -(float)(target_data.x - 120.0f);
        vErr.yErr = -0;
        last_xErr = vErr.xErr;
        last_yErr = vErr.yErr;
        pidProcess(&yawPid, vErr.xErr);
        pidProcess(&pitPid, vErr.yErr);
        yawPos = yawPid.controlValue;
        pitPos = pitPid.controlValue;
      }

      else {
        controlMode = pos;
        pidProcess(&yawPid, last_xErr);
        pidProcess(&pitPid, last_yErr);
        yawPos = yawPid.controlValue;
        pitPos = pitPid.controlValue;
      }
    }
  }

  if (turn_flag == 2) {
    imu_data_process();
  } else {
    imu.yaw = 0;
  }
}