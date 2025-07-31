#include "zf_common_headfile.h"

VERR vErr;

PID yawPid;
PID pitPid;

void controlInit(void)
{
    step_motor_clear_pos(STEP_MOTOR_1);
    step_motor_clear_pos(STEP_MOTOR_2);

    pidInit(0.0f, &yawPid, 0.0f, 0.0f, 0.0f, 0.0f);
    pidInit(0.0f, &pitPid, 0.0f, 0.0f, 0.0f, 0.0f);
}

#define SIGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
void TIMA1_PIT(void)
{
    vErr.xErr = target_data.x - laser_data.x;
    vErr.yErr = target_data.y - laser_data.y;

    pidProcess(&yawPid, vErr.xErr);
    pidProcess(&pitPid, vErr.yErr);

    step_motor_set_pos(STEP_MOTOR_1, SIGN(yawPid.controlValue), 5000, 0, (uint32_t)(fabs(yawPid.controlValue)), 1);
    step_motor_set_pos(STEP_MOTOR_2, SIGN(pitPid.controlValue), 5000, 0, (uint32_t)(fabs(pitPid.controlValue)), 1);
}