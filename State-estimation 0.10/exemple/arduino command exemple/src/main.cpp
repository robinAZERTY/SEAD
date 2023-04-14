#include "../MPU9250/scr/MPU9250.h"
#include <Adafruit_BMP085.h>
#include <TinyGPSPlus.h>

#define USE_B_or_BLE false
#define JUST_INCLUDE false

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
Adafruit_BMP085 bmp;
TinyGPSPlus gps;

int freq = 10;
int status = -1;
int prev_status = status;
int data_to_send = -1;


#if USE_B_or_BLE
  #include "BluetoothSerial.h"
  #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
    #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
  #endif

  #if !JUST_INCLUDE

    BluetoothSerial SerialBT;

    int *get_command()
    {
      int *ret = new int[2]; //[0] : command, [1] : option
      ret[0] = -1;
      ret[1] = -1;

      if (!SerialBT.available())
        return ret;

      // extract the command from the BT serial

      // read the firt int value from SerialBT
      ret[0] = int(SerialBT.parseInt());
      if (ret[0] == 5 || ret[0] == 6)
        ret[1] = int(SerialBT.parseInt());
      // read the rest of the line
      while (SerialBT.available())
        SerialBT.read();
      // 0 : standby
      // 1 : sending data
      // 2 : calibration
      // 3 : read calibration
      // 4 : write calibration
      // 5 : change the frequency of acting
      // 6 : change the data type of data sending
      return ret;
    }

    void send_data(int data_type)
    {
      if(data_type==-1) return;

      const char sep = '\t';
      String msg = "";
      // read the sensors
      IMU.readSensor();
      // display the data
      // sensor reading : (0:acc, 1:gyr, 2:mag, 3:temp, 4:press, 5:gps_pos)
      if (data_type == 0)
      {
        msg += "accX:" + String(IMU.getAccelX_mss(),3) + sep;
        msg += "accY:" + String(IMU.getAccelY_mss(),3) + sep;
        msg += "accZ:" + String(IMU.getAccelZ_mss(),3);
      }
      else if (data_type == 1)
      {
        msg += "gyroX:" + String(IMU.getGyroX_rads(), 4) + sep;
        msg += "gyroY:" + String(IMU.getGyroY_rads(), 4) + sep;
        msg += "gyroZ:" + String(IMU.getGyroZ_rads(), 4);
      }
      else if (data_type == 2)
      {
        msg += "magX:" + String(IMU.getMagX_uT(),3) + sep;
        msg += "magY:" + String(IMU.getMagY_uT(),3) + sep;
        msg += "magZ:" + String(IMU.getMagZ_uT(),3);
      }
      else if (data_type == 3)
      {
        msg += "tempIMU:" + String(IMU.getTemperature_C(),3) + sep;
        msg += "tempBMP:" + String(bmp.readTemperature(),3);
      }
      else if (data_type == 4)
      {
        msg += "press:" + String(bmp.readPressure(),3);
      }
      else if (data_type == 5)
      {
        msg += "lat:" + String(gps.location.lat(), 6) + sep;
        msg += "lng:" + String(gps.location.lng(), 6) + sep;
        msg += "alt:" + String(gps.altitude.meters(), 3);
      }
      SerialBT.println(msg);
    }

    unsigned long t0 = 0;
    void act(int *arg)
    { 
      

      if (arg[0]!=-1)
      {
        prev_status = status;
        status = arg[0];
        //Serial.println("new command :"+String(status));
      }

      unsigned long t = millis();

      if (t - t0 < 1000 / freq)
        return void();
      t0 = t;

      
      if (status == 1)
      {
        return send_data(data_to_send);
      }
      if (status == 5)
      {
        freq = arg[1];
        status = prev_status;
        return void();
      }
      if (status == 6)
      {
        data_to_send = arg[1];
        status = prev_status;
        return void();
      }
    }

    void setup()
    {
      // serial to display data
      Serial.begin(115200);
      SerialBT.setTimeout(100);
      SerialBT.begin("ESP32test"); // Bluetooth device name
      while (!SerialBT.connected())
      {
      } // wait for connection
      SerialBT.println("Connected");
      IMU.begin();
    }
    void loop()
    {
      act(get_command());
      delay(50);
    }
  #else

  void setup()
  {
  }
  void loop()
  {
  }
  #endif

#else
  #include <BLEDevice.h>
  #include <BLEUtils.h>
  #include <BLEServer.h>

  #define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
  #define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


  class MyCallbacks: public BLECharacteristicCallbacks {
      void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();

        if (value.length() > 0) {
          Serial.println("*********");
          Serial.print("New value: ");
          for (int i = 0; i < value.length(); i++)
            Serial.print(value[i]);

          Serial.println();
          Serial.println("*********");
        }
      }
  };

  void setup() {
    Serial.begin(115200);

    Serial.println("1- Download and install an BLE scanner app in your phone");
    Serial.println("2- Scan for BLE devices in the app");
    Serial.println("3- Connect to MyESP32");
    Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
    Serial.println("5- See the magic =)");

    BLEDevice::init("MyESP32");
    BLEServer *pServer = BLEDevice::createServer();

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                        );

    pCharacteristic->setCallbacks(new MyCallbacks());

    pCharacteristic->setValue("Hello World");
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
  }

  void loop() {
    // put your main code here, to run repeatedly:
    delay(2000);
  }

#endif