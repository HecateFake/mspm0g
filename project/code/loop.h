#ifndef __LOOP_H__
#define __LOOP_H__

#include "pid.h"
#include <stdint.h>

typedef struct
{
    float xErr;
    float yErr;
}VERR;

typedef enum
{
    pos,
    spe,
    sto,
}control_mode_enum;

extern VERR vErr;

extern PID yawPid;
extern PID pitPid;

extern control_mode_enum controlMode;
extern float pitSpeed;
extern float yawSpeed;
extern float pitPos;
extern float yawPos;

extern uint8_t state;
extern float openLoopAng;

#define SIGN(x) ((x) > 0 ? 1 : 0)

void controlInit(void);
void TIMA1_PIT(void);

#endif /* __LOOP_H__ */
