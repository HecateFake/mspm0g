#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "zf_common_fifo.h"

#define cameraUart UART_1
#define cameraUartTx UART1_TX_B6
#define cameraUartRx UART1_RX_B7
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

// 全局变量声明
extern uint8 uart_buffer[128];
extern uint8 fifo_get_data[128];                                                       // fifo 输出读出缓冲区
extern int buffer_index;
extern TargetData target_data;
extern LaserData laser_data;
extern int data_ready_flag;

extern uint8 get_data;                                                             // 接收数据变量
extern uint32 fifo_data_count;                                                     // fifo 数据个数
extern fifo_struct camera_data_fifo;                                                   // 相机数据FIFO

int parse_received_data(uint8* data_string);
void uart_rx_interrupt_handler(void);
void process_camera_data(void);

#endif /* __CAMERA_H__ */
