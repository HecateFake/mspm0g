#ifndef __LOOP_H__
#define __LOOP_H__

typedef struct
{
    float xErr;
    float yErr;
}VERR;

extern VERR vErr;

extern PID yawPid;
extern PID pitPid;

void controlInit(void);
void TIMA1_PIT(void);

#endif /* __LOOP_H__ */
