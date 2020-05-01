#include <LSM303.h>

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/
bool LSM303::begin() {
  TinyWireM.begin();

  // Enable the accelerometer
  write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x27);
  // Set to +-8G
  write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG4_A, (2 << 4));

  return true;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
void LSM303::read() {
  // Read the accelerometer
  TinyWireM.beginTransmission((byte)LSM303_ADDRESS_ACCEL);
  TinyWireM.write(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom((byte)LSM303_ADDRESS_ACCEL, (byte)6);

  // Wait around until enough data is available
  while (TinyWireM.available() < 6)
    ;

  uint8_t xlo = TinyWireM.read();
  uint8_t xhi = TinyWireM.read();
  uint8_t ylo = TinyWireM.read();
  uint8_t yhi = TinyWireM.read();
  uint8_t zlo = TinyWireM.read();
  uint8_t zhi = TinyWireM.read();

  // Shift values to create properly formed integer (low byte first)
  accelData.x = (int16_t)(xlo | (xhi << 8));
  accelData.y = (int16_t)(ylo | (yhi << 8));
  accelData.z = (int16_t)(zlo | (zhi << 8));
}

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
void LSM303::write8(byte address, byte reg, byte value) {
  TinyWireM.beginTransmission(address);
  TinyWireM.write(reg);
  TinyWireM.write(value);
  TinyWireM.endTransmission();
}