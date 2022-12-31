#include "../MPU9250/scr/MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;


void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  Serial.println("IMU initialization successful");
  Serial.println("Starting calibration... please don't move the IMU");

  //IMU.calibrateGyro_advanced(100,2,true);
  delay(1000);
  IMU.readCalibrationFromFlash();
  //IMU.calibrateAccel_advanced(30);
}

void loop() {
delay(1000);
}