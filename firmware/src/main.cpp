#include <Arduino.h>

// To surpress warning
#define FASTLED_INTERNAL
#include "FastLED.h"

// IMU Library
#include "LSM6DSM.h"

// Total number of leds in the array
#define NUM_LEDS 8

// Data pin that led data will be written out over
#define DATA_PIN 3 //PB3

// These need to be initalized to a non 0 value for some reason
int16_t prevX = 1;
int16_t prevY = 1;
int16_t prevZ = 1;

LSM6DSM imu(LSM6DSM_ADDRESS_PULLUP);

// This is an array of leds.
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  imu.begin();
}

void loop() {
  imu.readRawAccel();

  // Calculate change from last accel data
  // Divide by 2^6 to map to a lower sensitivity
  uint8_t red =  abs((imu.accelData.x >> 6) - (prevX >> 6));
  uint8_t green = abs((imu.accelData.y >> 6) - (prevY >> 6));
  uint8_t blue = abs((imu.accelData.z >> 6) - (prevZ >> 6));

  // imu.readRawGyro();
  // uint8_t red = abs(imu.gyroData.x >> 6);
  // uint8_t green = abs(imu.gyroData.x >> 6);
  // uint8_t blue = abs(imu.gyroData.x >> 6);

  // // If LEDs are too bright at lowest brightness
  // // Set min threshold out of 255
  // red = red < 5 ? 0 : red;
  // green = green < 5 ? 0 : green;
  // blue = blue < 5 ? 0 : blue;

  // Store current reading as new previous data
  prevX = imu.accelData.x;
  prevY = imu.accelData.y;
  prevZ = imu.accelData.z;

  // Shift all LEDs down one spot
  memmove(&leds[1], &leds[0], NUM_LEDS * sizeof(CRGB) - 1);
  // Set color of 0th LED
  leds[0] = CRGB(red, green, blue);
  FastLED.show();
  delay(25);
}