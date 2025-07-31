#include "zf_common_headfile.h"

// 全局变量
uint8 uart_buffer[128];
int buffer_index = 0;
TargetData target_data = {0};
LaserData laser_data = {0};
int data_ready_flag = 0;

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
