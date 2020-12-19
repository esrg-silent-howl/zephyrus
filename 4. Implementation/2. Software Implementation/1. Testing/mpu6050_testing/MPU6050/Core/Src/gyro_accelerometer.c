/************************************************************************
* FILENAME :        gyro_accelerometer.c
*
* DESCRIPTION :
*       Source file for MPU-6050 interface
*
* PUBLIC FUNCTIONS :
*
* NOTES :
*      
*
* AUTHOR :
*
*
***/

#include <math.h>
#include "gyro_accelerometer.h"
#include "i2c.h"

#define M_PI   3.14159265358979323846264338327950288
#define RAD_TO_DEG(x) (x*180/M_PI)

#define DEVICE_REG	0x75
#define PWR_MGMT_1_REG	0x6B
#define SMPLRT_DIV_REG	0x19
#define ACCEL_CONFIG_REG	0x1C
#define ACCEL_XOUT_H_REG	0x3B
#define GYRO_CONFIG_REG	0x1B
#define GYRO_XOUT_H_REG	0x43

// Setup MPU6050
#define MPU6050_ADDR 0xD0
#define MPU6050_ACK  0x68
const double Accel_Z_corrector = 14418.0;

uint32_t timer;

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

inline double to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}

//void XferCpltCallback(DMA_HandleTypeDef* _hdma){
//	if(_hdma->Instance == DMA1_Stream0)
//		dma_buffer_flag = 1;
//}

//void XferCpltCallback(DMA_HandleTypeDef* _hdma) {
//		if(_hdma->Instance == DMA1_Stream0)
//			dma_buffer_flag = 1;
//}

uint8_t MPU6050_Init(I2C_HandleTypeDef* hI2C) {
    uint8_t check;
    uint8_t Data;

		
		//HAL_StatusTypeDef state = HAL_DMA_RegisterCallback (hI2C->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, XferCpltCallback);
	
		//HAL_DMA_Init(hI2C->hdmarx);
		//if(i2c_rx_flag)
		HAL_I2C_Mem_Read_IT(hI2C, MPU6050_ADDR, DEVICE_REG, 1, &check, 1);
		while (!i2c_rx_flag);
		i2c_rx_flag = 0;

    if (check == MPU6050_ACK)  // 0x68 will be returned by the sensor if everything goes well
    {
        // power management register 0X6B we should write all 0's to wake the sensor up
        Data = 0;
        HAL_StatusTypeDef state1 = HAL_I2C_Mem_Write_IT(hI2C, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1);
				while (!i2c_tx_flag);
				i2c_tx_flag = 0;

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
        Data = 0x07;
        HAL_StatusTypeDef state2 = HAL_I2C_Mem_Write_IT(hI2C, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1);
				while (!i2c_tx_flag);
				i2c_tx_flag = 0;
			
        // Set accelerometer configuration in ACCEL_CONFIG Register
        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0
        Data = 0x00;
        HAL_StatusTypeDef state3 = HAL_I2C_Mem_Write_IT(hI2C, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1);
				while (!i2c_tx_flag);
				i2c_tx_flag = 0;
			
        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0
        Data = 0x00;
        HAL_StatusTypeDef state4 = HAL_I2C_Mem_Write_IT(hI2C, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1);
				while (!i2c_tx_flag);
				i2c_tx_flag = 0;
				
        return 1;
    }
    return 0;
}


void MPU6050_Read_Accel(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) {
    uint8_t Rec_Data[6] = {0};

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register
    HAL_StatusTypeDef state = HAL_I2C_Mem_Read_IT(I2Cx, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6);
		while (!i2c_rx_flag);
		i2c_rx_flag = 0;
	
    DataStruct->Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into acceleration in 'g'
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 16384.0
         for more details check ACCEL_CONFIG Register              ****/

    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0;
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0;
    DataStruct->Az = DataStruct->Accel_Z_RAW / Accel_Z_corrector;
}


void MPU6050_Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) {
    uint8_t Rec_Data[6] = {0};

    // Read 6 BYTES of data starting from GYRO_XOUT_H register

    HAL_StatusTypeDef state = HAL_I2C_Mem_Read_IT(I2Cx, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6);
		while (!i2c_rx_flag);
		i2c_rx_flag = 0;

    DataStruct->Gyro_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Gyro_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Gyro_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into dps (�/s)
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 131.0
         for more details check GYRO_CONFIG Register              ****/

    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;
}

void MPU6050_Read_All(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) {
    uint8_t Rec_Data[14] = {0};
    // Read 14 BYTES of data starting from ACCEL_XOUT_H register

    HAL_StatusTypeDef state = HAL_I2C_Mem_Read_IT(I2Cx, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14);
		while (!i2c_rx_flag);
		i2c_rx_flag = 0;

    DataStruct->Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

    DataStruct->Gyro_X_RAW = (int16_t) (Rec_Data[8] << 8 | Rec_Data[9]);
    DataStruct->Gyro_Y_RAW = (int16_t) (Rec_Data[10] << 8 | Rec_Data[11]);
    DataStruct->Gyro_Z_RAW = (int16_t) (Rec_Data[12] << 8 | Rec_Data[13]);

    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0;
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0;
    DataStruct->Az = DataStruct->Accel_Z_RAW / Accel_Z_corrector;
    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;

    // Kalman angle solve
    double dt = (double) (HAL_GetTick() - timer) / 1000;
    timer = HAL_GetTick();
    double roll;
    double roll_sqrt = sqrt(
            DataStruct->Accel_X_RAW * DataStruct->Accel_X_RAW + DataStruct->Accel_Z_RAW * DataStruct->Accel_Z_RAW);
    if (roll_sqrt != 0.0) {
        roll = RAD_TO_DEG(atan(DataStruct->Accel_Y_RAW / roll_sqrt));
    } else {
        roll = 0.0;
    }
    double pitch = RAD_TO_DEG(atan2(-DataStruct->Accel_X_RAW, DataStruct->Accel_Z_RAW));
    if ((pitch < -90 && DataStruct->KalmanAngleY > 90) || (pitch > 90 && DataStruct->KalmanAngleY < -90)) {
        KalmanY.angle = pitch;
        DataStruct->KalmanAngleY = pitch;
    } else {
        DataStruct->KalmanAngleY = Kalman_getAngle(&KalmanY, pitch, DataStruct->Gy, dt);
    }
    if (fabs(DataStruct->KalmanAngleY) > 90)
        DataStruct->Gx = -DataStruct->Gx;
    DataStruct->KalmanAngleX = Kalman_getAngle(&KalmanX, roll, DataStruct->Gy, dt);

}

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt) {
    double rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_bias * dt;

    double S = Kalman->P[0][0] + Kalman->R_measure;
    double K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    double y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    double P00_temp = Kalman->P[0][0];
    double P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};

