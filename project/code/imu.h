/*
 * imu.h
 *
 *  Created on: 2025Äê8ÔÂ1ÈÕ
 *      Author: daishuying
 */

#ifndef CODE_IMU_H_
#define CODE_IMU_H_

#include "zf_common_headfile.h"


typedef struct {
    float offset_g_z;
    float yaw;
		float tar_angle;
} imu660_struct;

void imu_data_process();
void calculate_offset();

extern imu660_struct imu;
#endif /* CODE_IMU_H_ */
