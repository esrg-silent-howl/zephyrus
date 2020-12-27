#include "stdint.h"
#include "i2c.h"

#define IMU_REG_COUNT	14

typedef struct {

    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    double KalmanAngleX;
    double KalmanAngleY;
} imu_t;

typedef struct {
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman_t;

extern uint8_t imuRX[IMU_REG_COUNT];

uint8_t IMU_Init(I2C_HandleTypeDef *I2Cx);

HAL_StatusTypeDef IMU_dataRequest(I2C_HandleTypeDef *I2Cx);

void IMU_dataFetch(imu_t *data_struct);

extern uint32_t i2c_tx_flag;
extern uint32_t i2c_rx_flag;
