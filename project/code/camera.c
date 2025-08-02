#include "zf_common_headfile.h"

// 全局变量
uint8 uart_buffer[128];
uint8 fifo_get_data[128];                                                       // fifo 输出读出缓冲区
int buffer_index = 0;
TargetData target_data = {0};
LaserData laser_data = {0};
int data_ready_flag = 0;

uint8 get_data = 0;                                                             // 接收数据变量
uint32 fifo_data_count = 0;                                                     // fifo 数据个数
fifo_struct camera_data_fifo;                                                   // 相机数据FIFO

/**
 * 解析接收到的数据
 * 格式: "T:x,y,status;L:x,y,status\n"
 */
int parse_received_data(uint8* data_string) 
{
    
    // 将uint8类型转换为char类型用于字符串处理
    char* data_str = (char*)data_string;
    char* target_part = NULL;
    char* laser_part = NULL;
    char* saveptr;
    
    // 分割字符串，以分号为分隔符
    target_part = _strtok_r(data_str, ";", &saveptr);
    laser_part = _strtok_r(NULL, ";", &saveptr);
    
    if (target_part == NULL || laser_part == NULL) {
        return 1; // 数据格式错误
    }
    
    // 解析靶心数据 "T:x,y,status"
    if (strncmp(target_part, "T:", 2) == 0) {
        char* token;
        char* target_saveptr;
        char* target_data_str = target_part + 2; // 跳过 "T:"
        
        token = _strtok_r(target_data_str, ",", &target_saveptr);
        if (token) target_data.x = atoi(token);
        
        token = _strtok_r(NULL, ",", &target_saveptr);
        if (token) target_data.y = atoi(token);
        
        token = _strtok_r(NULL, ",", &target_saveptr);
        if (token) target_data.status = atoi(token);
    }
    
    // 解析激光数据 "L:x,y,status"
    if (strncmp(laser_part, "L:", 2) == 0) {
        char* token;
        char* laser_saveptr;
        char* laser_data_str = laser_part + 2; // 跳过 "L:"
        
        token = _strtok_r(laser_data_str, ",", &laser_saveptr);
        if (token) laser_data.x = atoi(token);
        
        token = _strtok_r(NULL, ",", &laser_saveptr);
        if (token) laser_data.y = atoi(token);
        
        token = _strtok_r(NULL, ",", &laser_saveptr);
        if (token) laser_data.status = atoi(token);
    }

    return 0; // 解析成功
}

/**
 * 查找完整数据包
 * 返回: 完整数据包的长度，0表示没有完整数据包
 */
uint32 find_complete_packet(uint8* buffer, uint32 length)
{
    for(uint32 i = 0; i < length; i++)
    {
        if(buffer[i] == '\n')  // 找到数据包结束符
        {
            return i + 1;  // 返回包括结束符的完整长度
        }
    }
    return 0;  // 没有找到完整数据包
}

void uart_rx_interrupt_handler (void)
{ 
    //tft180_show_int(0, 80, 1, 1);

    uart_query_byte(cameraUart, &get_data);                                     
    
    // 检查FIFO空间
    uint32 fifo_free_space = sizeof(uart_buffer) - fifo_used(&camera_data_fifo);
    if(fifo_free_space > 1)
    {
        fifo_write_buffer(&camera_data_fifo, &get_data, 1);                         
    }
    else
    {
        // FIFO满了，清空一部分旧数据
        fifo_clear(&camera_data_fifo);
        fifo_write_buffer(&camera_data_fifo, &get_data, 1);
    }
}

/**
 * 处理FIFO中的数据
 * 在主循环中调用
 */
void process_camera_data(void)
{
    fifo_data_count = fifo_used(&camera_data_fifo);                             // 查看 fifo 是否有数据
    if(fifo_data_count > 0)                                                     // 读取到数据了
    {
        // 读取FIFO中的所有数据，但不清空FIFO
        uint32 read_length = fifo_data_count;
        if(read_length > sizeof(fifo_get_data) - 1)
        {
            read_length = sizeof(fifo_get_data) - 1;
        }
        
        fifo_read_buffer(&camera_data_fifo, fifo_get_data, &read_length, FIFO_READ_ONLY);
        
        // 查找完整的数据包
        uint32 packet_length = find_complete_packet(fifo_get_data, read_length);
        if(packet_length > 0)
        {
            // 找到完整数据包，添加字符串结束符
            fifo_get_data[packet_length - 1] = '\0'; // 将\n替换为\0
            
            // 解析数据
            if(parse_received_data(fifo_get_data) == 0)
            {
                data_ready_flag = 1; // 标记数据解析成功
            }
            
            // 从FIFO中清除已处理的数据
            uint32 clear_length = packet_length;
            fifo_read_buffer(&camera_data_fifo, fifo_get_data, &clear_length, FIFO_READ_AND_CLEAN);
        }
        // 如果没有完整数据包，继续等待更多数据
    }
}

