#define USE_BT

#ifdef USE_BT
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#endif


#include "../MPU9250/scr/MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;

void print_msg(String msg)
{
  #ifdef USE_BT
  SerialBT.print(msg);
  #else
  Serial.print(msg);
  #endif
}

void print_calibration()
{
  // print the calibration data
  //gyroscope
  String msg = "";
  msg += "Gyro bias: ";
  msg += String(IMU._calData.gyr.Bias[0], 4);
  msg += ", ";
  msg += String(IMU._calData.gyr.Bias[1], 4);
  msg += ", ";
  msg += String(IMU._calData.gyr.Bias[2], 4);
  msg += "\n";
  msg += "Gyro std dev: ";
  msg += String(IMU._calData.gyr.std_dev, 4);
  msg += "\n";
  msg += "Acc rotation matrix: ";
  msg += String(IMU._calData.acc.RotationMatrix[0][0], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[0][1], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[0][2], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[1][0], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[1][1], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[1][2], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[2][0], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[2][1], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.RotationMatrix[2][2], 4);
  msg += "\n";
  msg += "Acc bias: ";
  msg += String(IMU._calData.acc.Bias[0], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.Bias[1], 4);
  msg += ", ";
  msg += String(IMU._calData.acc.Bias[2], 4);
  msg += "\n";
  msg += "Acc std dev: ";
  msg += String(IMU._calData.acc.std_dev, 4);
  msg += "\n";
  msg += "Mag rotation matrix: ";
  msg += String(IMU._calData.mag.RotationMatrix[0][0], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[0][1], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[0][2], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[1][0], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[1][1], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[1][2], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[2][0], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[2][1], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.RotationMatrix[2][2], 4);
  msg += "\n";
  msg += "Mag bias: ";
  msg += String(IMU._calData.mag.Bias[0], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.Bias[1], 4);
  msg += ", ";
  msg += String(IMU._calData.mag.Bias[2], 4);
  msg += "\n";
  msg += "Mag std dev: ";
  msg += String(IMU._calData.mag.std_dev, 4);
  msg += "\n";

  print_msg(msg);
}



void setup() {
  // serial to display data
  #ifdef USE_BT
  SerialBT.begin("ESP32test");
  while(!SerialBT.connected()) {}
  SerialBT.println("The device started and connected !");
  Serial.println("The device started and connected !");
  #else
  Serial.begin(115200);
  while(!Serial) {}
  #endif

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    String msg = "IMU initialization unsuccessful";
    msg += "Status: ";
    msg += String(status);
    msg += "\n";
    print_msg(msg);
    while(1) {}
  }

  print_msg("IMU initialization successful\n");
  print_msg("Calibrating...\n");
  IMU.read_from_flash_gyro_cal();

  //IMU.calibrateGyro_advanced(100,2,true);
  //IMU.calibrateAccel_advanced(30,true);
  String mag_cal_status = "";
  while (mag_cal_status != "COMPLETED") {
    mag_cal_status = IMU.mag_calibration_advanced_step(100);
    if (mag_cal_status !="")print_msg(mag_cal_status+"\n");
    delay(100);
  }
  
  
  delay(1000);

  //IMU.read_from_flash_gyro_cal();
  IMU.read_from_flash_accel_cal();
  print_calibration();

}

void loop() {
delay(1000);
}