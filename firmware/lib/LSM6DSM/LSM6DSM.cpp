#include "LSM6DSM.h"

LSM6DSM::LSM6DSM() { LSM6DSM(LSM6DSM_ADDRESS_PULLUP); }

LSM6DSM::LSM6DSM(uint8_t ADDR) {
  _ADDR = ADDR;
  TinyWireM.begin();
}

uint8_t LSM6DSM::getChipID() { return readByte(LSM6DSM_WHO_AM_I); }

void LSM6DSM::reset() {
  writeByte(LSM6DSM_CTRL3_C, 0x01);  // Set bit 0 to 1 to reset LSM6DSM
  delay(100);                        // Wait for all registers to reset
}

bool LSM6DSM::begin(void) {
  setAccelRate(AFS_16G);
  setGyroScale(GFS_1000DPS);
  setAccelRate(AODR_833Hz);
  setGyroRate(GODR_52Hz);

  // Enable Block Update
  uint8_t CTRL3_C = readByte(LSM6DSM_CTRL3_C);
  writeByte(LSM6DSM_CTRL3_C, CTRL3_C | 0x01 << 6);

  return true;
}

void LSM6DSM::readRawAccel(void) {
  uint8_t rawAccel[6];
  readBytes(LSM6DSM_OUTX_L_XL, sizeof(rawAccel) / sizeof(rawAccel[0]),
            &rawAccel[0]);
  accelData.x = ((int16_t)rawAccel[1] << 8) | rawAccel[0];
  accelData.y = ((int16_t)rawAccel[3] << 8) | rawAccel[2];
  accelData.z = ((int16_t)rawAccel[5] << 8) | rawAccel[4];
}

void LSM6DSM::readRawGyro(void) {
  uint8_t rawGyro[6];
  readBytes(LSM6DSM_OUTX_L_G, sizeof(rawGyro) / sizeof(rawGyro[0]),
            &rawGyro[0]);
  gyroData.x = ((int16_t)rawGyro[1] << 8) | rawGyro[0];
  gyroData.y = ((int16_t)rawGyro[3] << 8) | rawGyro[2];
  gyroData.z = ((int16_t)rawGyro[5] << 8) | rawGyro[4];
}

uint16_t LSM6DSM::readRawTemp(void) {
  uint8_t rawTemp[2];
  readBytes(LSM6DSM_OUT_TEMP_L, sizeof(rawTemp) / sizeof(rawTemp[0]),
            &rawTemp[0]);
  return ((int16_t)rawTemp[1] << 8) | rawTemp[0];
}

void LSM6DSM::setAccelScale(uint8_t Ascale) {
  uint8_t CTRL1_XL = readByte(LSM6DSM_CTRL1_XL);
  CTRL1_XL = CTRL1_XL & ~(0x03 << 2);
  writeByte(LSM6DSM_CTRL1_XL, CTRL1_XL | Ascale << 2);
}
void LSM6DSM::setGyroScale(uint8_t Gscale) {
  uint8_t CTRL2_G = readByte(LSM6DSM_CTRL2_G);
  CTRL2_G = CTRL2_G & ~(0x03 << 2);
  writeByte(LSM6DSM_CTRL2_G, CTRL2_G | Gscale << 2);
}
void LSM6DSM::setAccelRate(uint8_t AODR) {
  uint8_t CTRL1_XL = readByte(LSM6DSM_CTRL1_XL);
  CTRL1_XL = CTRL1_XL & ~(0x0F << 4);
  writeByte(LSM6DSM_CTRL1_XL, CTRL1_XL | AODR << 4);
}
void LSM6DSM::setGyroRate(uint8_t GODR) {
  uint8_t CTRL2_G = readByte(LSM6DSM_CTRL2_G);
  CTRL2_G = CTRL2_G & ~(0x0F << 4);
  writeByte(LSM6DSM_CTRL2_G, CTRL2_G | GODR << 4);
}

// I2C read/write functions for the LSM6DSM

void LSM6DSM::writeByte(uint8_t reg, uint8_t data) {
  TinyWireM.beginTransmission(_ADDR);
  TinyWireM.write(reg);
  TinyWireM.write(data);
  TinyWireM.endTransmission();
}

uint8_t LSM6DSM::readByte(uint8_t reg) {
  TinyWireM.beginTransmission(_ADDR);
  TinyWireM.write(reg);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(_ADDR, 1);
  while (!TinyWireM.available()) {
  }
  return TinyWireM.read();
}

void LSM6DSM::readBytes(uint8_t reg, uint8_t count, uint8_t* dest) {
  TinyWireM.beginTransmission(_ADDR);
  TinyWireM.write(reg);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(_ADDR, count);
  while (TinyWireM.available() < count) {
  }
  for (uint16_t i = 0; i < count; i++) {
    dest[i] = TinyWireM.read();
  }
}