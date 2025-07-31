#ifndef __CAMERA_H__
#define __CAMERA_H__

#define cameraUart UART_1
#define cameraUartTx UART1_TX_A8
#define cameraUartRx UART1_RX_A9
#define cameraUartBaudrate 115200

#define cameraUartInit() uart_init(cameraUart, cameraUartBaudrate, cameraUartTx, cameraUartRx)

// 数据结构定义
typedef struct 
{
    int x;
    int y;
    int status;  // 1=找到, 0=未找到
} TargetData;

typedef struct
{
    int x;
    int y;
    int status;  // 1=找到, 0=未找到
} LaserData;

extern uint8 uart_buffer[128];
extern int buffer_index;
extern TargetData target_data;
extern LaserData laser_data;
extern int data_ready_flag;

int parse_received_data(uint8* data_string);

#endif /* __CAMERA_H__ */
