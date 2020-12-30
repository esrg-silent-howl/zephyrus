#include <math.h>
#include "_imu.h"
#include "i2c.h"

#define M_PI   3.14159265358979323846264338327950288
#define RAD_TO_DEG(x) (x*180/M_PI)

#define DEVICE_REG					0x75
#define PWR_MGMT_1_REG			0x6B
#define SMPLRT_DIV_REG			0x19
#define ACCEL_CONFIG_REG		0x1C
#define ACCEL_XOUT_H_REG		0x3B
#define GYRO_CONFIG_REG			0x1B
#define GYRO_XOUT_H_REG			0x43

/*!< Setup MPU6050 */
#define IMU_ADDR 						0xD0
#define IMU_ACK  						0x68

const float Accel_Z_corrector = 14418.0;

uint32_t timer;

uint8_t imuRX[IMU_REG_COUNT] = {0};

Kalman_t KalmanX = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f
};

Kalman_t KalmanY = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f,
};

inline float to_degrees(float radians) {
    return radians * (180.0 / M_PI);
}

uint8_t IMU_Init(I2C_HandleTypeDef* hI2C) {

#define IMU_CONFIG_TIMEOUT 	1
	
	uint8_t check;
	uint8_t data;

	HAL_I2C_Mem_Read(hI2C, IMU_ADDR, DEVICE_REG, 1, &check, 1, IMU_CONFIG_TIMEOUT);

	if (check == IMU_ACK)  // 0x68 will be returned by the sensor if everything goes well
	{
		// power management register 0X6B we should write all 0's to wake the sensor up
		data = 0;
		if (HAL_I2C_Mem_Write(hI2C, IMU_ADDR, PWR_MGMT_1_REG, 1, &data, 1, IMU_CONFIG_TIMEOUT) != HAL_OK) 
			return 0;
		
		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		data = 0x07;
		if (HAL_I2C_Mem_Write(hI2C, IMU_ADDR, SMPLRT_DIV_REG, 1, &data, 1, IMU_CONFIG_TIMEOUT) != HAL_OK) 
			return 0;
		
		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0
		data = 0x00;
		if (HAL_I2C_Mem_Write(hI2C, IMU_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, IMU_CONFIG_TIMEOUT) != HAL_OK) 
			return 0;
		
		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0
		data = 0x00;
		if (HAL_I2C_Mem_Write(hI2C, IMU_ADDR, GYRO_CONFIG_REG, 1, &data, 1, IMU_CONFIG_TIMEOUT) != HAL_OK) 
			return 0;
		
		return 1;
	}

	return 0;
}

float Kalman_getAngle(Kalman_t *Kalman, float newAngle, float newRate, float dt) {
    float rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_bias * dt;

    float S = Kalman->P[0][0] + Kalman->R_measure;
    float K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    float y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    float P00_temp = Kalman->P[0][0];
    float P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};

HAL_StatusTypeDef IMU_dataRequest(I2C_HandleTypeDef *I2Cx){
	
/*!< Read 14 bytes of data starting from the first register (ACCEL_XOUT_H) */
return HAL_I2C_Mem_Read_IT(I2Cx, IMU_ADDR, ACCEL_XOUT_H_REG, 1, imuRX, IMU_REG_COUNT);
}

void IMU_dataFetch(imu_t *data_struct) {

    data_struct->Accel_X_RAW = (int16_t) (imuRX[0] 	<< 8 | imuRX[1]);
    data_struct->Accel_Y_RAW = (int16_t) (imuRX[2] 	<< 8 | imuRX[3]);
    data_struct->Accel_Z_RAW = (int16_t) (imuRX[4] 	<< 8 | imuRX[5]);

    data_struct->Gyro_X_RAW 	= (int16_t)	(imuRX[8]		<< 8 | imuRX[9]);
    data_struct->Gyro_Y_RAW 	= (int16_t)	(imuRX[10] 	<< 8 | imuRX[11]);
    data_struct->Gyro_Z_RAW 	= (int16_t)	(imuRX[12] 	<< 8 | imuRX[13]);

    data_struct->Ax = data_struct->Accel_X_RAW 	/ 16384.0;
    data_struct->Ay = data_struct->Accel_Y_RAW 	/ 16384.0;
    data_struct->Az = data_struct->Accel_Z_RAW 	/ Accel_Z_corrector;
	
    data_struct->Gx = data_struct->Gyro_X_RAW 	/ 131.0;
    data_struct->Gy = data_struct->Gyro_Y_RAW 	/ 131.0;
    data_struct->Gz = data_struct->Gyro_Z_RAW 	/ 131.0;

    /*!< Kalman filter computations */
    float dt = (float) (HAL_GetTick() - timer) / 1000;
    timer = HAL_GetTick();
    float roll;
    float roll_sqrt = sqrt(
            data_struct->Accel_X_RAW * data_struct->Accel_X_RAW + data_struct->Accel_Z_RAW * data_struct->Accel_Z_RAW);
    if (roll_sqrt != 0.0) {
        roll = RAD_TO_DEG(atan(data_struct->Accel_Y_RAW / roll_sqrt));
    } else {
        roll = 0.0;
    }
    float pitch = RAD_TO_DEG(atan2(-data_struct->Accel_X_RAW, data_struct->Accel_Z_RAW));
    if ((pitch < -90 && data_struct->KalmanAngleY > 90) || (pitch > 90 && data_struct->KalmanAngleY < -90)) {
        KalmanY.angle = pitch;
        data_struct->KalmanAngleY = pitch;
    } else {
        data_struct->KalmanAngleY = Kalman_getAngle(&KalmanY, pitch, data_struct->Gy, dt);
    }
    if (fabs(data_struct->KalmanAngleY) > 90)
        data_struct->Gx = -data_struct->Gx;
    data_struct->KalmanAngleX = Kalman_getAngle(&KalmanX, roll, data_struct->Gy, dt);
}

