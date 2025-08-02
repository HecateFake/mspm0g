/*
 * imu.c
 *
 *  Created on: 2025��8��1��
 *      Author: daishuying
 */
#include "zf_common_headfile.h"
#define g 9.8066
#define pai 3.1514
#define imu_pit 0.033
int imu_num=100;

float k=0.095;
imu660_struct imu;

//����Ʈ
void calculate_offset() {
    for (int i = 0; i < imu_num; i++) {
        imu660rb_get_gyro();
        imu.offset_g_z += imu660rb_gyro_z;
        system_delay_ms(1);
    }
    imu.offset_g_z /= imu_num ;
		
}

void imu_data_process(){
    imu660rb_get_gyro();
    imu.yaw += (imu660rb_gyro_transition(imu660rb_gyro_z - imu.offset_g_z)*imu_pit);
}

