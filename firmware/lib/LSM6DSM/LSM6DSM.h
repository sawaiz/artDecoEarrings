#ifndef __LSM6DSM_H__
#define __LSM6DSM_H__

#include <Arduino.h>
#include <TinyWireM.h>

// Address of LSM6DSM accel/gyro when ADO = 0
#define LSM6DSM_ADDRESS_PULLDOWN  0x6A   // 1101010b
// Address of LSM6DSM accel/gyro when ADO = 1
#define LSM6DSM_ADDRESS_PULLUP    0x6B   // 1101011b

// LSM6DSM registers
#define LSM6DSM_FUNC_CFG_ACCESS           0x01 // Embedded functions configuration
#define LSM6DSM_SENSOR_SYNC_TIME_FRAME    0x04 // Sensor sync configuration
#define LSM6DSM_SENSOR_SYNC_RES_RATIO     0x05 // Sensor sync configuration
#define LSM6DSM_FIFO_CTRL1                0x06 // FIFO configuration
#define LSM6DSM_FIFO_CTRL2                0x07 // FIFO configuration
#define LSM6DSM_FIFO_CTRL3                0x08 // FIFO configuration
#define LSM6DSM_FIFO_CTRL4                0x09 // FIFO configuration
#define LSM6DSM_FIFO_CTRL5                0x0A // FIFO configuration
#define LSM6DSM_DRDY_PULSE_CFG            0x0B
#define LSM6DSM_INT1_CTRL                 0x0D // INT1 pin control
#define LSM6DSM_INT2_CTRL                 0x0E // INT1 pin control
#define LSM6DSM_WHO_AM_I                  0x0F // Who I am ID (fixed) [7:0] 0x6A
#define LSM6DSM_CTRL1_XL                  0x10 // Accelerometer and gyroscope control 
#define LSM6DSM_CTRL2_G                   0x11 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL3_C                   0x12 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL4_C                   0x13 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL5_C                   0x14 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL6_C                   0x15 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL7_G                   0x16 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL8_XL                  0x17 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL9_XL                  0x18 // Accelerometer and gyroscope control
#define LSM6DSM_CTRL10_C                  0x19 // Accelerometer and gyroscope control
#define LSM6DSM_MASTER_CONFIG             0x1A // I2C master configuration
#define LSM6DSM_WAKE_UP_SRC               0x1B // Interrupt
#define LSM6DSM_TAP_SRC                   0x1C // Interrupt
#define LSM6DSM_D6D_SRC                   0x1D // Interrupt
#define LSM6DSM_STATUS_REG                0x1E // User interface and OIS data status
#define LSM6DSM_OUT_TEMP_L                0x20 // Temperature low byte
#define LSM6DSM_OUT_TEMP_H                0x21 // Temperature high byte
#define LSM6DSM_OUTX_L_G                  0x22 // Gyroscope output
#define LSM6DSM_OUTX_H_G                  0x23 // Gyroscope output
#define LSM6DSM_OUTY_L_G                  0x24 // Gyroscope output
#define LSM6DSM_OUTY_H_G                  0x25 // Gyroscope output
#define LSM6DSM_OUTZ_L_G                  0x26 // Gyroscope output
#define LSM6DSM_OUTZ_H_G                  0x27 // Gyroscope output
#define LSM6DSM_OUTX_L_XL                 0x28 // Accelerometer output
#define LSM6DSM_OUTX_H_XL                 0x29 // Accelerometer output
#define LSM6DSM_OUTY_L_XL                 0x2A // Accelerometer output
#define LSM6DSM_OUTY_H_XL                 0x2B // Accelerometer output
#define LSM6DSM_OUTZ_L_XL                 0x2C // Accelerometer output
#define LSM6DSM_OUTZ_H_XL                 0x2D // Accelerometer output
#define LSM6DSM_SENSORHUB1_REG            0x2E // Sensor hub output
#define LSM6DSM_SENSORHUB2_REG            0x2F // Sensor hub output
#define LSM6DSM_SENSORHUB3_REG            0x30 // Sensor hub output
#define LSM6DSM_SENSORHUB4_REG            0x31 // Sensor hub output
#define LSM6DSM_SENSORHUB5_REG            0x32 // Sensor hub output
#define LSM6DSM_SENSORHUB6_REG            0x33 // Sensor hub output
#define LSM6DSM_SENSORHUB7_REG            0x34 // Sensor hub output
#define LSM6DSM_SENSORHUB8_REG            0x35 // Sensor hub output
#define LSM6DSM_SENSORHUB9_REG            0x36 // Sensor hub output
#define LSM6DSM_SENSORHUB10_REG           0x37 // Sensor hub output
#define LSM6DSM_SENSORHUB11_REG           0x38 // Sensor hub output
#define LSM6DSM_SENSORHUB12_REG           0x39 // Sensor hub output
#define LSM6DSM_FIFO_STATUS1              0x3A // FIFO status
#define LSM6DSM_FIFO_STATUS2              0x3B // FIFO status
#define LSM6DSM_FIFO_STATUS3              0x3C // FIFO status
#define LSM6DSM_FIFO_STATUS4              0x3D // FIFO status
#define LSM6DSM_FIFO_DATA_OUT_L           0x3E // FIFO Data
#define LSM6DSM_FIFO_DATA_OUT_H           0x3F // FIFO Data
#define LSM6DSM_TIMESTAMP0_REG            0x40 // Timestamp Ouput
#define LSM6DSM_TIMESTAMP1_REG            0x41 // Timestamp Ouput
#define LSM6DSM_TIMESTAMP2_REG            0x42 // Timestamp Ouput
#define LSM6DSM_STEP_TIMESTAMP_L          0x49 // Step counter timestamps
#define LSM6DSM_STEP_TIMESTAMP_H          0x4A // Step counter timestamps
#define LSM6DSM_STEP_COUNTER_L            0x4B // Step counter value
#define LSM6DSM_STEP_COUNTER_H            0x4C // Step counter value
#define LSM6DSM_SENSORHUB13_REG           0x4D // Sensor hub output
#define LSM6DSM_SENSORHUB14_REG           0x4E // Sensor hub output
#define LSM6DSM_SENSORHUB15_REG           0x4F // Sensor hub output
#define LSM6DSM_SENSORHUB16_REG           0x50 // Sensor hub output
#define LSM6DSM_SENSORHUB17_REG           0x51 // Sensor hub output
#define LSM6DSM_SENSORHUB18_REG           0x52 // Sensor hub output
#define LSM6DSM_FUNC_SRC1                 0x53 // Interrupt
#define LSM6DSM_FUNC_SRC2                 0x54 // Interrupt
#define LSM6DSM_WRIST_TILT_IA             0x55 // Wrist tilt interrupt
#define LSM6DSM_TAP_CFG                   0x58
#define LSM6DSM_TAP_THS_6D                0x59
#define LSM6DSM_INT_DUR2                  0x5A
#define LSM6DSM_WAKE_UP_THS               0x5B
#define LSM6DSM_WAKE_UP_DUR               0x5C
#define LSM6DSM_FREE_FALL                 0x5D
#define LSM6DSM_MD1_CFG                   0x5E
#define LSM6DSM_MD2_CFG                   0x5F
#define LSM6DSM_MASTER_MODE_CODE          0x60
#define LSM6DSM_SENS_SYNC_SPI_ERROR_CODE  0x61
#define LSM6DSM_OUT_MAG_RAW_X_L           0x66
#define LSM6DSM_OUT_MAG_RAW_X_H           0x67
#define LSM6DSM_OUT_MAG_RAW_Y_L           0x68
#define LSM6DSM_OUT_MAG_RAW_Y_H           0x69
#define LSM6DSM_OUT_MAG_RAW_Z_L           0x6A
#define LSM6DSM_OUT_MAG_RAW_Z_H           0x6B
#define LSM6DSM_INT_OIS                   0x6F
#define LSM6DSM_CTRL1_OIS                 0x70
#define LSM6DSM_CTRL2_OIS                 0x71
#define LSM6DSM_CTRL3_OIS                 0x72
#define LSM6DSM_X_OFS_USR                 0x73
#define LSM6DSM_Y_OFS_USR                 0x74
#define LSM6DSM_Z_OFS_USR                 0x75

