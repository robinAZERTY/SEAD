
#include <Adafruit_BMP085.h>
#include "MPU9250.h"
#include <TinyGPSPlus.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
Adafruit_BMP085 bmp;
TinyGPSPlus gps;
BluetoothSerial SerialBT;

String data_to_print = "acc";

void initialiseSerial()
{
  if (data_to_print == "gps")
  {
    SerialBT.println("lat\tlng\talt");
  }
  else if (data_to_print == "acc")
  {
    SerialBT.println("accX\taccY\taccZ");
  }
  else if (data_to_print == "gyro")
  {
    SerialBT.println("gyroX\tgyroY\tgyroZ");
  }
  else if (data_to_print == "mag")
  {
    SerialBT.println("magX\tmagY\tmagZ");
  }
  else if (data_to_print == "temp")
  {
    SerialBT.println("tempIMU\ttempBMP");
  }
  else if (data_to_print == "press")
  {
    SerialBT.println("press");
  }
  else
  {
    SerialBT.println("error");
  }
}
const String sep = "\t";

void updateSerial()
{

  // update data_to_print

  if (SerialBT.available())
  {
    data_to_print = SerialBT.readStringUntil('\n');
    if (data_to_print != "gps" && data_to_print != "acc" && data_to_print != "gyro" && data_to_print != "mag" && data_to_print != "temp" && data_to_print != "press")
    {
      SerialBT.println("you must enter gps, acc, gyro, mag, temp or press");
    }
    else
    {
      initialiseSerial();
    }
  }
String msg = "";
// read the sensors
IMU.readSensor();
// display the data

if (data_to_print == "gps")
{
  msg += "lat:"+ String(gps.location.lat(), 6) + sep;
  msg += "lng:"+ String(gps.location.lng(), 6) + sep;
  msg += "alt:"+ String(gps.altitude.meters());
}
else if (data_to_print == "acc")
{
  msg += "accX:"+ String(IMU.getAccelX_mss()) + sep;
  msg += "accY:"+ String(IMU.getAccelY_mss()) + sep;
  msg += "accZ:"+ String(IMU.getAccelZ_mss());
}
else if (data_to_print == "gyro")
{
  msg += "gyroX:"+ String(IMU.getGyroX_rads(),4) + sep;
  msg += "gyroY:"+ String(IMU.getGyroY_rads(),4) + sep;
  msg += "gyroZ:"+ String(IMU.getGyroZ_rads(),4);
}
else if (data_to_print == "mag")
{
  msg += "magX:"+ String(IMU.getMagX_uT()) + sep;
  msg += "magY:"+ String(IMU.getMagY_uT()) + sep;
  msg += "magZ:"+ String(IMU.getMagZ_uT());
}
else if (data_to_print == "temp")
{
  msg += "tempIMU:"+ String(IMU.getTemperature_C()) + sep;
  msg += "tempBMP:"+ String(bmp.readTemperature());
}
else if (data_to_print == "press")
{
  msg += "press:"+ String(bmp.readPressure());
}
SerialBT.println(msg);
}

void setup()
{
  // serial to display data
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); // Bluetooth device name
  while (!SerialBT.connected())
  {
  } // wait for connection
  IMU.begin();
  bmp.begin();
}

void loop()
{
  updateSerial();
  delay(2000);
}