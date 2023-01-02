#include "../MPU9250/scr/MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;

void print_calibration()
{
  // print the calibration data
  //gyroscope
  Serial.print("Gyro bias: ");
  Serial.print(IMU._calData.gyr.Bias[0]);
  Serial.print(", ");
  Serial.print(IMU._calData.gyr.Bias[1]);
  Serial.print(", ");
  Serial.println(IMU._calData.gyr.Bias[2]);
  Serial.print("Gyro std dev: ");
  Serial.println(IMU._calData.gyr.std_dev);
  Serial.print("Acc rotation matrix: ");
  Serial.print(IMU._calData.acc.RotationMatrix[0][0]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.RotationMatrix[0][1]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.RotationMatrix[0][2]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.RotationMatrix[1][0]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.RotationMatrix[1][1]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.RotationMatrix[1][2]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.RotationMatrix[2][0]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.RotationMatrix[2][1]);
  Serial.print(", ");
  Serial.println(IMU._calData.acc.RotationMatrix[2][2]);
  Serial.print("Acc bias: ");
  Serial.print(IMU._calData.acc.Bias[0]);
  Serial.print(", ");
  Serial.print(IMU._calData.acc.Bias[1]);
  Serial.print(", ");
  Serial.println(IMU._calData.acc.Bias[2]);
  Serial.print("Acc std dev: ");
  Serial.println(IMU._calData.acc.std_dev);
  Serial.print("Mag rotation matrix: ");
  Serial.print(IMU._calData.mag.RotationMatrix[0][0]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.RotationMatrix[0][1]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.RotationMatrix[0][2]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.RotationMatrix[1][0]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.RotationMatrix[1][1]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.RotationMatrix[1][2]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.RotationMatrix[2][0]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.RotationMatrix[2][1]);
  Serial.print(", ");
  Serial.println(IMU._calData.mag.RotationMatrix[2][2]);
  Serial.print("Mag bias: ");
  Serial.print(IMU._calData.mag.Bias[0]);
  Serial.print(", ");
  Serial.print(IMU._calData.mag.Bias[1]);
  Serial.print(", ");
  Serial.println(IMU._calData.mag.Bias[2]);
  Serial.print("Mag std dev: ");
  Serial.println(IMU._calData.mag.std_dev);
}

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
  IMU.read_from_flash_gyro_cal();
  //IMU.calibrateGyro_advanced(100,2,true);
  //IMU.calibrateAccel_advanced(30,true);
  IMU.calibrateMag_advanced(100,true);
  
  delay(1000);

  //IMU.read_from_flash_gyro_cal();
  IMU.read_from_flash_accel_cal();
  print_calibration();

}

void loop() {
delay(1000);
}