#define AFS_2G  0x00
#define AFS_4G  0x02
#define AFS_8G  0x03
#define AFS_16G 0x01

#define GFS_245DPS  0x00
#define GFS_500DPS  0x01
#define GFS_1000DPS 0x02
#define GFS_2000DPS 0x03

#define AODR_12_5Hz  0x01  // same for accel and gyro in normal mode
#define AODR_26Hz    0x02
#define AODR_52Hz    0x03
#define AODR_104Hz   0x04
#define AODR_208Hz   0x05
#define AODR_416Hz   0x06
#define AODR_833Hz   0x07
#define AODR_1660Hz  0x08
#define AODR_3330Hz  0x09
#define AODR_6660Hz  0x0A

#define GODR_12_5Hz  0x01   
#define GODR_26Hz    0x02
#define GODR_52Hz    0x03
#define GODR_104Hz   0x04
#define GODR_208Hz   0x05
#define GODR_416Hz   0x06
#define GODR_833Hz   0x07
#define GODR_1660Hz  0x08
#define GODR_3330Hz  0x09
#define GODR_6660Hz  0x0A

  typedef struct LSM6DSMAccelData_s {
    int16_t x;
    int16_t y;
    int16_t z;
  } LSM6DSMAccelData;

    typedef struct LSM6DSMGyroData_s {
    int16_t x;
    int16_t y;
    int16_t z;
  } LSM6DSMGyroData;

class LSM6DSM
{
  public:
  LSM6DSM();
  LSM6DSM(uint8_t ADDR);

  bool begin(void);
  void read(void);
  void reset(void);
  uint8_t getChipID();

  void readRawAccel(void);
  void readRawGyro(void);
  uint16_t readRawTemp(void);

  void setAccelScale(uint8_t Ascale);
  void setGyroScale(uint8_t Gscale);
  void setAccelRate(uint8_t AODR);
  void setGyroRate(uint8_t GODR);

  LSM6DSMAccelData accelData; 
  LSM6DSMGyroData gyroData; 

  private:
  void writeByte(uint8_t register, uint8_t data);
  uint8_t readByte(uint8_t register);
  void readBytes(uint8_t register, uint8_t count, uint8_t * dest);

  uint8_t _ADDR;
};

#endif // __LSM6DSM_H__